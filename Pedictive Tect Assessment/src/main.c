#include <stdio.h>
#include "Trie.h"
#include "FileOperations.h"
#include <malloc.h>
#include <conio.h>
#include <string.h>


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
			//check
			if(TextFile[Position + 1] == '\r')
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
				if(TextFile[Position + 1] == '\r')
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
						if(TextFile[Position + 1] == '\r')
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
					//check
					if(TextFile[Position + 1] == '\r')
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
					CurrentNode->Sibling = TrieNodeConstructor(TextFile[Position]);
					CurrentNode = CurrentNode->Sibling;

					//check
					if(TextFile[Position + 1] == '\r')
					{
						CurrentNode->WordMarker = 1;
						CurrentNode = Root->FirstLetter;
					}
					else
					{
						CurrentNode->Child = TrieNodeConstructor(0);
						CurrentNode = CurrentNode->Child;
					}
				}
			}
		}
	}

	return Root;
}

int
main()
{
	void *FileMemory = LoadFile("data/words.txt");

	char *TextFile = (char *)FileMemory;

	trie_root_node* Root = CreateTrie(TextFile);

	char KeyPressed;

	//ASCII Keycodes
	int EscapeKey = 27;

	int SpaceKey = 32;

	int TabKey = 9;

	//20 here is a starting estimate of how long the largest work in the 
	//dictionary is. Probably good to find the size of the largest word when poplating the trie
	int BufferSize = 20;
	char *PrefixBuffer = malloc(sizeof(char) * BufferSize);
	char *OutputBuffer = malloc(sizeof(char) * BufferSize);
	//flush buffer before use
	int i;
	for(i = 0; i < BufferSize; i++)
	{
		PrefixBuffer[i] = 0;
	}

	int LetterCounter = 0;

	//text entry loop
	while(KeyPressed != EscapeKey)
	{
		do
		{
			KeyPressed = getch();
			printf("%c", KeyPressed);
		}
		while(KeyPressed == 0);

		if(KeyPressed == SpaceKey)
		{
			//clear the prefix buffer
			int i;
			for(i = 0; i < BufferSize; i++)
			{
				PrefixBuffer[i] = 0;
			}

			LetterCounter = 0;
		}
		else if(KeyPressed == TabKey)
		{
			//give suggestions
			trie_node *CurrentNode = Root->FirstLetter;

			int FoundPrefix = 1;

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
					while(CurrentNode->Sibling != NULL)
					{
						if(CurrentNode->Value == PrefixBuffer[i])
						{
							CurrentNode = CurrentNode->Child;
							break;
						}
						else
						{
							CurrentNode = CurrentNode->Sibling;
						}
					}
					//continue;

					//maybe dont need if as i know the control flow,
					//will only get here if sibling == NULL
					if(CurrentNode->Sibling == NULL)
					{
						if(CurrentNode->Value == PrefixBuffer[i])
						{
							CurrentNode = CurrentNode->Child;
						}
						else
						{
							printf("No words with current prefix\n");
							FoundPrefix = 0;
							//breaks out of the for loop
							break;
						}
					}


				}
			}

			//found current node
			if(FoundPrefix)
			{
				strcpy(OutputBuffer, PrefixBuffer);

				printf("%s", OutputBuffer);

				
				/*int i;
				int NumberOfOptionsToDisplay = 3;
				for(i = 0; i < NumberOfOptionsToDisplay; i++)
				{
					
				}*/
			}

		}
		else
		{
			PrefixBuffer[LetterCounter++] = KeyPressed;
		}



	}



	FreeFile(FileMemory);

	return 0;
}
