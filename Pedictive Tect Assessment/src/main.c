#include <stdio.h>
#include "Trie.h"
#include "FileOperations.h"

int
main()
{
	void *FileMemory = LoadFile("data/words.txt");

	char *TextFile = (char *)FileMemory;

	trie_root_node *Root = TrieRootNodeConstructor();

	int Position;

	trie_node *CurrentNode = Root->FirstLetter;

	printf("population started\n");

	for(Position = 0; TextFile[Position] != 0; Position++)
	{
		//the \r case will already have reset the CurrentNode to Root
		//so here we can just skip \n
		if(TextFile[Position] == '\n')
		{
			continue;
		}

		//This is here because all the weird characters 
		//at the end of the text file have values of < 0
		//NEED TO TEST THIS IN LABSSS
		if(TextFile[Position] < 0)
		{
			continue;
		}

		if(TextFile[Position] == CurrentNode->Value)
		{
			//useful here as checks for repeated words
			//check
			if(TextFile[Position] == '\r')
			{
				CurrentNode->WordMarker = 1;
				CurrentNode = Root->FirstLetter;
			}
			else
			{
				//deal with it
				if(CurrentNode->Child != NULL)
				{
					CurrentNode = CurrentNode->Child;		
				}
				else
				{
					CurrentNode->Child = TrieNodeConstructor(0);
					CurrentNode = CurrentNode->Child;
				}
			}
		}
		else
		{
			if(CurrentNode->Value == 0)
			{
				CurrentNode->Value = TextFile[Position];

				//check
				if(TextFile[Position] == '\r')
				{
					CurrentNode->WordMarker = 1;
					CurrentNode = Root->FirstLetter;
				}
				else
				{
					//deal with it
					if(CurrentNode->Child != NULL)
					{
						CurrentNode = CurrentNode->Child;		
					}
					else
					{
						CurrentNode->Child = TrieNodeConstructor(0);
						CurrentNode = CurrentNode->Child;
					}
				}
			}
			else
			{
				while(CurrentNode->Sibling != NULL)
				{
					if(CurrentNode->Value == TextFile[Position])
					{
						//check
						if(TextFile[Position] == '\r')
						{
							CurrentNode->WordMarker = 1;
							CurrentNode = Root->FirstLetter;
						}
						else
						{
							//deal with it
							if(CurrentNode->Child != NULL)
							{
								CurrentNode = CurrentNode->Child;		
							}
							else
							{
								CurrentNode->Child = TrieNodeConstructor(0);
								CurrentNode = CurrentNode->Child;
							}
						}
						break;
					}
					else
					{
						CurrentNode = CurrentNode->Sibling;
					}
				}

				//dealing with case of the last link with Sibling being Null but this being the right 
				//value
				if(CurrentNode->Value == TextFile[Position])
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
				}
				else if(CurrentNode->Sibling == NULL)
				{
					CurrentNode->Sibling = TrieNodeConstructor(TextFile[Position]);
					CurrentNode = CurrentNode->Sibling;
					CurrentNode->Child = TrieNodeConstructor(0);
					CurrentNode = CurrentNode->Child;
				}

			}
		}
	}

	printf("population ended\n");			

	FreeFile(FileMemory);

	return 0;
}