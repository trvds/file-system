#ifndef FILES
#define FILES



/*--------------------------------------------ESTRUTURAS------------------------------------------*/

/* Estrutura de uma node da AVL tree*/
typedef struct tree_node
{
    char *dir; /* nome da diretoria */
    char *value; /* valor da diretoria */
    struct tree_node *parent_dir;
    struct tree_node *subdirs; /* arvore com as subdiretorias da diretoria */
    struct list_node *creation; /* lista da ordem de criacao das subdiretorias */
    struct tree_node *left; /* node esquerda */
    struct tree_node *right; /* node direita */
    int height; /* altura da node*/
}* tree_link;


/* Estrutura de uma node da Linked List*/
typedef struct list_node
{
    tree_link dir; /* nome da diretoria */
    struct list_node *next; /* pointer para a node seguinte*/
}* list_link;




/*--------------------------------------------FUNÇÕES---------------------------------------------*/

/* Funções de manipulação das Linked Lists */
list_link new_listlink(tree_link dir);
list_link insert_listlink(list_link head, tree_link dir);
list_link delete_listlink(list_link head, tree_link ptr);

/* Funções de manipulação das AVL Trees */
tree_link new_treelink(char* dir_name, char* value, tree_link l, tree_link r);
int height(tree_link h);
tree_link rotL(tree_link h);
tree_link rotR(tree_link h);
tree_link rotLR(tree_link h);
tree_link rotRL(tree_link h);
int balance(tree_link h);
tree_link AVLbalance(tree_link h);
tree_link insert_treelink(tree_link h, char *dir_name, char *value);
tree_link delete_treelink(tree_link h, char *dir_name);
tree_link max_tree(tree_link h);
tree_link min_tree(tree_link h);
tree_link search_tree(tree_link h, char *dir_name);

/* Funções de manipulação da HASH TABLE */
extern list_link *hash_head;

int hash_s(char *value);
void init_hashtable(int m);
void insert_hash(tree_link h);
void delete_hash(tree_link h);

#endif