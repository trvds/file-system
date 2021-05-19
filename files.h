#ifndef FILES
#define FILES


/*--------------------------------------------ESTRUTURAS------------------------------------------*/

typedef struct tree_node
{
    char *dir; /* nome da diretoria */
    void *value; /* valor da diretoria */
    struct tree_node *subdirs; /* arvore com as subdiretorias da diretoria */
    struct list_node *creation; /* lista da ordem de criacao das subdiretorias */
    struct tree_node *left; /* node esquerda */
    struct tree_node *right; /* node direita */
    int height; /* altura da node*/
}* tree_link;


typedef struct list_node
{
    tree_link current; /* pointer para uma subdiretoria */
    struct list_node *next; /* pointer para a node seguinte*/
}* list_link;




/*--------------------------------------------FUNÇÕES---------------------------------------------*/

/* Funções de manipulação das Linked Lists*/
list_link new_listlink(tree_link new_tree);
list_link insert_listlink(list_link head, tree_link new_tree);
list_link delete_listlink(list_link head, tree_link ptr);

/* Funções de manipulação das AVL Trees*/
tree_link NEW(char* dir_name, void* value, tree_link l, tree_link r);
int height(tree_link h);
tree_link rotL(tree_link h);
tree_link rotR(tree_link h);
tree_link rotLR(tree_link h);
tree_link rotRL(tree_link h);
int balance(tree_link h);
tree_link AVLbalance(tree_link h);
tree_link insertR(tree_link h, char *dir_name, void *value);
tree_link deleteR(tree_link h, char *dir_name);


#endif