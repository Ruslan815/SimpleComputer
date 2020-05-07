#ifndef MYTERM_H
#define MYTERM_H

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h> 

enum Colors
{	
	SPACEGRAY = 0,	
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	PURPLE = 5,
	AQUA = 6,
	WHITE = 7,
	GRAY = 8,
	BLACK = 9
};

int mt_clearScreen(void);
int mt_gotoXY(int X, int Y);
int mt_getScreenSize(int* rows, int* cols);
int mt_setFgColor(enum Colors fgColor);
int mt_setBgColor(enum Colors bgColor);

#endif
