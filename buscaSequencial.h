#ifndef BUSCASEQUENCIAL_H
#define BUSCASEQUENCIAL_H

#include "funcionario.h" // include the header file that defines the TFunc type

/*
Realiza uma busca sequencial por um funcionario na base de dados
*/

TFunc *buscaSequencial(int chave, FILE *in);

#endif // BUSCASEQUENCIAL_H