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


typedef struct { char *key; int val; char *description;} commands;

static commands commandlist[COMMANDLIST_SIZE] = { 
    { "help", HELP, "Imprime os comandos disponíveis." },
    { "quit", QUIT, "Termina o programa." },
    { "set", SET, "Adiciona ou modifica o valor a armazenar." }, 
    { "print", PRINT, "Imprime todos os caminhos e valores." }, 
    { "find", FIND, "Imprime o valor armazenado." },
    { "list", LIST, "Lista todos os componentes de um caminho." },
    { "search", SEARCH, "Procura o caminho dado um valor." },
    { "delete", DELETE, "Apaga um caminho e todos os subcaminhos." },
};

int commandcheck(char *key)
{
    int i;
    commands *listcheck;
    for (i=0; i < COMMANDLIST_SIZE; i++) {
        listcheck = commandlist + i;
        if (strcmp(listcheck->key, key) == 0)
            return listcheck->val;
    }
    return -1;
}

void handle_help_command()
{
    int i;
    commands *temp;
    for(i=0; i < COMMANDLIST_SIZE; i++){
        temp = commandlist + i;
        printf("%s: %s\n", temp->key, temp->description);
    }
    return;
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
            handle_help_command();
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
    return QUIT;
}