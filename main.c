/*
    Ficheiro:  main.c
    Autor:  Tiago Rodrigues Vieira da Silva
    Descrição: Projeto 2 de IAED 2020/2021.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"

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


/* Estrutura para a lista de comandos */
typedef struct { char *key; int val; char *description;} commands;

/* Lista de comandos */
static commands commandlist[COMMANDLIST_SIZE] = { 
    { "help", HELP, "Imprime os comandos disponíveis." },
    { "quit", QUIT, "Termina o programa." },
    { "set", SET, "Adiciona ou modifica o valor a armazenar." }, 
    { "print", PRINT, "Imprime todos os caminhos e valores." }, 
    { "find", FIND, "Imprime o valor armazenado." },
    { "list", LIST, "Lista todos os componentes imediatos de um sub-caminho." },
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


void handle_set_command(tree_link root)
{
    tree_link current_dir, father_dir = root;
    char path_buffer[INPUT_MAX_SIZE];
    char value_buffer[INPUT_MAX_SIZE];
    char *dir_token, *value, *new_dir;
    char c;

    scanf("%s", path_buffer);
    /* analisar cada diretoria*/
    dir_token = strtok(path_buffer, "/");
    while(dir_token != NULL)
    {
        current_dir = search_tree(father_dir -> subdirs, dir_token);
        /* a diretoria já existe, passar para a subdiretoria */
        if(current_dir == NULL)
        {
            /* alocar memoria para a subdiretoria */
            new_dir = (char *)malloc(sizeof(char)*(strlen(dir_token)+1));
            strcpy(new_dir, dir_token);
            /* inserir subdiretoria na diretoria pai */
            father_dir -> subdirs = insert_treelink(father_dir->subdirs, new_dir, NULL);
            /* ir buscar o pointer da subdiretoria acabada de criar */
            current_dir = search_tree(father_dir -> subdirs, new_dir);
            /* inserir o pointer para a diretoria pai na subdiretoria */
            current_dir -> parent_dir = father_dir;
            /* criar lista da ordem de criação das diretorias */
            father_dir -> creation = insert_listlink(father_dir -> creation, current_dir);
            /* vamos passar para a subdiretoria a analisar */
            father_dir = current_dir;
        }
        else
        {
            /* a diretoria já existe, passar para a subdiretoria*/
            father_dir = current_dir;
        }
        dir_token = strtok(NULL, "/");
    }
    if((c = getchar()) == ' ')
    {
        scanf("%[^\n]s", value_buffer);
        value = (char *)malloc(sizeof(char)*((strlen(value_buffer)+1)));
        strcpy(value, value_buffer);
    }
    else
        value = NULL;
    free(father_dir -> value);
    father_dir -> value = value;
}

void check_tree_node(tree_link node);
void check_list_node(list_link node);
void print_directory(tree_link node);

void handle_print_command(tree_link root)
{
    check_list_node(root -> creation);
}

void check_tree_node(tree_link node)
{
    if (node -> value != NULL)
    {
        print_directory(node);
        printf(" %s\n", node -> value);
    }
    check_list_node(node -> creation);
    return;
}

void check_list_node(list_link node)
{
    if(node == NULL)
        return;
    check_tree_node(node->dir);
    check_list_node(node->next);
}

void print_directory(tree_link node)
{
    if (node->dir == NULL)
        return;
    print_directory(node->parent_dir);
    printf("/%s", node->dir);
    return;
}


void handle_find_command(tree_link root)
{
    tree_link current_node = root;
    char path_buffer[INPUT_MAX_SIZE];
    char *dir_token, *value;

    scanf("%s", path_buffer);
    dir_token = strtok(path_buffer, "/");
    while(dir_token != NULL)
    {
        current_node = current_node -> subdirs;
        current_node = search_tree(current_node, dir_token);
        if(current_node == NULL){
            printf("not found\n");
            return;
        }
        dir_token = strtok(NULL, "/");
    }
    
    value = current_node -> value;
    if (value == NULL)
        printf("no data\n");
    else
        printf("%s\n", value);
}


void print_subdirs(tree_link head)
{
        if (head == NULL)
            return;
        print_subdirs(head->left);
        printf("%s\n", head->dir);
        print_subdirs(head->right);
    return;
}


void handle_list_command(tree_link root)
{
    tree_link current_node = root;
    char path_buffer[INPUT_MAX_SIZE];
    char *dir_token;
    char c;

    if((c = getchar()) == ' ')
    {
        scanf("%s", path_buffer);
        dir_token = strtok(path_buffer, "/");
        while(dir_token != NULL)
        {
            current_node = current_node -> subdirs;
            current_node = search_tree(current_node, dir_token);
            if(current_node == NULL){
                printf("not found\n");
                return;
            }
            dir_token = strtok(NULL, "/");
        }
    }

    print_subdirs(current_node -> subdirs);
}


int main()
{
    char input[INPUT_MAX_SIZE];
    tree_link root;
    root = new_treelink(NULL, NULL, NULL, NULL);

    while(commandcheck(input) != QUIT)
    {
        scanf("%s", input);
        switch(commandcheck(input))
        {
        case HELP:
            handle_help_command();
            break;
        case SET:
            handle_set_command(root);
            break;
        case PRINT:
            handle_print_command(root);
            break;
        case FIND:
            handle_find_command(root);
            break;
        case LIST:
            handle_list_command(root);
            break;
        case SEARCH:
            printf("workind in progress");
            break;
        case DELETE:
            printf("workind in progress");
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