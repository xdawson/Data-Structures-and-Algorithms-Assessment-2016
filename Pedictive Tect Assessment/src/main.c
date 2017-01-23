#include <stdio.h>
#include "Trie.h"
#include "FileOperations.h"
#include "Buffer.h"
#include <malloc.h>
#include <conio.h>
#include <string.h>

void
PredictiveText(trie_root_node *Root)
{
	char KeyPressed = 0;

	//ASCII Keycodes
	int EscapeKey = 27;
	int SpaceKey = 32;
	int TabKey = 9;

	//1KB allocated for the input buffer
	buffer InputBuffer = BufferConstructor(1024);
	//Using 20 here assuming no word in the inputted Textfile is longer than 20 characters
	//long. Ideally I would have a counter going as the Trie is made and return the amount of characters in
	//the longest word.
	buffer PrefixBuffer = BufferConstructor(20);
	buffer OutputBuffer = BufferConstructor(20);

	//flush buffers before use
	FlushBuffer(InputBuffer);
	FlushBuffer(PrefixBuffer);
	FlushBuffer(OutputBuffer);

	//counts the letters in a certain word
	int LetterCounter = 0;

	//text entry loop
	while(KeyPressed != EscapeKey)
	{

		do
		{
			KeyPressed = getch();

			if(KeyPressed != TabKey)
			{
				InputBuffer.Memory[InputBuffer.Position++] = KeyPressed;
			}
		}
		while(KeyPressed == 0);

		if(KeyPressed == SpaceKey)
		{
			//clear the prefix buffer
			FlushBuffer(PrefixBuffer);
			LetterCounter = 0;
		}
		else if(KeyPressed == TabKey)
		{
			trie_node *PrefixNode = FindPrefixNode(Root, PrefixBuffer.Memory);

			if(PrefixNode)
			{
				strcpy(OutputBuffer.Memory, PrefixBuffer.Memory);

				trie_node *Temp = PrefixNode;

				//find the first 0 char of output buffer
				OutputBuffer.Position = NumberOfEnteredCharacters(OutputBuffer);

				while(Temp != NULL)
				{
					OutputBuffer.Memory[OutputBuffer.Position++] = Temp->Value;

					if(Temp->WordMarker == 1)
					{

						trie_node *Option = Temp;
						printf("\nPress 1. to select the word below\n1. %s\n", OutputBuffer.Memory);

						int Chosen = 0;
						while(!Chosen)
						{
							char Key = getch();

							if(Key != 0)
							{
								//accept word
								if(Key == '1')
								{
									strcpy(&InputBuffer.Memory[(InputBuffer.Position - LetterCounter)], OutputBuffer.Memory);
									InputBuffer.Position = NumberOfEnteredCharacters(InputBuffer);

									InputBuffer.Memory[InputBuffer.Position++] = SpaceKey;
									FlushBuffer(PrefixBuffer);
									LetterCounter = 0;
									Chosen = 1;
								}
								//reject word
								else
								{
									InputBuffer.Memory[InputBuffer.Position++] = Key;
									PrefixBuffer.Memory[LetterCounter++] = Key;
									Chosen = 1;
								}

								FlushBuffer(OutputBuffer);
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
			PrefixBuffer.Memory[LetterCounter++] = KeyPressed;
		}

		printf("\n%s", InputBuffer.Memory);


	}
}

int
main()
{
	void *FileMemory = LoadFile("data/words.txt");

	char *TextFile = (char *)FileMemory;

	trie_root_node* Root = CreateTrie(TextFile);


	PredictiveText(Root);


	FreeFile(FileMemory);

	return 0;
}
