#include "mySimpleComputer.h"

int commandsCPU[38] = {	
						16,  17,  32,  33,  48,  49,  50,  51, 
						64,  65,  66,  67,  81,  82,  83,  84,
						85,  86,  87,  88,  89,  96,  97,  98, 
						99,  100, 101, 102, 103, 104, 105, 112, 
						113, 114, 115, 116, 117, 118
					  };

int sc_memoryInit ()
{
	int i;
	
	for (i = 0; i < 100; i++)
	{
		RAM[i] = 0;
	}
	
	return 0;
}

int sc_memorySet (int address, int value)
{
	if (address >= 0 && address <= 99)
	{
		RAM[address] = value & (~(1 << 14)); 
	}
	else
	{
		flagsRegister = flagsRegister | (1 << (MEMORY_BORDER - 1));
	//	printf("Error: Wrong address!\n");
		return -1;
	}
	
	return 0;
}

int sc_memoryGet (int address, int* value)
{
	if (address >= 0 && address <= 99)
	{
		*value = RAM[address]; 
	}
	else
	{
		flagsRegister = flagsRegister | (1 << (MEMORY_BORDER - 1));
	//	printf("Error: Wrong address!\n");
		return -1;
	}
	
	return 0;
}

int sc_memorySave (char* filename)
{
	FILE* filePtr = NULL;
	
	filePtr = fopen(filename, "wb");
	
	if (filePtr == NULL)
	{
	//	printf("Error: Error opening file!\n");
		return -1;
	}
	
	fwrite(RAM, sizeof(int), 100, filePtr);
	
	fclose(filePtr);
	
	return 0;
}

int sc_memoryLoad (char* filename)
{
	FILE* filePtr = NULL;
	
	filePtr = fopen(filename, "rb");
	
	if (filePtr == NULL)
	{
	//	printf("Error: Error opening file!\n");
		return -1;
	}
	
	sc_regInit();
	sc_memoryInit();
	instructionCounter = 0;
	accumulator = 0;
	fread(RAM, sizeof(int), 100, filePtr);
	
	fclose(filePtr);
	
	return 0; 
}

int sc_regInit (void)
{
	flagsRegister = 0;
	
	return 0;
}

int sc_regSet (int regNumber, int value)
{
	if (regNumber > 0 && regNumber <= 5 && (value == 0 || value == 1)) 
	{
		if (value)
		{
			flagsRegister = flagsRegister | (1 << (regNumber - 1));
		}
		else 
		{
			flagsRegister = flagsRegister & (~(1 << (regNumber - 1)));
		}	
	}
	else 
	{
	//	printf("Error: Wrong register or value!\n");
		return -1;
	}
	
	return 0;
}

int sc_regGet (int regNumber, int* value)
{
	if (regNumber > 0 && regNumber <= 5) // from 1 to 5
	{
		*value = (flagsRegister >> (regNumber - 1)) & 0x1;
	}
	else 
	{
	//	printf("Error: Wrong register!\n");
		return -1;
	}
	
	return 0;
}

int sc_commandEncode (int command, int operand, int* value)
{ 
	int isCommand = 0;

	int i;
	for(i = 0; i < 38; i++)
	{
		if(command == commandsCPU[i])
		{
			isCommand = 1;
			break;
		}
	}

	if (isCommand == 1 && operand >= 0 && operand <= 127) 
	{
		int code = 0;
		code = operand;
		code += (command << 7);
		code = code & (~(1 << 14));
		
		*value = code; 
		
		return 0;
	}
	else 
	{
	//	printf("Error: Wrong command or operand!\n");
		return -1;
	}
	
	return 0;
}

int sc_commandDecode (int value, int* command, int* operand)
{
	int notCommand = (value >> 14) & 0x1; 
	
	if (!notCommand)
	{
		*command = (value >> 7);
		
		int i;
		int isCommand = 0;
		
		for (i = 0; i < 38; i++)
		{
			if(*command == commandsCPU[i])
			{
				isCommand = 1;
				break;
			}
		}
		
		if (isCommand == 0)
		{
		//	printf("Error: Wrong command!\n");
		//	flagsRegister = flagsRegister | (1 << (WRONG_COMMAND - 1));	
			return -1;
		}
		
		for (i = 0; i < 7; i++)
		{
			value = value & (~(1 << (i + 7))); 
		}
		
		*operand = value;
		
		return 0;
	}
	else 
	{
	//	printf("Error: Not a Simple Computer command!\n");
	//	flagsRegister = flagsRegister | (1 << (WRONG_COMMAND - 1));	
		return -1;
	}
}
