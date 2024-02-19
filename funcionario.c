#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define TAM_BLOCO 100 // Tamanho do bloco de registros

// Retorna tamanho do funcionario em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double); //salario
}


// Cria funcionario.
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaco de memoria com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    strcpy(func->cpf, cpf);
    strcpy(func->data_nascimento, data_nascimento);
    func->salario = salario;
    return func;
}


// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}


// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}


// Imprime funcionario
void imprime(TFunc *func) {
    printf("\n**********************************************");
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nCPF: ");
    printf("%s", func->cpf);
    printf("\nData de Nascimento: ");
    printf("%s", func->data_nascimento);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
}


// Cria a base de dados
void criarBase(FILE *out, int tam){

    TFunc *f;
    int i;

    printf("\nGerando a base de dados...\n");
    
    for (i=0;i<tam;i++){
        char nome[50];
        sprintf(nome, "Funcionario %d", i);  // Formata o nome com o número i
        f = funcionario(i, nome, "000.000.000-00", "01/01/1980", 3000 * i);
        salva(f, out);
        free(f);
    }

}

//embaralha base de dados
void embaralhar_base(FILE *base) {
    // Obtém o tamanho do arquivo
    fseek(base, 0, SEEK_END);
    long tamanho = ftell(base);
    rewind(base);

    // Calcula o número de registros na base
    int num_registros = tamanho / tamanho_registro();

    // Lê todos os registros da base em um array de ponteiros
    TFunc **registros = (TFunc **)malloc(num_registros * sizeof(TFunc *));
    if (registros == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_registros; i++) {
        registros[i] = le(base);
        if (registros[i] == NULL) {
            fprintf(stderr, "Erro ao ler registro da base de dados.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Embaralha os registros aleatoriamente
    srand(time(NULL));
    for (int i = num_registros - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        TFunc *temp = registros[i];
        registros[i] = registros[j];
        registros[j] = temp;
    }

    // Reescreve os registros embaralhados na base de dados
    rewind(base);
    for (int i = 0; i < num_registros; i++) {
        salva(registros[i], base);
    }

    // Libera a memória alocada para os registros
    for (int i = 0; i < num_registros; i++) {
        free(registros[i]);
    }
    free(registros);
}


void imprimirBase(FILE *out){

printf("\n Imprimindo a base de dados...\n");

    rewind(out);
    TFunc *f;

    while ((f = le(out)) != NULL)
        imprime(f);

    free(f);

}

// Função para ler um registro de funcionário do arquivo
TFunc *le_registro(FILE *in) {
    TFunc *func = (TFunc *)malloc(sizeof(TFunc));
    if (func == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    if (fread(func, sizeof(TFunc), 1, in) != 1) {
        free(func);
        return NULL;
    }

    return func;
}

// Função para escrever um registro de funcionário no arquivo
void escreve_registro(TFunc *func, FILE *out) {
    fwrite(func, sizeof(TFunc), 1, out);
}


// Função para obter o RRN (índice do arquivo) de um funcionário com base no código
long get_RRN_from_code(int cod) {
    FILE *file = fopen("base_de_dados.bin", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        exit(1);
    }

    TFunc func;
    long RRN = -1; // Valor padrão caso o funcionário não seja encontrado

    while (fread(&func, sizeof(TFunc), 1, file)) {
        if (func.cod == cod) {
            RRN = ftell(file) / sizeof(TFunc) - 1; // Calcula o RRN
            break;
        }
    }

    fclose(file);
    return RRN;
}

