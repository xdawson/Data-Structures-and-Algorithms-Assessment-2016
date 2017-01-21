#include <stdio.h>
#include "Trie.h"
#include "FileOperations.h"
#include <malloc.h>
#include <conio.h>
#include <string.h>

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
		//NEED TO TEST THIS IN LABSSS
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

void
FlushBuffer(char *Buffer, int BufferSize)
{
	int i;
	for(i = 0; i < BufferSize; i++)
	{
		Buffer[i] = 0;
	}
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

int 
NumberOfEnteredCharacters(char *Buffer)
{
	int Position = 0;
	while(Buffer[Position] != 0)
	{
		Position++;
	}
	return Position;
}

int
main()
{
	void *FileMemory = LoadFile("data/words.txt");

	char *TextFile = (char *)FileMemory;

	trie_root_node* Root = CreateTrie(TextFile);

	char KeyPressed = 0;

	//ASCII Keycodes
	int EscapeKey = 27;

	int SpaceKey = 32;

	int TabKey = 9;

	//20 here is a starting estimate of how long the largest work in the
	//dictionary is. Probably good to find the size of the largest word when poplating the trie
	int BufferSize = 20;
	//A large number
	//space for 256 characters
	int InputBufferSize = 256;
	int InputBufferPosition = 0;

	char *InputBuffer = malloc(sizeof(char) * InputBufferSize);
	char *PrefixBuffer = malloc(sizeof(char) * BufferSize);
	char *OutputBuffer = malloc(sizeof(char) * BufferSize);
	//flush buffer before use
	FlushBuffer(InputBuffer, InputBufferSize);
	FlushBuffer(PrefixBuffer, BufferSize);
	FlushBuffer(OutputBuffer, BufferSize);

	int LetterCounter = 0;

	//text entry loop
	while(KeyPressed != EscapeKey)
	{
		do
		{
			KeyPressed = getch();

			//only test for tab?
			if(KeyPressed != TabKey)
			{
				InputBuffer[InputBufferPosition++] = KeyPressed;				
			}
		}
		while(KeyPressed == 0);

		if(KeyPressed == SpaceKey)
		{
			//clear the prefix buffer
			FlushBuffer(PrefixBuffer, BufferSize);
			LetterCounter = 0;
		}
		else if(KeyPressed == TabKey)
		{
			trie_node *PrefixNode = FindPrefixNode(Root, PrefixBuffer);

			if(PrefixNode)
			{
				strcpy(OutputBuffer, PrefixBuffer);

				trie_node *Temp = PrefixNode;				

				//find the first 0 char of output buffer
				int Position = 0;
				while(OutputBuffer[Position] != 0)
				{
					Position++;
				}

				trie_node *Option;

				while(Temp != NULL)
				{
					OutputBuffer[Position++] = Temp->Value;

					if(Temp->WordMarker == 1)
					{
						
						Option = Temp;
						printf("\n1. %s", OutputBuffer);

						int Chosen = 0;
						while(!Chosen)
						{
							char Key = getch();

							if(Key != 0)
							{
								//accept word
								if(Key == '1')
								{
									strcpy(&InputBuffer[(InputBufferPosition - LetterCounter)], OutputBuffer);
									InputBufferPosition = NumberOfEnteredCharacters(InputBuffer);

									InputBuffer[InputBufferPosition++] = SpaceKey; 
									FlushBuffer(PrefixBuffer, BufferSize);
									LetterCounter = 0;
									Chosen = 1;
								}
								//reject word
								else
								{
									InputBuffer[InputBufferPosition++] = Key;
									PrefixBuffer[LetterCounter++] = Key;
									Chosen = 1;
								}
								
								FlushBuffer(OutputBuffer, BufferSize);
							}
						}

						break;
					}
					
					Temp = Temp->Child;
				}
			}
		}
		else
		{
			PrefixBuffer[LetterCounter++] = KeyPressed;
		}

		printf("\n%s", InputBuffer);


	}



	FreeFile(FileMemory);

	return 0;
}
