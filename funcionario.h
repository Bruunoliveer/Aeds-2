#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do funcionário
typedef struct {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} TFunc;


// Retorna tamanho do funcionário em bytes
int tamanho_registro();

// Cria funcionário
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

// Salva funcionário no arquivo
void salva(TFunc *func, FILE *out);

// Lê um funcionário do arquivo
TFunc *le(FILE *in);

// Imprime funcionário
void imprime(TFunc *func);

// Cria a base de dados
void criarBase(FILE *out, int tam);

// Embaralha base de dados
void embaralhar_base(FILE *base);

// Imprime a base de dados
void imprimirBase(FILE *out);

// Função para ler um registro de funcionário do arquivo
TFunc *le_registro(FILE *in);

// Função para escrever um registro de funcionário no arquivo
void escreve_registro(TFunc *func, FILE *out);

long get_RRN_from_code(int cod);

#endif /* FUNCIONARIO_H */
