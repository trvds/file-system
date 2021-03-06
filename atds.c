/*
    Ficheiro:  atds.c
    Autor:  Tiago Rodrigues Vieira da Silva
    Descrição:  Funções para a manipulação das ATDS do atds.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atds.h"
#include "functions.h"


/* = Funções da lista ======================================================= */

/* Criar nova node na lista */
list_link new_listlink(dir_link new_tree)
{
    list_link new = (list_link) malloc(sizeof(struct list_node));
    memory_check(new);
    new -> dir = new_tree;
    new -> next = NULL;
    return new;
}

/* Inserir a node na lista */
list_link insert_listlink(list_link head, dir_link dir)
{
    list_link new;
    if (head == NULL)
        return new_listlink(dir);
    for(new = head; new -> next != NULL; new = new -> next)
        ;
    new -> next = new_listlink(dir);
    return head;
}


/* Eliminar a node da lista */
list_link delete_listlink(list_link head, dir_link ptr)
{
    list_link t, prev;
    for (t = head, prev = NULL; t != NULL; prev = t, t = t -> next)
    {
        if (t -> dir == ptr){
            if (t == head)
                head = t -> next;
            else
                prev -> next = t -> next;
            t -> next = NULL;
            free(t);
            t = NULL;
            break;
        }
    }
    return head;
}


/* Procurar um valor na lista */
list_link search_list(list_link head, dir_link ptr)
{
    list_link x = head;
    while(x != NULL) {
        if(x->dir == ptr)
            return x;
        x = x -> next;
    }
    return x;
}


/* Apagar a lista toda */
void nuke_list(list_link head)
{
    if (head == NULL)
        return;
    nuke_list(head -> next);
    free(head);
    head = NULL;
    return;
}

/* = Funções da AVL ==========================================================*/

/* Criar node de uma diretoria*/
dir_link new_dirlink(char* dir_name, char* value)
{
    dir_link new = (dir_link) malloc(sizeof(struct dir_node));
    new -> dir = dir_name;
    new -> value = value;
    new -> subdirs = NULL;
    new -> creation = NULL;
    return new;
}


/* Criar node da árvore*/
tree_link new_treelink(char* dir_name, char* value, tree_link l, tree_link r)
{
    tree_link new = (tree_link) malloc(sizeof(struct tree_node));
    memory_check(new);
    new -> directory = new_dirlink(dir_name, value);
    new -> left = l;
    new -> right = r;
    new -> height = 1;
    return new;
}


/* Analisar altura da node da árvore */
int height(tree_link h)
{
    if (h == NULL)
        return 0;
    return h->height;
}


/* Rotação para a esquerda */
tree_link rotL(tree_link h)
{
    int hleft, hright, xleft, xright;
    tree_link x = h-> right;
    h -> right = x -> left;
    x -> left = h;
    hleft = height(h -> left);
    hright = height(h -> right);
    h -> height = hleft > hright ? hleft + 1 : hright + 1;
    xleft = height( x -> left);
    xright = height(x -> right);
    x -> height = xleft > xright ? xleft + 1 : xright + 1;
    return x;
}


/* Rotação para a direita */
tree_link rotR(tree_link h)
{
    int hleft, hright, xleft, xright;
    tree_link x = h->left;
    h->left = x->right;
    x->right = h;
    hleft = height(h->left);
    hright = height(h->right);
    h->height = hleft > hright ? hleft + 1 : hright + 1;
    xleft = height(x->left);
    xright = height(x->right);
    x->height = xleft > xright ? xleft + 1 : xright + 1;
    return x;
}

/* Rotação esquerda direita */
tree_link rotLR(tree_link h) 
{
    if (h == NULL)
        return h;
    h->left = rotL(h->left);
    return rotR(h);
}


/* Rotação direita esquerda */
tree_link rotRL(tree_link h) 
{
    if (h == NULL)
        return h;
    h->right = rotR(h->right);
    return rotL(h);
}


/* Fator de equilíbrio */
int balance(tree_link h)
{
    if(h == NULL)
        return 0;
    return height(h->left) - height(h->right);
} 


/* Equilibrar a árvore*/
tree_link AVLbalance(tree_link h)
{
    int balanceFactor, hleft, hright;
    if (h == NULL)
        return h;
    balanceFactor = balance(h);
    if(balanceFactor > 1) { /* mais peso para a esquerda */
        if (balance(h->left) >= 0)
            h = rotR(h);
        else h = rotLR(h);
    }
    else if(balanceFactor < -1){ /* mais peso para a direita*/
        if (balance(h->right) <= 0)
            h = rotL(h);
        else h = rotRL(h);
    }
    else{
        hleft = height(h->left);
        hright = height(h->right);
        h->height = hleft > hright ? hleft + 1 : hright + 1;
    }
    return h;
}


/* Inserir um node da árvore */
tree_link insert_treelink(tree_link h, char *dir_name, char *value)
{
    if (h == NULL)
        return new_treelink(dir_name, value, NULL, NULL);
    if (strcmp(dir_name, h -> directory -> dir) < 0)
        h->left = insert_treelink(h->left, dir_name, value);
    else
        h->right = insert_treelink(h->right, dir_name, value);
    h = AVLbalance(h);
    return h;
}


/* Eliminar um node da árvore */
tree_link delete_treelink(tree_link h, char *dir_name)
{
    tree_link aux;
    if (h == NULL)
        return h;
    else if (strcmp(dir_name, h -> directory -> dir) < 0)
        h->left = delete_treelink(h->left, dir_name);
    else if (strcmp(h -> directory -> dir, dir_name) < 0) 
        h->right = delete_treelink(h->right, dir_name) ;
    else{
        if (h->left != NULL && h->right != NULL){
            aux = max_tree(h->left);
            {dir_link x; x = h->directory; h->directory = aux->directory; aux->directory = x;}
            h->left = delete_treelink(h->left, aux->directory->dir);
        }
        else {
            aux = h;
            if (h->left == NULL && h->right == NULL) h = NULL;
            else if (h->left == NULL) h = h->right;
            else h = h->left;
            free(aux->directory->dir);
            aux->directory->dir = NULL;
            free(aux->directory->value);
            aux->directory->value = NULL;
            free(aux->directory);
            aux->directory = NULL;
            free(aux);
            aux = NULL;
        }
    }
    h = AVLbalance(h);
    return h;
}


/* Obter o máximo da árvore */
tree_link max_tree(tree_link h) {
    if (h == NULL || h->right == NULL)
        return h;
    else
        return max_tree(h->right);
}

/* Obter o minímo da árvore */
tree_link min_tree(tree_link h) {
    if (h == NULL || h->left == NULL)
        return h;
    else
        return min_tree(h->left);
}


/* Procurar por um elemento na árvore */
tree_link search_tree(tree_link h, char *dir_name) {
    if (h == NULL)
        return NULL;
    if (strcmp(dir_name, h -> directory -> dir) == 0)
        return h;
    if (strcmp(dir_name, h -> directory -> dir) < 0)
        return search_tree(h->left, dir_name);
    else
        return search_tree(h->right, dir_name);
}

/* = funções da Hash Table ===================================================*/
#define HASH_VALUE 171007

/*Criar chave para a hashtable*/
unsigned long hash_s(char *value)
{
    unsigned long h, a = 31415, b = 27183;
    for (h = 0; *value != '\0'; value++, a = a*b % (HASH_VALUE-1))
        h = (a*h + *value) % HASH_VALUE;
    return h;
}


/* Inserir na hashtable */
void insert_hash(tree_link h, list_link *hash_head)
{
    int i = hash_s(h->directory->value);
    hash_head[i] = insert_listlink(hash_head[i], h->directory);
}


/*Apagar da hashtable*/
void delete_hash(tree_link h, list_link *hash_head)
{
    int i;
    if (h->directory->value == NULL)
        return;
    i = hash_s(h->directory->value);
    hash_head[i] = delete_listlink(hash_head[i], h->directory);
}