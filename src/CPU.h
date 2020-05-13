#include "mySimpleComputer.h"
#include "myTerm.h"
#include "myReadKey.h" 
#include "displayTerm.h"

#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

extern short int accumulator;
extern short int instructionCounter;
int timer; 
struct itimerval beginVal, endVal;

void sigHandler(int sigNum);
int CU();
int ALU(int comand, int operand); 