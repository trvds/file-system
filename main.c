/*
    Ficheiro:  main.c
    Autor:  Tiago Rodrigues Vieira da Silva
    Descrição: Projeto 2 de IAED 2020/2021.
*/

/* Declaração de libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atds.h"
#include "functions.h"


int main()
{
    /* declaração de variáveis */
    int i;
    char input[INPUT_MAX_SIZE];
    tree_link root;
    list_link *hash_head;

    /* inicialização de variáveis */
    /* lista de comandos (declarei na main porque não pode ser global) */
    commands commandlist[COMMANDLIST_SIZE] = { 
    { "help", HELP, "Imprime os comandos disponíveis." },
    { "quit", QUIT, "Termina o programa." },
    { "set", SET, "Adiciona ou modifica o valor a armazenar." }, 
    { "print", PRINT, "Imprime todos os caminhos e valores." }, 
    { "find", FIND, "Imprime o valor armazenado." },
    { "list", LIST, "Lista todos os componentes imediatos de um sub-caminho." },
    { "search", SEARCH, "Procura o caminho dado um valor." },
    { "delete", DELETE, "Apaga um caminho e todos os subcaminhos." }, };
    /* hash table */
    hash_head = malloc(HASH_VALUE*sizeof(list_link));
    for (i = 0; i < HASH_VALUE; i++)
        hash_head[i] = NULL;
    /* root */
    root = new_treelink(NULL, NULL, NULL, NULL);

    /* analisar o input e determinar o comando */
    scanf("%s", input);
    while(commandcheck(input, commandlist) != QUIT)
    {
        switch(commandcheck(input, commandlist))
        {
        case HELP:
            handle_help_command(commandlist);
            break;
        case SET:
            handle_set_command(root, hash_head);
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
            handle_search_command(hash_head);
            break;
        case DELETE:
            handle_delete_command(root, hash_head);
            break;
        default:
            printf("input error\n");
            break;
        }
    scanf("%s", input);
    }

    handle_quit_command(root, hash_head);
    return QUIT;
}

void memory_check(void *pointer)
{
    if (pointer == NULL)
    {
        printf(NO_MEMORY);
    }
}

/* 
    Pega na string do input e devolve um codigo equivalente a um comando.
*/
int commandcheck(char *key, commands commandlist[])
{
    /* declaração de variáveis */
    int i;
    commands *listcheck;
    /* verificar qual é o inteiro equivalente ao comando */
    for (i=0; i < COMMANDLIST_SIZE; i++) {
        listcheck = commandlist + i;
        if (strcmp(listcheck->key, key) == 0)
            return listcheck->val;
    }
    return -1;
}


/* 
    Função que trata do comando "help".
*/
void handle_help_command(commands commandlist[])
{
    /* declaração de variáveis */
    int i;
    commands *temp;
    /* dar print a todos os comandos da estrutura */
    for(i=0; i < COMMANDLIST_SIZE; i++){
        temp = commandlist + i;
        printf("%s: %s\n", temp->key, temp->description);
    }
    return;
}


/*
    Função que trata do comando "set". Dá "set" a uma diretoria.
*/
void handle_set_command(tree_link root, list_link *hash_head)
{
    /* declaração de variáveis */
    tree_link current_dir, father_dir = root;
    char path_buffer[INPUT_MAX_SIZE];
    char value_buffer[INPUT_MAX_SIZE];
    char *dir_token, *value, *new_dir;
    char c;
    /* analisar o input do terminal */
    scanf("%s", path_buffer);
    /* retirar do input as diretorias, uma de cada vez*/
    dir_token = strtok(path_buffer, "/");
    while(dir_token != NULL)
    {
        /* ver se a diretoria que estamos a analisar já existe */
        current_dir = search_tree(father_dir-> directory -> subdirs, dir_token);
        /* a diretoria não existe, por isso teremos que a criar */
        if(current_dir == NULL)
        {
            /* alocar memoria para a subdiretoria */
            new_dir = (char *)malloc(sizeof(char)*(strlen(dir_token)+1));
            /* verificar se há memória disponível */
            memory_check(new_dir);
            strcpy(new_dir, dir_token);
            /* inserir subdiretoria na diretoria pai */
            father_dir -> directory -> subdirs =
            insert_treelink(father_dir->directory->subdirs, new_dir, NULL);
            /* ir buscar o pointer da subdiretoria acabada de criar */
            current_dir = search_tree(father_dir -> directory -> subdirs, new_dir);
            /* inserir o pointer para a diretoria pai na subdiretoria */
            current_dir -> directory -> parent_dir = father_dir;
            /* criar lista da ordem de criação das diretorias */
            father_dir -> directory -> creation =
            insert_listlink(father_dir -> directory -> creation, current_dir -> directory);
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
    /* verificar se o comando tem o argumento relativo ao "valor"*/
    /* possui o argumento relativo ao valor */
    if((c = getchar()) == ' ')
    {
        /* analisar o argumento do terminal */
        scanf("%[^\n]s", value_buffer);
        /* alocar memória para o value */
        value = (char *)malloc(sizeof(char)*((strlen(value_buffer)+1)));
        /* verificar se há memória disponível*/
        memory_check(value);
        strcpy(value, value_buffer);
    }
    else
        /* não tem nenhum argumento para o valor */
        value = NULL;
    /* apagar qualque entrada anterior de um valor na hash-table e na node */
    delete_hash(father_dir, hash_head);
    free(father_dir -> directory -> value);
    father_dir -> directory -> value = NULL;
    /* inserir o novo value na node da diretoria */
    father_dir -> directory -> value = value;
    /* inserir o novo value na hash table se este existir */
    if(value != NULL)
        insert_hash(father_dir, hash_head);
}


/*
    Função relativa ao comando "print". Imprime todas os caminhos com valor
    associado.
*/
void handle_print_command(tree_link root)
{
    check_list_node(root -> directory -> creation);
}

/*
    Funções auxiliares da "handle_print_command".
    Irão imprimir todos os elementos da lista de criação que possuam valores
*/
void check_list_node(list_link node)    /* recebe uma node da lista de criação*/
{
    /* Condição de paragem da recursão */
    if(node == NULL)
        return;
    /* ir verificar node da diretoria para ver se tem subdiretorias e "value" */
    check_tree_node(node->dir);
    /* passar para o proximo elemento */
    check_list_node(node->next);
}

void check_tree_node(dir_link node)
{
    /* imprimir a diretoria se tiver valor associado */
    if (node -> value != NULL)
    {
        print_directory(node);
        printf(" %s\n", node -> value);
    }
    /* analisar as subdiretorias da diretoria */
    check_list_node(node -> creation);
    return;
}


/*
    Função auxiliar que imprime o path completo de uma diretoria por
    ordem de profundidade.
*/
void print_directory(dir_link node)
{
    /* Condição de paragem da recursão */
    if (node->dir == NULL)
        return;
    /* Ir para a diretoria pai */
    print_directory(node->parent_dir->directory);
    /* Imprimir a diretoria, por ordem de profundidade */
    printf("/%s", node->dir);
    return;
}


/*
    Função relativa ao comando "find". Imprime o valor de uma diretoria.
*/
void handle_find_command(tree_link root)
{
    /* declaração de variáveis */    
    tree_link current_node = root;
    char path_buffer[INPUT_MAX_SIZE];
    char *dir_token, *value;
    /* analisar o input do terminal e ir até à node da subdiretoria desejada */
    scanf("%s", path_buffer);
    dir_token = strtok(path_buffer, "/");
    while(dir_token != NULL)
    {
        current_node = current_node -> directory -> subdirs;
        current_node = search_tree(current_node, dir_token);
        /* se a node não existir devolver a mensagem de erro */
        if(current_node == NULL){
            printf(NOT_FOUND);
            return;
        }
        dir_token = strtok(NULL, "/");
    }
    /* retirar o value da node e apresentá-lo se existir */
    value = current_node -> directory -> value;
    if (value == NULL)
        printf(NO_DATA);
    else
        printf("%s\n", value);
}


/*
    Função relativa ao comando "list". Lista todas as subdiretorias de uma
    diretoria.
*/
void handle_list_command(tree_link root)
{
    /* declaração de variáveis */    
    tree_link current_node = root;
    char path_buffer[INPUT_MAX_SIZE];
    char *dir_token;
    char c;
    /* Verificar o input contem argumentos */
    if((c = getchar()) == ' ')
    {
    /* analisar o input do terminal e ir até à node da subdiretoria desejada */
        scanf("%s", path_buffer);
        dir_token = strtok(path_buffer, "/");
        while(dir_token != NULL)
        {
            current_node = current_node -> directory -> subdirs;
            current_node = search_tree(current_node, dir_token);
            /* se a node não existir devolver a mensagem de erro */
            if(current_node == NULL){
                printf(NOT_FOUND);
                return;
            }
            dir_token = strtok(NULL, "/");
        }
    }
    /* Imprimir todas as subdiretorias da diretoria */
    print_subdirs(current_node -> directory -> subdirs);
}

/*
    Função auxiliar da "handle list command"
    Imprime todas as subdiretorias por ordem alfabetica.
    (transverse In Order)
*/
void print_subdirs(tree_link head)
{
        if (head == NULL)
            return;
        print_subdirs(head->left);
        printf("%s\n", head->directory->dir);
        print_subdirs(head->right);
    return;
}


/*
    Função relativa ao comando "search". Irá listar uma diretoria com o valor
    requisitado. 
*/
void handle_search_command(list_link *hash_head)
{
    /* declaração de variáveis */    
    char value_buffer[INPUT_MAX_SIZE];
    list_link h;
    int i;
    /* analisar o argumento do terminal(valor) */
    getchar();
    scanf("%[^\n]s", value_buffer);
    /* com a hash function obter o indice relativo ao valor */
    i = hash_s(value_buffer);
    /* retirar a node da hashtable referente ao value*/
    h = hash_head[i];
    /* Verificar se o elemento obtido tem o valor pretendido */
    while(h != NULL)
    {
        if (strcmp(h->dir->value, value_buffer) == 0)
            break;
        h = h -> next;
    }
    /* Imprimir a diretoria pretendida ou a mensagem de erro no caso de nao
       existir.
    */
    if (h == NULL)
        printf(NOT_FOUND);
    else
    {
        print_directory(h -> dir);
        printf("\n");
    }
}


/*
    Função relativa ao comando "delete". Elimina uma diretoria e todas as suas
    subdiretorias. Se não possuir apaga tudo.
*/
void handle_delete_command(tree_link root, list_link *hash_head)
{
    /* declaração de variáveis */ 
    tree_link current_node = root;
    tree_link father_node;
    char path_buffer[INPUT_MAX_SIZE];
    char *dir_token, c;
    /* analisar se o input possui argumentos */
    if ((c=getchar()) == ' ')
    {
        /* obter a diretoria que se pretende eliminar do terminal*/
        scanf("%s", path_buffer);
        dir_token = strtok(path_buffer, "/");
        while(dir_token != NULL)
        {
            current_node = current_node -> directory -> subdirs;
            current_node = search_tree(current_node, dir_token);
            /* erro se a diretoria não existir */
            if(current_node == NULL){
                printf(NOT_FOUND);
                return;
            }
            dir_token = strtok(NULL, "/");
        }
        /* apagar as subdiretorias da diretoria */
        delete_tree(current_node->directory->subdirs, hash_head);
        current_node -> directory -> subdirs = NULL;
        /* apagar a lista de criação da diretoria */
        nuke_list(current_node->directory->creation);
        current_node -> directory -> creation = NULL;
        /* apagar a entrada na hash table da diretoria se possuir valor */
        delete_hash(current_node, hash_head);
        /* father_node irá ser o pai da diretoria */
        father_node = current_node -> directory -> parent_dir;
        /* apagar a diretoria da árvore de subdiretorias da diretoria pai */
        father_node->directory->creation = 
        delete_listlink(father_node->directory->creation, current_node->directory);
        /* apagar a diretoria da lista de criação da diretoria pai*/
        father_node->directory->subdirs = 
        delete_treelink(father_node->directory->subdirs, current_node->directory->dir);
    }
    else
    {
        /* se nao possuir apaga-se as subdiretorias da root */
        delete_tree(root->directory->subdirs, hash_head);
        /* apagar a lista da ordem de criação da root */
        nuke_list(root->directory->creation);
        root -> directory -> subdirs = NULL;
        root -> directory -> creation = NULL;
    }
}


/*
    Função auxiliar da "handle_delete_command". Irá apagar todas as
    subdiretorias de uma diretoria.
*/
void delete_tree(tree_link h, list_link *hash_head)
{
    /* Ponto de paragem da recursão */
    if (h == NULL)
        return;
    /* apagar os elementos da arvore*/
    delete_tree(h->left, hash_head);
    delete_tree(h->right, hash_head);
    /* se o elemento tiver subdiretorias, apaga-se as subdiretorias */
    if (h -> directory -> subdirs != NULL)
    {
        delete_tree(h -> directory -> subdirs, hash_head);
        nuke_list(h -> directory -> creation);
    }
    /* apagar a entrada na hash table */
    delete_hash(h, hash_head);
    /* dar free da string do nome da diretoria e do value */
    free(h->directory->dir);
    h->directory->dir = NULL;
    free(h->directory->value);
    h->directory->value = NULL;
    free(h-> directory);
    h-> directory = NULL;
    free(h);
    h = NULL;
    return;
}


/*
    Função relativa ao comando "quit". Termina controladamente o programa.
*/
void handle_quit_command(tree_link root, list_link *hash_head)
{
    /* apagar todas as subdiretorias da root */
    delete_tree(root->directory->subdirs, hash_head);
    /* apagar a lista de criação da root */
    nuke_list(root->directory->creation);
    /* dar free da hash table e da root */
    free(hash_head);
    free(root->directory);
    free(root);
    return;
}