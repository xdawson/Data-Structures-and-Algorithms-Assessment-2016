#include <malloc.h>
#include <stdio.h>
//idk if this will be okay, i can write about it which is cool but theres no makrs for this 
#define _WIN32_WINNT 0x0501
#include <windows.h>

//returns a void * 
//maybe return a typed pointer once i figure out how this data is gonna be treated?
void *
LoadFile(char *FileName)
{
	HANDLE FileHandle = CreateFileA(FileName,
  			   						GENERIC_READ,
			   						FILE_SHARE_READ,
 			   						0,
        	   						OPEN_EXISTING,
     		   						0,
 			   						0);

	//Need to resolve all else cases or remove them
	if(FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;  

		if(GetFileSizeEx(FileHandle, &FileSize))
		{
			void *FileContents = malloc(FileSize.QuadPart);

			if(FileContents)
			{
				DWORD BytesRead;
				if(ReadFile(FileHandle, FileContents, FileSize.QuadPart, &BytesRead, 0))
				{
					printf("ReadFile success");
					return FileContents;
				}
				else
				{
					printf("ReadFile failure");
					return NULL;
				}					
			}
			else
			{
				printf("Failed to allocate memory for the file");
				return NULL;				
			}
		}
		else
		{
			printf("Failed to get file size");
			return NULL;			
		}
	}
	else
	{
		printf("Invalid file handle");
		return NULL;		
	}
}

//since this function is 1 line long does it really need to be its own function
//Does it improve readability??
void
FreeFile(void *FileMemory)
{
	free(FileMemory);
}

//do I even need WinMain here as opposed to main? Could remove WinMain to stop myself having 
//to inlude Windows specific code.
//Could alsmost certainly move this into its own .c file and have a header for the load function  
int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE PrevInstance,
		LPSTR     lpCmdLine,
		int       nCmdShow)
{
	void * FileMemory = LoadFile("data/words.txt");

	FreeFile(FileMemory);

	return 0;
}