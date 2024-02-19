#include "funcionario.h"
#include "buscaBinaria.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

TFunc* busca_binaria(int cod, FILE *arq, int tam) {

  int left = 0, right = tam - 1;
  while(left <= right)
  {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro(), SEEK_SET);
    TFunc* func = le(arq);
    if(cod == func->cod) {
      return func;
    }
    else if(func->cod < cod) {
      left = middle + 1;
    }
    else {
      right = middle - 1;
    }
  }

  return NULL;
}