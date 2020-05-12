#include "myReadKey.h"

int rk_readKey(enum keys* someKey)
{
    char temp[10];
    int count = 0;

    count = read(0, temp, 9); 

    if (count == -1)
	{
    //	printf("Error in RK: Error reading input stream!\n");
      	return -1;
    }

    if (strcmp(temp, "\E[A") == 0) 
    {
		*someKey = key_Up;
	} 
    else if (strcmp(temp, "\E[B") == 0) 
    {
		*someKey = key_Down;
	} 
    else if (strcmp(temp, "\E[C") == 0) 
    {
		*someKey = key_Right;
	} 
    else if (strcmp(temp, "\E[D") == 0) 
    {
		*someKey = key_Left;
	}
    else if (temp[0] >= '0' && temp[0] <= '9') 
    {
		*someKey = temp[0] - 48;
	}   
    else if (temp[0] == 'l') 
    {
		*someKey = key_l;
	} 
    else if (temp[0] == 's') 
    {
		*someKey = key_s;
	} 
    else if (temp[0] == 'r') 
    {
		*someKey = key_r;
	} 
    else if (temp[0] == 't') 
    {
		*someKey = key_tt;
	} 
    else if (temp[0] == 'i') 
    {
		*someKey = key_i;
	}
    else if (strcmp(temp, "\E[15~") == 0) 
    {
		*someKey = key_F5;
	} 
    else if (strcmp(temp, "\E[17~") == 0) 
    {
		*someKey = key_F6;
	}  
    else if (temp[0] == 'q') 
    {
		*someKey = key_q;
	}  
    else 
    {
		*someKey = key_Default;
	}

    return 0;    
}

int rk_myTermSave(void)
{
	if (tcgetattr(0, &termParams) == -1) 
  	{
    //	printf("Error in RK: Error getting of Term Attributes!\n");
		return -1;
	}
	
	return 0;
}

int rk_myTermRestore(void)
{
	if (tcsetattr(0, TCSAFLUSH, &termParams) == -1)
  	{
    //	printf("Error in RK: Error setting of Term Attributes!\n");
		return -1;
	}

	return 0;
}

int rk_myTermRegime(int regime, int vTime, int vMin, int echo, int sigInt) // 0 = NOT CANON, 1 = CANON
{
	struct termios newTermParams;

	rk_myTermSave();

	newTermParams = termParams;

	if (regime == 0) 
  	{
    	newTermParams.c_lflag &= (~ICANON);
    	newTermParams.c_cc[VMIN] = vMin;		
		newTermParams.c_cc[VTIME] = vTime;
    
    	if (echo == 0) 
        { 
			newTermParams.c_lflag &= (~ECHO);
		} 
        else if (echo == 1) 
        {
			newTermParams.c_lflag |= ECHO;
		}

		if (sigInt == 0) 
        {
			newTermParams.c_lflag &= (~ISIG);
		} 
        else if (sigInt == 1) 
        {
			newTermParams.c_lflag |= ISIG;
		}
	} 
	else if (regime == 1) 
	{
		newTermParams.c_lflag |= ICANON;
	}

  	termParams = newTermParams;

  	rk_myTermRestore();

	return 0;
}

