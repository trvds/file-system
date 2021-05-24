/*
    Ficheiro:  functions.h
    Autor:  Tiago Rodrigues Vieira da Silva
    Descrição:  Header da main.c do Projeto 2 de IAED 2020/2021
*/

#ifndef FUNC
#define FUNC

/* Declaração de constantes */
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
#define HASH_VALUE 171007
#define NO_MEMORY "No memory\n"
#define NOT_FOUND "not found\n"
#define NO_DATA "no data\n"


/* Estrutura para a lista de comandos */
typedef struct { char *key; int val; char *description;} commands;


int commandcheck(char *key, commands commandlist[]);
void handle_help_command(commands commandlist[]);
void handle_set_command(tree_link root, list_link *hash_head);
void handle_print_command(tree_link root);
void check_tree_node(tree_link node);
void check_list_node(list_link node);
void print_directory(tree_link node);
void handle_find_command(tree_link root);
void handle_list_command(tree_link root);
void print_subdirs(tree_link head);
void handle_search_command(list_link *hash_head);
void handle_delete_command(tree_link root, list_link *hash_head);
void delete_tree(tree_link h, list_link *hash_head);
void handle_quit_command(tree_link root, list_link *hash_head);
void memory_check(void *pointer);

#endif