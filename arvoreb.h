#ifndef ARVOREB_H
#define ARVOREB_H

#include "funcionario.h"
#include <stdio.h>

// Definição da estrutura Key_Sorting
typedef struct key_sorting {
    int cod;    // Código do funcionário como chave
    long RRN;   // Índice do funcionário no arquivo
} Key_Sorting;

// Definição da estrutura TreeNode
typedef struct TreeNode {
    Key_Sorting *key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Protótipos das funções
TreeNode *create_node(Key_Sorting *x);
void write_node_to_file(TreeNode *node, long position);
TreeNode *read_node_from_file(long position);
long get_free_position();
int insert_node(TreeNode **ppRoot, TFunc *func);
void create_tree_with_func(TFunc *funcs, TreeNode **root, int n);
int search_node(TreeNode *root, int code);
void print_tree(TreeNode *root);
void remove_node(TreeNode *root, int id);

#endif /* ARVOREB_H */