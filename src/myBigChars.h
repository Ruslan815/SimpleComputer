#ifndef MYBIGCHARS_H
#define MYBIGCHARS_H

#include <stdlib.h>
#include <unistd.h>
#include "myTerm.h"

int bc_printA(char* str);
int bc_box(int x1, int y1, int rows, int columns);
int bc_printBigChar(int* arr, int x, int y, enum Colors fgColor, enum Colors bgColor);
int bc_setBigCharPos(int* big, int x, int y, int value);
int bc_getBigCharPos(int* big, int x, int y, int* value);
int bc_bigCharWrite(int fd, int* big, int count);
int bc_bigCharRead(int fd,int* big,int need_count,int* count);

#endif
