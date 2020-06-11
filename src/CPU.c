#include "CPU.h"

void sigHandler(int sigNum)
{
	int value = 0;
	sc_regGet(IGNORING_PULSES, &value);

	switch(sigNum)
	{
		case SIGUSR1:

			sc_regInit();
			sc_memoryInit();
			instructionCounter = 0;
			accumulator = 0;
			sc_regSet(IGNORING_PULSES, 1); 
			break;

		case SIGALRM:	
			
			if (value == 0 && instructionCounter < 99)
			{
				if (CU() == -1)
				{
					sc_regSet(IGNORING_PULSES, 1);
					beginVal.it_value.tv_sec = 0;
					beginVal.it_value.tv_usec = 0;
					beginVal.it_interval.tv_sec = 0;
					beginVal.it_interval.tv_usec = 0;
					setitimer (ITIMER_REAL, &beginVal, &endVal);
					timer = 0;
				}

				mt_clearScreen();
				displayTerm();
			}
			break;
	}
}

int CU()
{
	int memoryCell = 0;
	sc_memoryGet(instructionCounter, &memoryCell);

	int command = 0;
	int operand = 0;
	int number = 0;

	if (sc_commandDecode(memoryCell, &command, &operand) == -1) // if not SC command
	{
		sc_regSet(WRONG_COMMAND, 1);
		sc_regSet(IGNORING_PULSES, 1);

		return -1;
	}

	usleep(500000); // usecs
	if ((command >= 0x30 && command <= 0x33) || command == 0x69)
	{
		int statusALU = ALU(command, operand);

		if (statusALU == -1)
		{
			return -1;
		}
	}
	else
	{ 
		switch (command)
		{
			case 0x10:

				mt_gotoYX(26, 1);

				rk_myTermRegime(1, 0, 0, 0, 0);
				fflush(stdout);
				fflush(stdin);
				printf("Enter a number: ");			
				scanf("%d", &number);				
				fflush(stdout);
				rk_myTermRegime(0, 15, 0, 1, 1);

				if (sc_memorySet(operand, number) == -1)
				{
					return -1;
				}
				break;

			case 0x11:

				mt_gotoYX(26, 1);

				if (sc_memoryGet(operand, &number) == -1)
				{
					return -1;
				}

				int tempCommand = (number >> 7);
				int tempOperand = number & (~(255 << 7));  

				fflush(stdout);
				fflush(stdin);
				printf("A output number: %2X%2X", tempCommand, tempOperand);
				fflush(stdout);
				fflush(stdin);
				sleep(2);
				break;

			case 0x20:

				if (sc_memoryGet(operand, &number) == -1)
				{
					return -1;
				}

				accumulator = number;
				break;

			case 0x21:

				number = accumulator;

				if (sc_memorySet(operand, number) == -1)
				{
					return -1;
				}
				break;

			case 0x40:

				if (operand >= 0 && operand <= 99)
				{
					instructionCounter = operand - 1;
				}
				else
				{
					sc_regSet(MEMORY_BORDER, 1);
					return -1;
				}
				
				break;

			case 0x41:

				if (accumulator >= 0)
				{
					break;
				}

				if (operand >= 0 && operand <= 99)
				{
					instructionCounter = operand - 1;
				}
				else
				{
					sc_regSet(MEMORY_BORDER, 1);
					return -1;
				}
				break;

			case 0x42:

				if (accumulator)
				{
					break;
				}

				if (operand >= 0 && operand <= 99)
				{
					instructionCounter = operand - 1;
				}
				else
				{
					sc_regSet(MEMORY_BORDER, 1);
					return -1;
				}
				break;

			case 0x43:

				sc_regSet(IGNORING_PULSES, 1);
				timer = 0;
				beginVal.it_value.tv_sec = 0;
				beginVal.it_value.tv_usec = 0;
				beginVal.it_interval.tv_sec = 0;
				beginVal.it_interval.tv_usec = 0;
				setitimer (ITIMER_REAL, &beginVal, &endVal); 
				return -1;
				break;

		}
	}
	
	if (instructionCounter + 1 > 99)
	{
		sc_regSet(MEMORY_BORDER, 1);
		return -1;
	}

	instructionCounter++;

	return 0;
}

int ALU(int comand, int operand) 
{
	int value, k, tempBit, tempNumber;
	sc_memoryGet(operand, &value); // получаем значение из указанной ячейки памяти

	switch(comand)
	{
		case 0x30: 
		{
			if(accumulator + value > 0x7FFF) 
			{
				sc_regSet(OVERFLOW, 1);
				return -1;
			}

			accumulator += value;
			break;
		}
		case 0x31:
		{
			if(accumulator - value < -0x7FFE) 
			{
				sc_regSet(OVERFLOW, 1);
				return -1;
			}

			accumulator -= value;
			break;
		}
		case 0x32:
		{
			if(value == 0) 
			{
				sc_regSet(DIVISION_BY_ZERO, 1);
				return -1;
			}

			accumulator /= value;
			break;
		}
		case 0x33:
		{
			if(accumulator * value > 0x7FFF) 
			{
				sc_regSet(OVERFLOW, 1);
				return -1;
			}

			accumulator *= value;
			break;
		}
		case 0x69:
		{
			k = accumulator;
			tempNumber = value;

			while (k--)
			{
				tempBit = (tempNumber >> 14) & 0x1; // запоминаем 15 разряд числа

				tempNumber = tempNumber << 1; // сдвигаем число влево на 1 разряд
				tempNumber = tempNumber | tempBit; // ставим в 1 разряд ранее записанный бит
				tempNumber = tempNumber & 0x7FFF; // зануляем разряды старше 15
			}

			if (tempNumber > 0x7FFF)
			{
				sc_regSet(OVERFLOW, 1);
				return -1;
			}

			accumulator = tempNumber;
			break;
		}
	}

	return 0;
}
