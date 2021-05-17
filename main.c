/*
    Ficheiro:  main.c
    Autor:  Tiago Rodrigues Vieira da Silva
    Descrição: Projeto 2 de IAED 2020/2021.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAX_SIZE 65536
#define QUIT 0
#define HELP 1
#define SET 2
#define PRINT 3
#define FIND 4
#define LIST 5
#define SEARCH 6
#define DELETE 7
#define COMMANDLIST_SIZE 8


typedef struct { char *key; int val; } commands;

static commands commandlist[COMMANDLIST_SIZE] = {
    { "quit", QUIT }, 
    { "help", HELP }, 
    { "set", SET }, 
    { "print", PRINT }, 
    { "find", FIND },
    { "list", LIST },
    { "search", SEARCH },
    { "delete", DELETE },
};

int commandcheck(char *key)
{
    int i;
    commands *temp = commandlist;
    commands *listcheck = temp;
    for (i=0; i < COMMANDLIST_SIZE; i++) {
        listcheck = temp + i;
        if (strcmp(listcheck->key, key) == 0)
            return listcheck->val;
    }
    return -1;
}

int main()
{
    char input[INPUT_MAX_SIZE];

    while(commandcheck(input) != QUIT)
    {
        scanf("%s", input);
        switch(commandcheck(input))
        {
        case HELP:
            /* code */
            printf("help");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case SET:
            /* code */
            printf("set");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case PRINT:
            /* code */
            printf("print");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case FIND:
            /* code */
            printf("find");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case LIST:
            /* code */
            printf("list");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case SEARCH:
            /* code */
            printf("search");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;

        case DELETE:
            /* code */
            printf("delete");
            fgets(input, INPUT_MAX_SIZE, stdin);
            printf("%s", input);
            break;
        
        case QUIT:
            break;

        default:
            printf("Erro no argumento introduzido!\n");
            break;
        }
    }
    return 0;
}