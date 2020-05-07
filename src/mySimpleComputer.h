#ifndef MYSIMPLECOMPUTER_H
#define MYSIMPLECOMPUTER_H

#include <stdlib.h>
#include <stdio.h>

#define OVERFLOW 1
#define DIVISION_BY_ZERO 2
#define MEMORY_BORDER 3
#define IGNORING_PULSES 4
#define WRONG_COMMAND 5

short int RAM[100];
short int accumulator;
short int instructionCounter;
char flagsRegister;

int sc_memoryInit (void);
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int* value);
int sc_memorySave (char* filename);
int sc_memoryLoad (char* filename);
int sc_regInit (void);
int sc_regSet (int regNumber, int value);
int sc_regGet (int regNumber, int* value);
int sc_commandEncode (int command, int operand, int* value);
int sc_commandDecode (int value, int* command, int* operand);

#endif
