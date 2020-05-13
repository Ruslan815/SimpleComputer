#include "translatorAssembly.h"

int main()
{
    char command[4], input[25], output[25];
    printf("Enter translation command: \n");
    fflush(stdin);
    fflush(stdout);
    rk_myTermRegime(1, 0, 0, 0, 0);
    scanf("%s %s %s", command, input, output);

    if (strcmp(command, "sat") != 0)
    {
        printf("\nWrong command!\n");
        sleep(3);
    }
    else
    {
        File_Read(input, output);
    }
    
    return 0;
}