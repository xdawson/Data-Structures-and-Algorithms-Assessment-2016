typedef struct buffer
{
	int Size;
	int Position;
	char *Memory;
}buffer;

buffer
BufferConstructor(int Size);

void
BufferDestructor(buffer Buffer);

void
FlushBuffer(buffer Buffer);

int
NumberOfEnteredCharacters(buffer Buffer);