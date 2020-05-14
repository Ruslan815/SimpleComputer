#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Variable
{
    char name;
    int address;
    int value;
};

struct Variable arr[27];

void initVarArr()
{
    int i;

    for (i = 0; i < 27; i++)
    {
        arr[i].name = '0';
        arr[i].address = 0;
        arr[i].value = 0;
    }
    
    arr[0].name = 'A';
    arr[0].address = 53;
    arr[0].value = 15;

    arr[0].name = 'B';
    arr[0].address = 54;
    arr[0].value = 16;

    arr[0].name = 'C';
    arr[0].address = 55;
    arr[0].value = 17;
/*
    for (i = 0; i < 27; i++)
    {
        printf("Variable: %c Address: %d Value: %d\n", arr[i].name, arr[i].address, arr[i].value);
    }
*/
}

char* let(char* str, int length, struct Variable* arr)
{
    int pos = 7;

    while (str[pos] == ' ')
    {
        pos++;
    }

    int expLen = 0;
    char* expression = "\0";

    int i;
    for (i = 0; pos < length; pos++, i++)
    {
        expression[i] = str[pos];
    }

    expLen = i + 1;
    expression[i] = '\0';

    

    return "0";
}

int main()
{
    initVarArr();

    


    return 0;
}