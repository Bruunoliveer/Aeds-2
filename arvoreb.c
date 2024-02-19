#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionario.h"
#include "arvoreb.h"

TreeNode *create_node(Key_Sorting *x) {
    TreeNode *new_node = (TreeNode *)malloc(sizeof(TreeNode));
    new_node->key = (Key_Sorting *)malloc(sizeof(Key_Sorting));
    new_node->key->cod = x->cod;
    new_node->key->RRN = x->RRN;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void write_node_to_file(TreeNode *node, long position) {
    FILE *file = fopen("arvoreb.bin", "r+b");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    fseek(file, position, SEEK_SET);
    fwrite(node, sizeof(TreeNode), 1, file);
    fclose(file);
}

TreeNode *read_node_from_file(long position) {
    FILE *file = fopen("arvoreb.bin", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    fseek(file, position, SEEK_SET);
    fread(node, sizeof(TreeNode), 1, file);
    fclose(file);
    return node;
}

long get_free_position() {
    FILE *file = fopen("arvoreb.bin", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long position = ftell(file);
    fclose(file);
    return position;
}

// Demais funções de manipulação da árvore B

int insert_node(TreeNode **ppRoot, TFunc *func) {
    if (*ppRoot == NULL) {
        Key_Sorting *key = (Key_Sorting *)malloc(sizeof(Key_Sorting));
        key->cod = func->cod;
        key->RRN = get_RRN_from_code(func->cod); // Obtém o RRN do funcionário
        *ppRoot = create_node(key);
        return 1;
    }
    if (func->cod < (*ppRoot)->key->cod)
        return insert_node(&((*ppRoot)->left), func);
    if (func->cod > (*ppRoot)->key->cod)
        return insert_node(&((*ppRoot)->right), func);
    return 0;
}

void create_tree_with_func(TFunc *funcs, TreeNode **root, int n) {
    for (int j = 0; j < n; j++) {
        insert_node(root, &funcs[j]);
    }
}

int search_node(TreeNode *root, int code) {
    if (root == NULL) {
        printf("Arvore nula!");
        return 0;
    }
    else if (code < root->key->cod) {
        return search_node(root->left, code);
    }
    else if (code > root->key->cod) {
        return search_node(root->right, code);
    }
    else {
        printf("\nCodigo %d encontrado no indice %ld!\n\n", root->key->cod, root->key->RRN);
        printf("\n********* INFORMACOES DO FUNCIONARIO %d ENCONTRADO ********* !\n\n", root->key->cod);
        return 1;
    }
}

void print_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    else {
        print_tree(root->left);
        printf("\nCode: %d\n", root->key->cod);
        printf("Indice: %ld\n", root->key->RRN);
        print_tree(root->right);
    }
}

void remove_node(TreeNode *root, int id) {
    TreeNode *prox = NULL;
    TreeNode *atual = root;

    while (atual != NULL) {
        if (atual->key->cod == id) {
            if (atual->left == NULL && atual->right == NULL) {
                if (prox == NULL) {
                    root = NULL;
                }
                else {
                    if (prox->left == atual) {
                        prox->left = NULL;
                    }
                    else {
                        prox->right = NULL;
                    }
                }
            }
            else if (atual->left == NULL) {
                if (prox == NULL) {
                    root = atual->right;
                }
                else {
                    if (prox->left == atual) {
                        prox->left = atual->right;
                    }
                    else {
                        prox->right = atual->right;
                    }
                }
            }
            else if (atual->right == NULL) {
                if (prox == NULL) {
                    root = atual->left;
                }
                else {
                    if (prox->left == atual) {
                        prox->left = atual->left;
                    }
                    else {
                        prox->right = atual->left;
                    }
                }
            }
            else {
                TreeNode *successor = atual->right;
                TreeNode *successor_parent = atual;

                while (successor->left != NULL) {
                    successor_parent = successor;
                    successor = successor->left;
                }

                if (successor_parent->left == successor) {
                    successor_parent->left = successor->right;
                }
                else {
                    successor_parent->right = successor->right;
                }

                atual->key = successor->key;
            }

            free(atual);
            return;
        }
        else if (atual->key->cod > id) {
            prox = atual;
            atual = atual->left;
        }
        else {
            prox = atual;
            atual = atual->right;
        }
    }
}