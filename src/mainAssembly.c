#include "mySimpleComputer.h"
#include "myReadKey.h"
#include <string.h>
#include <unistd.h>
#include <math.h>

int programInit();
int File_Read(char* inputFileName, char* outputFileName); 
int Get_Command(char* str);
int Read_String(char* str, int length);

short int programCode[100];

int programInit ()
{
	int i;
	
	for (i = 0; i < 100; i++)
	{
		programCode[i] = 0;
	}
	
	return 0;
}

int File_Read(char* inputFileName, char* outputFileName)
{
    FILE* input = fopen(inputFileName, "r");

    if(!input)
	{
        printf("INVALID FILE NAME !!!");
        return -1;
    }

	size_t length;
    char* buffer = '\0';
    programInit();

    while (getline(&buffer, &length, input) != -1)
	{
        Read_String(buffer, length); 
    } 

    fclose(input);

    FILE* outputFilePtr = fopen(outputFileName, "wb");
    fwrite(programCode, sizeof(int), 100, outputFilePtr);
    fclose(outputFilePtr);

	return 0;
}

int Get_Command(char *str)
{
    if (strcmp("READ",str) == 0)
	{
        return 0x10;
    }
	else if(strcmp("WRITE",str) == 0)
	{
        return 0x11;
    }
	else if(strcmp("LOAD",str) == 0)
	{
        return 0x20;
    }
	else if(strcmp("STORE",str) == 0)
	{
        return 0x21;
    }
	else if(strcmp("ADD",str) == 0)
	{
		return 0x30;
    }
	else if(strcmp("SUB",str) == 0)
	{
    	return 0x31;
    }
	else if(strcmp("DIVIDE",str) == 0)
	{
    	return 0x32;
    }
	else if(strcmp("MUL",str) == 0)
	{
        return 0x33;
    }
	else if(strcmp("JUMP",str) == 0)
	{
        return 0x40;
    }
	else if(strcmp("JNEG",str) == 0)
	{
        return 0x41;
    }
	else if(strcmp("JZ",str) == 0)
	{
        return 0x42;
    }
	else if(strcmp("HALT",str) == 0)
	{
        return 0x43;
    }
	else if(strcmp("JNS",str) == 0)
	{
        return 0x55;
    }
    else if(strcmp("ADDC",str) == 0)
	{
        return 0x65;
    }
    else if(strcmp("RCCL",str) == 0)
	{
        return 0x69;
    }
	else if(strcmp("=",str) == 0)
	{
        return 1;
    }
	else 
	{
        return -1;
    }

	return 0;
}

int Read_String(char *str, int length)
{
    int operand = 0;
    char operationString[7];
    int address = (str[0] - '0') * 10 + (str[1] - '0');
    int pos = 3;

    while (str[pos] != ' ')
	{
        operationString[pos - 3] = str[pos];
        pos++;
    }
    operationString[pos - 3] = '\0';

    int command = Get_Command(operationString);

    if (command == -1)
	{
        programCode[address] = 0;
        return 0;
    }

    while(str[pos] == ' ')
	{
        pos++;
    }

    int j;
    if (command == 1)
    {
        pos++;
        command = 0;

        for (j = 1; j >= 0; j--, pos++)
        {
            if (str[pos] >= 'A' && str[pos] <= 'F')
            {
                int symbolNumber = str[pos] - 'A' + 10;
                command += symbolNumber * pow(16, j);
            }
            else
            {
                int symbolNumber = str[pos] - '0';
                command += symbolNumber * pow(16, j);
            }
        }

        for (j = 1; j >= 0; j--, pos++)
        {
            if (str[pos] >= 'A' && str[pos] <= 'F')
            {
                int symbolNumber = str[pos] - 'A' + 10;
                operand += symbolNumber * pow(16, j);
            }
            else
            {
                int symbolNumber = str[pos] - '0';
                operand += symbolNumber * pow(16, j);
            }
        }

        programCode[address] = command;
        programCode[address] <<= 7;
        programCode[address] |= operand;
        printf("%d :", programCode[address]);
    } 
    else
    {
        for (j = pos; (str[j] >= '0' && str[j] <= '9'); j++)
        {
            operand *= 10;
            operand += (str[j] - '0');
        }

        int operation = 0;
        sc_commandEncode(command, operand, &operation);
        programCode[address] = operation;
    } 

	return 0;
}

int main()
{
    char command[4], input[25], output[25];
    printf("Enter the command to SimpleAssembly translator(sat *.sa *.o): \n");
    fflush(stdin);
    fflush(stdout);
    rk_myTermRegime(1, 0, 0, 0, 0);
    scanf("%s %s %s", command, input, output);

    if (strcmp(command, "sat") != 0)
    {
        printf("\nWrong command!\n");
        sleep(3);
    }
    else
    {
        File_Read(input, output);
    }
    
    return 0;
}
