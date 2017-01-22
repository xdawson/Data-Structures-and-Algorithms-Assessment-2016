#include "Trie.h"
#include <stdio.h>
#include <malloc.h>


trie_node *
TrieNodeConstructor(char Value)
{
	trie_node *Node = malloc(sizeof(trie_node));

	Node->Child = NULL;
	Node->Sibling = NULL;

	Node->Value = Value;
	Node->WordMarker = 0;

	return Node;
}

//Seems like I should some sort of non-implementation dependant name for these
//to go with the whole data hiding ideology
trie_root_node *
TrieRootNodeConstructor()
{
	//get memory for Trie root node
	trie_root_node *Root = malloc(sizeof(trie_root_node));

	Root->FirstLetter = TrieNodeConstructor(0);

	return Root;
}

int 
NumberOfSiblings(trie_node *CurrentNode)
{
	trie_node *Temp = CurrentNode;

	int NumberOfSiblings = 0;

	while(Temp->Sibling != NULL)
	{
		Temp = Temp->Sibling;
		NumberOfSiblings++;
	}

	return NumberOfSiblings;
}

trie_node *
MoveToChild(trie_node *CurrentNode)
{
	if(CurrentNode->Child != NULL)
	{
		CurrentNode = CurrentNode->Child;
	}
	else
	{
		CurrentNode->Child = TrieNodeConstructor(0);
		CurrentNode = CurrentNode->Child;
	}

	return CurrentNode;
}

trie_node *
FindNextNode(trie_root_node *Root, trie_node *CurrentNode, char *TextFile, int Position)
{
	if(TextFile[Position + 1] == '\r')
	{
		CurrentNode->WordMarker = 1;
		CurrentNode = Root->FirstLetter;
	}
	else
	{
		CurrentNode = MoveToChild(CurrentNode);
	}

	return CurrentNode;
}


//might need to move this to a Trie file to do data hiding things?
//TODO split this up into smaller functions and comment the shit out of this mess
trie_root_node*
CreateTrie(char* TextFile)
{
	trie_root_node *Root = TrieRootNodeConstructor();

	int Position;

	trie_node *CurrentNode = Root->FirstLetter;

	for(Position = 0; TextFile[Position] != 0; Position++)
	{
		//the \r case will already have reset the CurrentNode to Root
		//so here we can just skip \n
		if((TextFile[Position] == '\r') || (TextFile[Position] == '\n'))
		{
			continue;
		}

		//This is here because all the weird characters
		//at the end of the text file have values of < 0
		if(TextFile[Position] < 0)
		{
			continue;
		}

		if(TextFile[Position] == CurrentNode->Value)
		{
			//useful here as checks for repeated words
			CurrentNode = FindNextNode(Root, CurrentNode, TextFile, Position);
		}
		else
		{
			if(CurrentNode->Value == 0)
			{
				CurrentNode->Value = TextFile[Position];
				CurrentNode = FindNextNode(Root, CurrentNode, TextFile, Position);
			}
			else
			{
				int i;
				int NumSiblings = NumberOfSiblings(CurrentNode);

				for(i = 0; i <= NumSiblings; i++)
				{
					if(CurrentNode->Value == TextFile[Position])
					{
						CurrentNode = FindNextNode(Root, CurrentNode, TextFile, Position);
						break;
					}
					else
					{
						if(CurrentNode->Sibling == NULL)
						{
							CurrentNode->Sibling = TrieNodeConstructor(TextFile[Position]);
							CurrentNode = CurrentNode->Sibling;

							CurrentNode = FindNextNode(Root, CurrentNode, TextFile, Position);
						}
						else
						{
							CurrentNode = CurrentNode->Sibling;
						}
					}

				}
			}
		}
	}
	return Root;
}

trie_node *
FindPrefixNode(trie_root_node *Root, char *PrefixBuffer)
{
	trie_node *CurrentNode = Root->FirstLetter;

	//find the position of this prefix in the trie
	int i;
	for(i = 0; PrefixBuffer[i] != 0; i++)
	{
		if(PrefixBuffer[i] == CurrentNode->Value)
		{
			CurrentNode = CurrentNode->Child;
		}
		else
		{
			int j;
			int NumSiblings = NumberOfSiblings(CurrentNode);

			for(j = 0; j <= NumSiblings; j++)
			{
				if(PrefixBuffer[i] == CurrentNode->Value)
				{
					CurrentNode = CurrentNode->Child;
					break;
				}
				else
				{
					if(CurrentNode->Sibling != NULL)
					{
						CurrentNode = CurrentNode->Sibling;								
					}
					else
					{
						printf("\nNo words with current prefix\n");
						return NULL;
					}
				}
			}
		}
	}
	return CurrentNode;
}


//TODO destructors
//TODO insert(maybe) and search functions