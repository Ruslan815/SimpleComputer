#include "myTerm.h"

int mt_clearScreen(void)
{
	printf("\E[H\E[J \E[1;1H");

	return 0;
}

int mt_gotoYX(int X, int Y)
{
	if (X < 0 || Y < 0)
	{
	//	printf("Wrong value of coordinates!\n");
		return -1;
	}

	printf("\E[%d;%dH", X, Y);

	return 0;
}

int mt_getScreenSize(int* rows, int* cols)
{
	struct winsize ws;

	if(!ioctl(1, TIOCGWINSZ, &ws))
	{
	//	printf ("Received screen size\n");
	//	printf ("Number rows–%d\nNumber columns–%d\n",ws.ws_row, ws.ws_col);
		*rows = ws.ws_row;
		*cols = ws.ws_col;
	}
	else
	{
	//	printf("Error: Error getting screen size!\n");
		return -1;
	}

	return 0;
}

int mt_setFgColor(enum Colors fgColor)
{
	if (fgColor < 0 || fgColor > 9)
	{
	//	printf("Wrong value of foreground color!\n");
		return -1;
	}

	printf("\E[3%dm", fgColor);

	return 0;
}

int mt_setBgColor(enum Colors bgColor)
{
	if (bgColor < 0 || bgColor > 9)
	{
	//	printf("Wrong value of background color!\n");
		return -1;
	}	

	printf("\E[4%dm", bgColor);

	return 0;
}
