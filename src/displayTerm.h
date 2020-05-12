#ifndef DISPLAYTERM_H
#define DISPLAYTERM_H

#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include "myBigChars.h"
#include "myReadKey.h"
#include "mySimpleComputer.h"

#include <string.h>

extern int bcint0[2];
extern int bcint1[2];
extern int bcint2[2];
extern int bcint3[2];
extern int bcint4[2];
extern int bcint5[2];
extern int bcint6[2];
extern int bcint7[2];
extern int bcint8[2];
extern int bcint9[2];
extern int bcintA[2];
extern int bcintB[2];
extern int bcintC[2];
extern int bcintD[2];
extern int bcintE[2];
extern int bcintF[2];
extern int bcintp[2];
extern int bcintm[2];

extern short int accumulator;
extern short int instructionCounter;
int x;
int y;
int cursorAddress;
// char IOvar[80];

void runTerm(void);
void sigHandler(int sigNum);
void displayMemory(void);
void displayRegisters(void);
void displayKeys(void);
void displayBigNumber(void);
void displayTerm(void);
void moveCursor(int *x, int *y, int *cursorAddress, int pressedKey);
void displayIO();
int CU();
int File_Read(char *name);
int Read_string(char *str, int length);
int Get_Command(char *str);
#endif
