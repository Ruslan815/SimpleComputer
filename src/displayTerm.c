#include "displayTerm.h"

int bcint0 [2] = {1717992960, 8283750};
int bcint1 [2] = {471341056, 3938328};
int bcint2 [2] = {538983424, 3935292};
int bcint3 [2] = {2120252928, 8282238};
int bcint4 [2] = {2120640000, 6316158};
int bcint5 [2] = {2114092544, 8273984};
int bcint6 [2] = {33701376, 4071998};
int bcint7 [2] = {811630080, 396312};
int bcint8 [2] = {2120646144, 8283750};
int bcint9 [2] = {2087074816, 3956832};
int bcintA [2] = {2118269952, 4342338};
int bcintB [2] = {1044528640, 4080194};
int bcintC [2] = {37895168, 3949058};
int bcintD [2] = {1111637504, 4080194};
int bcintE [2] = {2114092544, 8258050};
int bcintF [2] = {33717760, 131646};
int bcintp [2] = {2115508224, 1579134};
int bcintm [2] = {2113929216, 126};

void runTerm(void)
{
	sc_regInit();
	instructionCounter = 0;
	accumulator = 0;

	enum keys pressedKey = key_Default;

	displayTerm();
	
	x = 2;
	y = 3;
	cursorAddress = 0;
	mt_gotoYX(x, y);

	rk_myTermRegime(0, 15, 0, 1, 1);

	signal (SIGUSR1, sigHandler);
	signal (SIGALRM, sigHandler);
	//struct itimerval beginVal, endVal;
	int timer = 0;

	while (pressedKey != key_q)
	{
		rk_readKey(&pressedKey);
				
		if (timer == 1 && (pressedKey < 10 || pressedKey > 13))
		{
			if (instructionCounter < 99)
			{
				instructionCounter++;
			}
		}

		char buffer[8] = "\0";
		int tempValue;	
		char tempNum[10] = "\0";
		int value = 0;		

		switch(pressedKey)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:

				tempNum[0] = pressedKey + 48;

				rk_myTermRegime(1, 0, 0, 0, 0);			

				read(0, &buffer, 3);

				strcat(tempNum, buffer);
				
				tempValue = atoi(tempNum);

				sc_memorySet(cursorAddress, tempValue);

				rk_myTermRegime(0, 15, 0, 1, 1);

				break;
			
			case 10:
				moveCursor(&x, &y, &cursorAddress, key_Up);
				break;

			case 11:
				moveCursor(&x, &y, &cursorAddress, key_Down);			
				break;

			case 12:
				moveCursor(&x, &y, &cursorAddress, key_Left);
				break;

			case 13:
				moveCursor(&x, &y, &cursorAddress, key_Right);
				break;

			case 14:
				sc_memoryLoad("SC_RAM.dat");
				break;

			case 15:
				sc_memorySave("SC_RAM.dat");
				break;

			case 16:
			/*	beginVal.it_value.tv_sec = 1;
				beginVal.it_value.tv_usec = 500;
				beginVal.it_interval.tv_sec = 1;
				beginVal.it_interval.tv_usec = 500;
				setitimer (ITIMER_REAL, &beginVal, &endVal);*/
				sc_regGet(4, &value);

				if (value == 0)
				{
					timer = 1;
				}
				break;

			case 17:
				if (instructionCounter < 99 && timer == 0)
				{
					instructionCounter++;
				}
				break;

			case 18:
			/*	beginVal.it_value.tv_sec = 0;
				beginVal.it_value.tv_usec = 0;
				beginVal.it_interval.tv_sec = 0;
				beginVal.it_interval.tv_usec = 0;
				setitimer (ITIMER_REAL, &beginVal, &endVal);*/
				timer = 0;
				raise(SIGUSR1);
				break;

			case 19:
				rk_myTermRegime(1, 0, 0, 0, 0);
				read(0, &buffer, 4);
				accumulator = atoi(buffer);
				rk_myTermRegime(0, 15, 0, 1, 1);
				break;

			case 20:
				rk_myTermRegime(1, 0, 0, 0, 0);
				read(0, &buffer, 2);
				instructionCounter = atoi(buffer);
				rk_myTermRegime(0, 15, 0, 1, 1);
				break;

			case 21:
				break;

			default:
				break;
		}

		mt_clearScreen();
		IOvar[0] = '\0';
		displayTerm();
	}
}

void sigHandler(int sigNum)
{
	int value = 0;
	sc_regGet(4, &value);

	switch(sigNum)
	{
		case SIGUSR1:

			if (value == 0)
			{
				sc_regInit();
				sc_memoryInit();
				instructionCounter = 0;
				accumulator = 0;
				sc_regSet(IGNORING_PULSES, 1); 
			}
			break;

		case SIGALRM:	
			
			if (value == 0 && instructionCounter < 99)
			{
				instructionCounter++;
			//	displayRegisters();	
			}
			break;
	}
}

void displayMemory(void)
{
	bc_box(1, 1, 12, 62);

	mt_gotoYX(1, 30);
	printf("Memory");

	int i, j;
	for (i = 0; i < 10; i++)
	{
		mt_gotoYX(2 + i, 2);

		for (j = 0; j < 10; j++)
		{
			int value = 0;
			sc_memoryGet(j + (i * 10), &value);

			if (j + (i * 10) == cursorAddress)
			{
				mt_setFgColor(GREEN);
				(value < 0) ? printf("-%4X ", -value) : printf("+%4X ", value); // (value > 16383)
				mt_setFgColor(WHITE);
			}
			else
			{
				(value < 0) ? printf("-%4X ", -value) : printf("+%4X ", value);
			}
			
		} 
	
	}
}

void displayRegisters(void)
{
	bc_box(1, 64, 3, 20);
	mt_gotoYX(1, 69);
	printf("Accumulator");
	mt_gotoYX(2, 71); 

	if (accumulator < 0)
	{
		printf("-%4X", -accumulator);
	}
	else
	{
		printf("+%4X", accumulator);
	}


	bc_box(4, 64, 3, 20);
	mt_gotoYX(4, 65);
	printf("InstructionCounter");
	mt_gotoYX(5, 71); 

    if (instructionCounter < 0 || instructionCounter > 99)
    {
        instructionCounter = 0;
    }

	printf("+%d", instructionCounter);

	bc_box(7, 64, 3, 20);
	mt_gotoYX(7, 70);
	printf("Operation");
	mt_gotoYX(8, 71); 

	int value = 0;
	sc_memoryGet(instructionCounter, &value);

	int command, operand;

	if (sc_commandDecode(value, &command, &operand) == 0) // if decoded
	{
		printf("+%X : %X", command, operand);
	}
	else
	{
		printf("+%d : %d", 0, 0);
	}

	bc_box(10, 64, 3, 20);
	mt_gotoYX(10, 72);
	printf("Flags");
	mt_gotoYX(11, 70); 

	int i;
	for (i = 1; i <= 5; i++)
	{
		int tempReg = 0;

	//	sc_regSet(i, i % 2);
		sc_regGet(i, &tempReg);

		if (tempReg)
		{
			switch(i)
			{
				case 1:
					printf("P ");
					break;

				case 2:
					printf("0 ");
					break;

				case 3:
					printf("M ");
					break;

				case 4: 
					printf("T ");
					break;

				case 5:
					printf("E ");
					break;

			}
		}
		else
		{
			printf("  ");
		}

	}
}

void displayKeys(void)
{
	bc_box(13, 45, 9, 39);
	mt_gotoYX(13, 47);

	printf("Keys:");
	mt_gotoYX(14, 46);
	printf("l  - load");
	mt_gotoYX(15, 46);
	printf("s  - save");
	mt_gotoYX(16, 46);
	printf("r  - run");
	mt_gotoYX(17, 46);
	printf("t  - step");
	mt_gotoYX(18, 46);
	printf("i  - reset");
	mt_gotoYX(19, 46);
	printf("F5 - accumulator");
	mt_gotoYX(20, 46);
	printf("F6 - instructionCounter");

	mt_gotoYX(14, 36);
}

void displayBigNumber(void)
{
	int tempMemoryNumber = 0; 

	int address = instructionCounter;
	sc_memoryGet(address, &tempMemoryNumber);

	if (tempMemoryNumber < 0)
	{
		bc_printBigChar(bcintm, 14, 2, GREEN, BLACK);
	}
	else
	{
		bc_printBigChar(bcintp, 14, 2, GREEN, BLACK);
	}
		
	int i; 
	for (i = 0; i < 4; i++)
	{
		int tempNumber = tempMemoryNumber % 16;
		tempMemoryNumber /= 16;

		switch(tempNumber)
		{
			case 0:
				bc_printBigChar(bcint0, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 1:
				bc_printBigChar(bcint1, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 2:
				bc_printBigChar(bcint2, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 3:
				bc_printBigChar(bcint3, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 4:
				bc_printBigChar(bcint4, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 5:
				bc_printBigChar(bcint5, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 6:
				bc_printBigChar(bcint6, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 7:
				bc_printBigChar(bcint7, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 8:
				bc_printBigChar(bcint8, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 9:
				bc_printBigChar(bcint9, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 10:
				bc_printBigChar(bcintA, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 11:
				bc_printBigChar(bcintB, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 12:
				bc_printBigChar(bcintC, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 13:
				bc_printBigChar(bcintD, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 14:
				bc_printBigChar(bcintE, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			case 15:
				bc_printBigChar(bcintF, 14, 36 - (i * 9), GREEN, BLACK);
				break;

			default:
				printf("Error: Error getting memory data!");
				return;
		}

	}

	bc_box(13, 1, 10, 44);
	mt_gotoYX(23, 1);
}

void displayTerm(void)
{
	int rows = 0;
	int columns = 0;
	struct winsize ws;

	if(!ioctl(1, TIOCGWINSZ, &ws))
	{
		rows = ws.ws_row;
		columns = ws.ws_col;
	} 
	else
	{
		printf ("Error: Error deviding size of the screen!\n");
		return;
	}

	if (rows < 22 || columns < 83)
	{
		printf("Error: Small Window Size, make bigger! (22x83) \n");
		return;
	}

	mt_clearScreen();
	displayMemory();
	displayRegisters();	
	displayKeys();
	displayBigNumber();
	displayIO();
}

void moveCursor(int* x, int* y, int *cursorAddress, int pressedKey)
{
	switch (pressedKey)
	{
		case 10:

			if (*y > 3)
			{
				*y -= 1;
				*cursorAddress -= 10;
				mt_gotoYX(*x, *y);
			}
			break;

		case 11:

			if (*y < 12)
			{
				*y += 1;
				*cursorAddress += 10;
				mt_gotoYX(*x, *y);
			}
			break;

		case 12:

			if (*x > 6)
			{
				*x -= 6;
				*cursorAddress -= 1;
				mt_gotoYX(*x, *y);
			}
			break;

		case 13:

			if (*x < 51)
			{
				*x += 6;
				*cursorAddress += 1;
				mt_gotoYX(*x, *y);
			}
			break;
	}
}

void displayIO()
{
	printf("Input/Output:\n");
	printf("%s\n",IOvar);
}