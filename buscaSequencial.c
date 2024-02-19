#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//Realiza uma busca sequencial por um funcionario na base de dados
TFunc *buscaSequencial(int chave, FILE *in){

    TFunc *f;
    int achou;
    rewind(in);

    while ((f = le(in)) != NULL){

        if(f->cod == chave){
           //return f;
           achou = 1;
           break;
        }
    }
        if(achou == 1)
            return f;
        else printf("Funcionario nao encontrado");

        free(f);

     return NULL;
}

