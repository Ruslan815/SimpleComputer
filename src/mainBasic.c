#include "myReadKey.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Variable
{
    char name;
    int address;
    int value;
};

struct Stack
{
    char sym;
    struct Stack *next;
};

//  				 !  +  -  *  /  (  )
int arrRPN[6][7] = {{4, 1, 1, 1, 1, 1, 5},  // !
                    {2, 2, 2, 1, 1, 1, 2},  // +
                    {2, 2, 2, 1, 1, 1, 2},  // -
                    {2, 2, 2, 2, 2, 1, 2},  // *
                    {2, 2, 2, 2, 2, 1, 2},  // /
                    {5, 1, 1, 1, 1, 1, 3}}; // (

struct Variable var_arr[27];

FILE *input = NULL;
FILE *output = NULL;

const char command_arr[][7] = {"REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END"};

int count_asm = 0;
int count_bas = 1;
int var_count = 0;
int var_adr = 99;
char asmCommand[240];
int bas_to_asm[10];

char *command_decode(char *input_str, char *command);

void pushStack(char sym, struct Stack **top)
{
    struct Stack *newElem = malloc(sizeof(struct Stack));
    newElem->sym = sym;
    newElem->next = *top;
    *top = newElem;
}

char popStack(struct Stack **top)
{
    struct Stack *ptr = *top;

    if (*top == NULL)
    {
        printf("Stack is Empty!");
        return '!';
    }

    *top = (*top)->next;
    char tmp = ptr->sym;
    free(ptr);

    return tmp;
}

void init_var_arr()
{
    for (int i = 0; i < 27; i++)
    {
        var_arr[i].name = '\0';
        var_arr[i].address = 0;
        var_arr[i].value = 0;
    }
}

void add_var(char var_tmp, int value)
{
    for (int i = 0; i < 27; i++)
    {
        if (var_arr[i].name == '\0')
        {
            var_arr[i].name = var_tmp;
            var_arr[i].address = var_adr;
            var_adr--;
            var_arr[i].value = value;
            break;
        }
    }
}

int get_var(char var_tmp)
{
    for (int i = 0; i < 27; i++)
    {
        if (var_arr[i].name == var_tmp)
        {
            return i;
        }
    }
    return -1;
}

char generateVar()
{
    for (int i = 0; i < 27; i++)
    {
        char tmp = 'Z' - i;
        if (get_var(tmp) == -1)
        {
            add_var(tmp, 0);
            return tmp;
        }
    }
    return '\0';
}

char *add_command(const char *command, int operand)
{
    char *result = malloc(sizeof(char) * 12);
    int i = 0;

    if (count_asm < 10)
    {
        result[i] = '0';
        result[i + 1] = count_asm + '0';
        count_asm++;
    }
    else
    {
        result[i] = count_asm / 10 + '0';
        result[i + 1] = count_asm % 10 + '0';
        count_asm++;
    }

    i += 2;
    strcat(result, " ");
    i++;
    strcat(result, command);
    i += strlen(command);
    strcat(result, " ");
    i++;

    if (operand < 10)
    {
        strcat(result, "0");
        char param[2];
        param[0] = operand + '0';
        param[1] = '\0';
        strcat(result, param);
    }
    else
    {
        char param[2];
        param[0] = operand / 10 + '0';
        param[1] = '\0';
        strcat(result, param);
        param[0] = operand % 10 + '0';
        strcat(result, param);
    }

    i += 2;
    strcat(result, "\n\0");
    
    return result;
}

char *rem(char *input_str)
{
    char *command = (char *)calloc(100, sizeof(char));
    int i = 7;

    command[0] = ';';

    while (input_str[i] != '\0')
    {
        command[i - 6] = input_str[i];
        i++;
    }

    command[i] = '\n';
    return command;
}

char *input_func(char *input_str)
{
    char *command = malloc(sizeof(char) * 12);

    if (count_asm < 10)
    {
        command[0] = '0';
        command[1] = count_asm + '0';
        count_asm++;
    }
    else
    {
        command[0] = count_asm / 10 + '0';
        command[1] = count_asm % 10 + '0';
        count_asm++;
    }

    command[2] = ' ';
    command[3] = 'R';
    command[4] = 'E';
    command[5] = 'A';
    command[6] = 'D';
    char value = input_str[9];
    int i = get_var(value);
    command[7] = ' ';

    if (i == -1)
    {
        add_var(value, 0);
        i = get_var(value);
    }

    if (var_arr[i].address < 10)
    {
        command[8] = '0';
        command[9] = var_arr[i].address + '0';
    }
    else
    {
        command[8] = var_arr[i].address / 10 + '0';
        command[9] = var_arr[i].address % 10 + '0';
    }

    command[10] = '\n';
    command[11] = '\0';

    return command;
}

char *print_func(char *input_str)
{
    char *command = (char *)calloc(12, sizeof(char));

    if (count_asm < 10)
    {
        command[0] = '0';
        command[1] = count_asm + '0';
        count_asm++;
    }
    else
    {
        command[0] = count_asm / 10 + '0';
        command[1] = count_asm % 10 + '0';
        count_asm++;
    }

    command[2] = ' ';
    command[3] = 'W';
    command[4] = 'R';
    command[5] = 'I';
    command[6] = 'T';
    command[7] = 'E';
    char value = input_str[9];
    int i = get_var(value);
    command[8] = ' ';

    if (i == -1)
    {
        add_var(value, 0); 
        i = get_var(value); 
    }

    if (var_arr[i].address < 10)
    {
        command[9] = '0';
        command[10] = var_arr[i].address + '0';
    }
    else
    {
        command[9] = var_arr[i].address / 10 + '0';
        command[10] = var_arr[i].address % 10 + '0';
    }

    command[11] = '\n';
    return command;
}

char *goto_func(char *input_str)
{
    char *command = (char *)calloc(12, sizeof(char));
    int tmp;

    tmp = (input_str[8] - '0') * 10 + (input_str[9] - '0');

    command = add_command("JUMP", tmp);

    return command;
}

char *if_func(char *input_str)
{
    char *command = malloc(sizeof(char) * 100);
    char var_a;
    char var_b;
    int i = 6;

    if (input_str[6] < 'A')
    {
        char tmp_value[10];

        int tmp_char;
        for ( ; input_str[i] != ' '; i++)
        {
            char tmp[] = {input_str[i], '\0'};
            strcat(tmp_value, tmp);
        }

        tmp_char = atoi(tmp_value);
        var_a = generateVar();
        int tempVarIndex = get_var(var_a);
        var_arr[tempVarIndex].value = tmp_char;
        i++;
    }
    else
    {
        int tmp = get_var(input_str[6]);

        if (tmp == -1)
        {
            add_var(input_str[6], 0);
        }

        var_a = input_str[6];
        i += 2;
    }

    char sign = input_str[i];
    
    i += 2;

    if (input_str[i] < 'A')
    {
        char tmp_value[10];

        int tmp_char;
        for (; input_str[i] != ' '; i++)
        {
            char tmp[] = {input_str[i], '\0'};
            strcat(tmp_value, tmp);
        }

        tmp_char = atoi(tmp_value);
        var_b = generateVar();
        int tempVarIndex = get_var(var_b);
        var_arr[tempVarIndex].value = tmp_char;
        i++;
    }
    else
    {
        int tmp = get_var(input_str[i]);

        if (tmp == -1)
        {
            add_var(input_str[i], 0);
        }

        var_b = input_str[i];
        i += 2;
    }

    char *result;

    if (sign == '>')
    {
        int pos = get_var(var_a);
        result = add_command("LOAD", var_arr[pos].address);
        strcat(command, result);

        pos = get_var(var_b);
        result = add_command("SUB", var_arr[pos].address);
        strcat(command, result);

        result = add_command("JNEG", (count_bas + 1) * 10); //выполнится, если условие ложное (отрицательное число)
        strcat(command, result);

        result = add_command("JZ", (count_bas + 1) * 10); //выполнится, если условие ложное (ноль)
        strcat(command, result);
    }
    else if (sign == '<')
    {
        int pos = get_var(var_b);
        result = add_command("LOAD", var_arr[pos].address);
        strcat(command, result);

        pos = get_var(var_a);
        result = add_command("SUB", var_arr[pos].address);
        strcat(command, result);

        result = add_command("JNEG", (count_bas + 1) * 10); //выполнится, если условие ложное (отрицательное число)
        strcat(command, result);

        result = add_command("JZ", (count_bas + 1) * 10); //выполнится, если условие ложное (ноль)
        strcat(command, result);
    }
    else if (sign == '=')
    {
        int pos = get_var(var_b);
        result = add_command("LOAD", var_arr[pos].address);
        strcat(command, result);

        pos = get_var(var_a);
        result = add_command("SUB", var_arr[pos].address);
        strcat(command, result);

        result = add_command("JJ", count_asm + 2); // если 0, то истина
        strcat(command, result);
        result = add_command("JUMP", (count_bas + 1) * 10);
        strcat(command, result);
    }

    int j = 3;
    char input_str_second[50] = {'\0'};
    char command_second[7] = {'\0'};
    input_str_second[0] = input_str[0];
    input_str_second[1] = input_str[1];
    input_str_second[2] = ' ';

    for (; input_str[i] != '\n'; i++, j++)
    {
        input_str_second[j] = input_str[i];
    }

    for (i = 3; input_str_second[i] != ' '; i++)
    {
        command_second[i - 3] = input_str_second[i];
    }

    result = command_decode(input_str_second, command_second);

    strcat(command, result);

    return command;
}

void parseLetString(char *str, int length)
{
    char resultString[80];

    int pos = 7;

    int j = pos;
    while (str[j] == ' ')
    {
        j++;
    }

    char answerSymbol = str[j];
    if (get_var(answerSymbol) == -1)
    {
        add_var(answerSymbol, 0);
    }

    int i = 0;
    for (; pos <= length; pos++)
    {
        if (str[pos] != ' ' && str[pos] != '\n')
        {
            if (str[pos] >= '0' && str[pos] <= '9')
            {
                int varValue = 0;

                while (str[pos] >= '0' && str[pos] <= '9' && pos <= length)
                {
                    int number = str[pos] - '0';

                    varValue *= 10;
                    varValue += number;
                    pos++;
                }
                pos--;

                char newNumberVar = generateVar();
                int newVarIndex = get_var(newNumberVar);
                var_arr[newVarIndex].value = varValue;

                resultString[i] = newNumberVar;
            }
            else
            {
                resultString[i] = str[pos];
            }

            i++;
        }
    }

    resultString[i] = '\0';
    strcpy(str, resultString);
}

void toReversePolishNotation(char *str, int length)
{
    str[length] = '!';
    str[length + 1] = '\0';

    struct Stack *top = malloc(sizeof(struct Stack));
    top->sym = '!';
    top->next = NULL;

    char result[80];
    int pos = 0;

    int i = 0;
    while (i <= length)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            result[pos] = str[i];
            i++;
            pos++;
        }
        else
        {
            char stackTop = top->sym;
            char currentSym = str[i];

            int stackSymNum;
            int currSymNum;

            switch (stackTop)
            {
                case '!':
                    stackSymNum = 0;
                    break;

                case '+':
                    stackSymNum = 1;
                    break;

                case '-':
                    stackSymNum = 2;
                    break;

                case '*':
                    stackSymNum = 3;
                    break;

                case '/':
                    stackSymNum = 4;
                    break;

                case '(':
                    stackSymNum = 5;
                    break;
                }

                switch (currentSym)
                {
                case '!':
                    currSymNum = 0;
                    break;

                case '+':
                    currSymNum = 1;
                    break;

                case '-':
                    currSymNum = 2;
                    break;

                case '*':
                    currSymNum = 3;
                    break;

                case '/':
                    currSymNum = 4;
                    break;

                case '(':
                    currSymNum = 5;
                    break;

                case ')':
                    currSymNum = 6;
                    break;
            }

            int operationCode = arrRPN[stackSymNum][currSymNum];

            switch (operationCode)
            {
                case 1:
                    pushStack(currentSym, &top);
                    i++;
                    break;

                case 2:
                    currentSym = popStack(&top);
                    result[pos] = currentSym;
                    pos++;
                    break;

                case 3:
                    i++;
                    popStack(&top);
                    break;

                case 4:
                    result[pos] = '\0';
                    i++;
                    break;

                case 5:
                    printf("Incorrectly balanced formula!");
                    strcpy(str, "?????\0");
                    break;
            }
        }
    }

    strcpy(str, result);
    free(top);
}

void addAsmCommand(char *command, int operand)
{
    char stringNumber[3];
    char operandNumber[3];

    if (operand < 10)
    {
        operandNumber[0] = '0';
        operandNumber[1] = operand + '0';
    }
    else
    {
        operandNumber[0] = operand / 10 + '0';
        operandNumber[1] = operand % 10 + '0';
    }

    if (count_asm < 10)
    {
        stringNumber[0] = '0';
        stringNumber[1] = count_asm + '0';
    }
    else
    {
        stringNumber[0] = count_asm / 10 + '0';
        stringNumber[1] = count_asm % 10 + '0';
    }

    stringNumber[2] = '\0';
    operandNumber[2] = '\0';

    strcat(asmCommand, stringNumber);
    strcat(asmCommand, " ");
    strcat(asmCommand, command);
    strcat(asmCommand, " ");
    strcat(asmCommand, operandNumber);
    strcat(asmCommand, "\n");
    count_asm++;
}

void calculate(char *expressionString, int length, char answerVariable)
{
    struct Stack *top = malloc(sizeof(struct Stack));
    top->sym = '!';
    top->next = NULL;

    int var_index = 0;
    int i = 0;

    while (i < length)
    {
        if (expressionString[i] >= 'A' && expressionString[i] <= 'Z')
        {
            pushStack(expressionString[i], &top);
        }
        else
        {
            char rightOperand = popStack(&top); // memory cell
            char leftOperand = popStack(&top);  // accumulator

            int rightVarIndex = get_var(rightOperand);
            if (rightVarIndex == -1)
            {
                add_var(rightOperand, 0);
                rightVarIndex = get_var(rightOperand);
            }

            int leftVarIndex = get_var(leftOperand);
            if (leftVarIndex == -1)
            {
                add_var(leftOperand, 0);
                leftVarIndex = get_var(leftOperand);
            }

            char tmp[10] = {"LOAD"};
            addAsmCommand(tmp, var_arr[leftVarIndex].address);

            char newVar = generateVar();
            var_index = get_var(newVar);

            switch (expressionString[i])
            {
                case '+':
                    strcpy(tmp, "ADD");
                    addAsmCommand(tmp, var_arr[rightVarIndex].address);
                    var_arr[var_index].value = var_arr[leftVarIndex].value + var_arr[rightVarIndex].value;
                    break;

                case '-':
                    strcpy(tmp, "SUB");
                    addAsmCommand(tmp, var_arr[rightVarIndex].address);
                    var_arr[var_index].value = var_arr[leftVarIndex].value - var_arr[rightVarIndex].value;
                    break;

                case '*':
                    strcpy(tmp, "MUL");
                    addAsmCommand(tmp, var_arr[rightVarIndex].address);
                    var_arr[var_index].value = var_arr[leftVarIndex].value * var_arr[rightVarIndex].value;
                    break;

                case '/':
                    strcpy(tmp, "DIVIDE");
                    addAsmCommand(tmp, var_arr[rightVarIndex].address);
                    var_arr[var_index].value = var_arr[leftVarIndex].value / var_arr[rightVarIndex].value;
                    break;
            }

            strcpy(tmp, "STORE");
            addAsmCommand(tmp, var_arr[var_index].address);
            pushStack(newVar, &top);
        }

        i++;
    }

    char tmp[10] = {"LOAD"};
    int vaultOperandAddress = var_arr[var_index].address;
    addAsmCommand(tmp, vaultOperandAddress);

    strcpy(tmp, "STORE");
    vaultOperandAddress = get_var(answerVariable);
    vaultOperandAddress = var_arr[vaultOperandAddress].address;
    addAsmCommand(tmp, vaultOperandAddress);
}

void let(char *str, int length)
{
    parseLetString(str, length);
    length = strlen(str);

    int i;
    char answerVariable = str[0];

    char expressionString[80];

    for (i = 0; i < length - 2; i++)
    {
        expressionString[i] = str[i + 2];
    }
    expressionString[i] = '\0';

    length -= 2;

    toReversePolishNotation(expressionString, length);
    length = strlen(expressionString);

    if (length != 1)
    {
        calculate(expressionString, length, answerVariable);
    }
    else
    {
        int pos = get_var(expressionString[0]);
        pos = var_arr[pos].address;
        char var[] = {"LOAD"};
        addAsmCommand(var, pos);

        char var1[] = {"STORE"};
        pos = get_var(answerVariable);
        pos = var_arr[pos].address;
        addAsmCommand(var1, pos);
    }
}

char *end(char *input_str)
{
    char *command = malloc(sizeof(char) * 11);

    if (count_asm < 10)
    {
        command[0] = '0';
        command[1] = count_asm + '0';
        count_asm++;
    }
    else
    {
        command[0] = count_asm / 10 + '0';
        command[1] = count_asm % 10 + '0';
        count_asm++;
    }

    command[2] = '\0';
    strcat(command, " HALT 00\n");

    return command;
}

void movingFix(char *file_name)
{
    FILE *ptr = fopen(file_name, "w");
    char *input_str;
    rewind(output);
    size_t tmp_length;
    int tmp_bsk;

    while (getline(&input_str, &tmp_length, output) != -1)
    {
        if (input_str[3] == 'J' && (input_str[4] == 'U' || input_str[4] == 'N'))
        {
            tmp_bsk = (input_str[8] - '0') * 10 + (input_str[9] - '0');
            tmp_bsk = bas_to_asm[tmp_bsk / 10 - 1];
            input_str[8] = tmp_bsk / 10 + '0';
            input_str[9] = tmp_bsk % 10 + '0';
        }
        else if (input_str[3] == 'J' && input_str[4] == 'Z')
        {
            tmp_bsk = (input_str[6] - '0') * 10 + (input_str[7] - '0');
            tmp_bsk = bas_to_asm[tmp_bsk / 10 - 1];
            input_str[6] = tmp_bsk / 10 + '0';
            input_str[7] = tmp_bsk % 10 + '0';
        }
        else if (input_str[3] == 'J' && input_str[4] == 'J')
        {
            input_str[4] = 'Z';
        }
 
        fwrite(input_str, sizeof(char), strlen(input_str), ptr);
    }

    char *var_string = malloc(sizeof(char) * 11);
    int address;
    for (int i = 26; i >= 0; i--)
    {
        if (var_arr[i].name != '\0')
        {
            int tempNumber = 0;

            address = var_arr[i].address;
            var_string[0] = address / 10 + '0';
            var_string[1] = address % 10 + '0';
            var_string[2] = ' ';
            var_string[3] = '=';
            var_string[4] = ' ';
            var_string[5] = '+';

            int j = 9;
            for ( ; j >= 6; j--)
            {
                tempNumber = var_arr[i].value % 16;
                var_arr[i].value /= 16;

                if (tempNumber >= 10)
                {
                    var_string[j] = tempNumber - 10 + 'A';
                }
                else
                {
                    var_string[j] = tempNumber + '0';
                }
            }

            var_string[10] = '\n';
            fwrite(var_string, sizeof(char), strlen(input_str), ptr);
        }
    }

    fclose(ptr);

    system("rm tempFile.txt");
}

char *command_decode(char *input_str, char *command)
{
    int i;
    for (i = 0; i < 7; i++)
    {
        if (!strcmp(command, command_arr[i]))
        {
            break;
        }
    }

    char *result = malloc(sizeof(char) * 240);

    switch (i)
    {
        case 0:
        {
            //result = rem(input_str);
            break;
        }
        case 1:
        {
            result = input_func(input_str);
            break;
        }
        case 2:
        {
            result = print_func(input_str);
            break;
        }
        case 3:
        {
            result = goto_func(input_str);
            break;
        }
        case 4:
        {
            result = if_func(input_str);
            break;
        }
        case 5:
        {
            let(input_str, strlen(input_str));
            strcpy(result, asmCommand);
            strcpy(asmCommand, "\0");
            break;
        }
        case 6:
        {
            result = end(input_str);
            break;
        }
        default:
        {
            printf("wrong command!!! \n");
            exit(EXIT_FAILURE);
        }
    }

    return result;
}

int main()
{
    init_var_arr();
    char *input_str = '\0';
    char command[4], inputFileName[25], outputFileName[25];
    printf("Enter the command to SimpleBasic translator(sbt *.sb *.sa): \n");
    fflush(stdin);
    fflush(stdout);
    rk_myTermRegime(1, 0, 0, 0, 0);
    scanf("%s %s %s", command, inputFileName, outputFileName);

    if (strcmp(command, "sbt") != 0)
    {
        printf("\nWrong command!\n");
        sleep(3);
        return -1;
    }

    input = fopen(inputFileName, "r");
    output = fopen("tempFile.txt", "w");

    if (!input)
    {
        printf("Error open file! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        size_t tmp_length;

        while (getline(&input_str, &tmp_length, input) != -1)
        {
            bas_to_asm[count_bas - 1] = count_asm;
            char command[7] = {'\0'};

            for (int i = 3; input_str[i] != ' ' && input_str[i] != '\0' && input_str[i] != '\n'; i++)
            {
                command[i - 3] = input_str[i];
            }

            char *result = malloc(sizeof(char) * 240);
            result = command_decode(input_str, command);
            count_bas++;

            fwrite(result, sizeof(char), strlen(result), output);
        }

        fclose(output);
        output = fopen("tempFile.txt", "rt");
        movingFix(outputFileName);
    }

    fclose(input);
    fclose(output);

    return 0;
}
