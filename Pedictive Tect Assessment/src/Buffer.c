#include "buffer.h"
#include <malloc.h>
buffer
BufferConstructor(int Size)
{
	buffer Buffer;

	Buffer.Memory = malloc(sizeof(char) * Size);
	Buffer.Size = Size;
	Buffer.Position = 0;

	return Buffer;
}

void
BufferDestructor(buffer Buffer)
{
	free(Buffer.Memory);
	free(&Buffer);
}

void
FlushBuffer(buffer Buffer)
{
	int i;
	for(i = 0; i < Buffer.Size; i++)
	{
		Buffer.Memory[i] = 0;
	}
}

int 
NumberOfEnteredCharacters(buffer Buffer)
{
	int Position = 0;
	while(Buffer.Memory[Position] != 0)
	{
		Position++;
	}
	return Position;
}