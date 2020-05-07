#include "myBigChars.h"

int bc_printA(char* str)
{
	printf("\E(0%s\E(B", str);

	return 0;
}

int bc_box(int x, int y, int rows, int columns)
{
	if (x < 0 || y < 0 || rows < 0 || columns < 0)
	{
		printf("Error: Wrong coordinates or size!");
		return -1;
	}

	mt_gotoXY(x, y);
	printf("\E(0l");

	int i;
	for (i = 0; i < columns - 2; i++)
	{
		printf("q");
	}
	printf("k");

	for (i = 1; i <= rows - 2; i++)
	{
		printf("\E[%d;%dHx", x + i, y);
		printf("\E[%d;%dHx", x + i, y + columns - 1);
	}

	printf("\E[%d;%dHm", x + rows - 1, y);
	for (i = 0; i < columns - 2; i++)
	{
		printf("q");
	}
	printf("j");

	printf("\E(B\n");

	return 0;
}

int bc_printBigChar(int* arr, int x, int y, enum Colors fgColor, enum Colors bgColor)
{
	if (x < 0 || y < 0 || fgColor < 0 || fgColor > 9 || bgColor < 0 || bgColor > 9)
	{
		printf("Error: Wrong coordinates or color!");
		return -1;
	}

	mt_setFgColor(fgColor);
	mt_setBgColor(bgColor);
	printf("\E(0");

	int i;
	for (i = 0; i < 4; i++)
	{	
		mt_gotoXY(x + i, y);

		int j;
		for (j = 0; j < 8; j++)
		{
			int temp = (arr[0] >> (j + (i * 8))) & 0x1;
			
			if (temp)
			{
				printf("a");
			}
			else
			{
				printf(" ");
			}
		}	
	}

	int coorX = 4;
	for (i = 0; i < 4; i++)
	{	
		mt_gotoXY(x + coorX++, y);

		int j;
		for (j = 0; j < 8; j++)
		{
			int temp = (arr[1] >> (j + (i * 8))) & 0x1;
			
			if (temp)
			{
				printf("a");
			}
			else
			{
				printf(" ");
			}
		}	
	}

	mt_setFgColor(WHITE);
	mt_setBgColor(BLACK);
	printf("\E(B");

	return 0;
}

// is this part used?
/*
int bc_setBigCharPos(int* big, int x, int y, int value)
{
	if (x < 0 || x > 7 || y < 0 || y > 7 || value < 0 || value > 1)
	{
		printf("Error: Wrong coordinates or value!");
		return -1;
	}

	int index = 0;

	if (x > 3)
	{
		index = 1;
	}

	int position = (x * 8) + y;
	if (value)
	{
		big[index] = (big[index] | (1 << (position - 1)));
	}
	else
	{
		big[index] = (big[index] & (~(1 << (position - 1))));
	}

	return 0;
}

int bc_getBigCharPos(int* big, int x, int y, int* value)
{
	if (x < 0 || x > 7 || y < 0 || y > 7)
	{
		printf("Error: Wrong coordinates!");
		return -1;
	}

	int index = 0;

	if (x > 3)
	{
		index = 1;
	}

	int position = (x * 8) + y;

	*value = (big[index] >> (position - 1)) & 0x1;	

	return 0;
}

int bc_bigCharWrite(int fd, int* big, int count)
{
	if (count < 1 || fd == -1)
	{
		printf("Error: Wrong File Descriptor or Count!");
		return -1;
	}

	// int temp = write(fd, big, sizeof(int) * 2 * count);
	while (count > 0)
	{
		int temp = write(fd, big, sizeof(int) * 2);

		if (temp == -1)
		{
			return -1;
		}

		count--;
	}

	return 0;
}

int bc_bigCharRead(int fd,int* big,int need_count,int* count)
{
	if (need_count < 1 || fd == -1)
	{
		printf("Error: Wrong File Descriptor or Count!");
		return -1;
	}

	*count = 0;

	while (need_count > 0)
	{
		int temp = read(fd, big, sizeof(int) * 2);

		if (temp == -1)
		{
			return -1;
		}

		*count = *count + 1;
		need_count--;
	}

	return 0;
}
*/