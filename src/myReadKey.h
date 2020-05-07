#ifndef MYREADKEY_H
#define MYREADKEY_H

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

struct termios termParams;

enum keys
{
    key_Number0 = 0,
    key_Number1 = 1,
    key_Number2 = 2,
    key_Number3 = 3,
    key_Number4 = 4,
    key_Number5 = 5,
    key_Number6 = 6,
    key_Number7 = 7,
    key_Number8 = 8,
    key_Number9 = 9,
    key_Up = 10,
    key_Down = 11,
    key_Left = 12,
    key_Right = 13,
    key_l = 14,
    key_s = 15,
    key_r = 16,
    key_tt = 17,
    key_i = 18,
    key_F5 = 19,
    key_F6 = 20,
    key_q = 21,
    key_Default = 22
};

int rk_readKey(enum keys* someKey);
int rk_myTermSave(void);
int rk_myTermRestore(void);
int rk_myTermRegime(int regime, int vTime, int vMin, int echo, int sigInt);

#endif
