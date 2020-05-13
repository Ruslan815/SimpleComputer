#ifndef TRANSLATORASSEMBLY_H
#define TRANSLATORASSEMBLY_H

#include "mySimpleComputer.h"
#include "myReadKey.h"
#include <string.h>
#include <unistd.h>

int programInit();
int File_Read(char* inputFileName, char* outputFileName); 
int Get_Command(char* str);
int Read_String(char* str, int length);

short int programCode[100];

#endif