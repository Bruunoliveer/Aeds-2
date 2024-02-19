#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h> 

#include "selecaoNatural.h"
#include "ordenacao.h"

TFunc* getFunc(FILE* arq, int* contLidos)
{
    fseek(arq, *contLidos * tamanho_registro(), SEEK_SET);
    TFunc* func = le_registro(arq);
    (*contLidos)++;
    return func;
}

// Fun��o que recebe o indice do menor elemento do vetor,
// joga ele para o inicio e retorna sua posi��o
int menorParaInicio(TFunc **v, int M, int menor) {
    // A função não precisa fazer nada, pois já encontramos o menor no início.
    return menor;
}
// Fun��o que retorna a quantidade de elementos dentro do vetor
int elementosNoVetor(bool controle[], int tam)
{
    int aux = 0;
    for (int i = 0; i < tam; i++)
    {
        if (controle[i] == false)
        {
            aux++;
        }
    }
    return(aux);
}

// Procedimento que cria novas parti��es caso necess�rio
void atualizaNomesParticao(Lista* nomes, int* nParticoes)
{
    if(nomes->prox == NULL)
    {
        char* newNome = malloc(5 * sizeof(char));
        (*nParticoes)++;
        sprintf(newNome, "p%d.bin", *nParticoes);
        nomes->prox = cria(newNome, NULL);
    }
}

// Fun��o que retorna o �ndice do menor registro do vetor
int getMenor(TFunc **v, int M) {
    int menor = 0;
    for (int i = 1; i < M; i++) {
        if (v[i]->cod < v[menor]->cod) {
            menor = i;
        }
    }
    return menor;
}

// Fun��o para verificar se um vetor de tamanho tam possui elementos
int possuiElementos(TFunc* v[], int tam)
{
    for (int i = 0; i < tam; i++)
    {
        if(v[i] != NULL){
            return(1);
        }
    }
    return(0);
}

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc)
{
    rewind(arq); //posiciona cursor no inicio do arquivo
    int reg = 0;

    while (reg != nFunc)
    {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq))
        {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_registro(arq);
            //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M)
                break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M)
        {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++)
        {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod))
            {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        else
        {
            for (int i = 0; i < M; i++)
            {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                escreve_registro(v[i], p);
                imprime(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

// Função de comparação para qsort
int compara_registros(const void *a, const void *b) {
    TFunc *registro_a = (TFunc *)a;
    TFunc *registro_b = (TFunc *)b;
    
    if (registro_a->cod < registro_b->cod) return -1;
    if (registro_a->cod > registro_b->cod) return 1;
    return 0;
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n, int* nParticoes) {
    Lista* nomes = nome_arquivos_saida;
    Lista* nome_part = nomes;
    FILE* particao = NULL;
    int cursorPart = 0;
    int totalLidos = 0;

    TFunc** v = malloc(M * sizeof(TFunc*));

    FILE* res = fopen("reservatorio.bin", "wb+");
    int cursorRes = 0;
    bool reservatorio_cheio;

    bool controleVet[M];
    int i = 0;

    // M registros do arquivo para a memória
    while (i < M && totalLidos < nFunc && !feof(arq)) {
        v[i] = malloc(sizeof(TFunc)); // Aloca memória para o registro
        if (fread(v[i], sizeof(TFunc), 1, arq) == 1) {
            controleVet[i] = false;
            i++;
            totalLidos++;
        }
    }

    // Abre arquivo de partição
    if (particao == NULL) {
        if (*nParticoes > 0) {
            char* nome = nome_part->nome;
            particao = fopen(nome, "wb+");
            nome_part = nome_part->prox;
            (*nParticoes)--; // Decrementa o número de partições restantes
        } else {
            printf("Todas as partições foram utilizadas.\n");
            return;
        }
    }

    // Enquanto há elementos no vetor e todos os registros não foram lidos
    while (totalLidos < nFunc) {
        reservatorio_cheio = false;
        // Enquanto não chega ao fim do arquivo e o reservatório não está cheio
        while (totalLidos < nFunc && !reservatorio_cheio) {
            // Caso não haja partição aberta ou a partição atual esteja cheia, abre a próxima partição
            if (cursorPart >= M) {
                if (particao != NULL) {
                    fclose(particao);
                    particao = NULL;
                    cursorPart = 0;
                }
                if (*nParticoes > 0) {
                    char* nome = nome_part->nome;
                    particao = fopen(nome, "wb+");
                    nome_part = nome_part->prox;
                    (*nParticoes)--; // Decrementa o número de partições restantes
                } else {
                    // printf("Todas as partições foram utilizadas.\n");
                    return;
                }
            }
            // Pega o menor valor do vetor
            int menor = getMenor(v, M);
            menor = menorParaInicio(v, M, menor);

            // Salva o menor registro na partição
            fseek(particao, cursorPart * sizeof(TFunc), SEEK_SET);
            fwrite(v[menor], sizeof(TFunc), 1, particao);
            cursorPart++;
            controleVet[menor] = true;
            // Salva o código do registro particionado acima
            int lastKey = v[menor]->cod;

            // Enquanto o índice do registro particionado não for trocado e nenhuma das condições de parada anteriores for verdadeira
            while (controleVet[menor] && !reservatorio_cheio && totalLidos < nFunc) {
                if (!feof(arq)) {
                    // Troca o registro já particionado com o próximo registro do arquivo
                    if (fread(v[menor], sizeof(TFunc), 1, arq) == 1) {
                        totalLidos++;
                        // Se for maior ou igual ao registro da partição, grava na partição normalmente
                        if (v[menor]->cod >= lastKey) {
                            // Caso a partição atual esteja cheia, abre a próxima partição
                            if (cursorPart >= M) {
                                fclose(particao);
                                particao = NULL;
                                cursorPart = 0;

                                if (*nParticoes >= 0) {
                                    char* nome = nome_part->nome;
                                    particao = fopen(nome, "wb+");
                                    nome_part = nome_part->prox;
                                    (*nParticoes)--; // Decrementa o número de partições restantes
                                } else {
                                    // printf("Todas as partições foram utilizadas.\n");
                                    return;
                                }
                            }
                            fseek(particao, cursorPart * sizeof(TFunc), SEEK_SET);
                            fwrite(v[menor], sizeof(TFunc), 1, particao);
                            controleVet[menor] = false;
                        }
                        // Se for menor do que o registro da partição, grava no reservatório e substitui pelo próximo
                        else {
                            fseek(res, cursorRes * sizeof(TFunc), SEEK_SET);
                            fwrite(v[menor], sizeof(TFunc), 1, res);
                            cursorRes++;
                            if (cursorRes == n) {
                                reservatorio_cheio = true;
                            }
                        }
                    }
                }
            }
            
        }
    }

    // Libera a memória alocada para os registros no vetor
    for (int i = 0; i < M; i++) {
        free(v[i]);
    }
    free(v);

    // Fecha os arquivos abertos
    fclose(res);
    if (particao != NULL) {
        fclose(particao);
    }
}

void intercalacao_otima(Lista *nomes_arquivos_entrada, FILE *arquivo_saida, int M) {

    FILE **arquivos_entrada = malloc(M * sizeof(FILE *));
    TFunc **registros = malloc(M * sizeof(TFunc *));
    int *proximos_indices = malloc(M * sizeof(int));
    bool *fim_arquivo = malloc(M * sizeof(bool));
    bool *arquivo_vazio = malloc(M * sizeof(bool));

    // Abrir todos os arquivos de entrada
    for (int i = 0; i < M; i++) {
        char nome_particao[20]; // Define um buffer para armazenar o nome do arquivo
        if(i==0){
            sprintf(nome_particao, "reservatorio.bin"); // Alterado para .bin
        }else{
        sprintf(nome_particao, "p%d.bin", i); // Alterado para .bin
        }
        arquivos_entrada[i] = fopen(nome_particao, "rb");
        if (arquivos_entrada[i] == NULL) {
            printf("Arquivo %s nao encontrado. Pulando para o proximo arquivo.\n", nome_particao);
            registros[i] = NULL; // Marca como NULL para indicar que não há registros neste arquivo
            fim_arquivo[i] = true; // Marca como fim de arquivo para evitar operações desnecessárias
            arquivo_vazio[i] = true; // Marca como vazio para evitar operações desnecessárias
            proximos_indices[i] = 0;
            continue; // Pula para a próxima iteração do loop
        }
        
        // Ler o primeiro registro de cada arquivo
        registros[i] = le_registro(arquivos_entrada[i]);
        fim_arquivo[i] = false;
        arquivo_vazio[i] = false;
        proximos_indices[i] = 0;
    }

    // Processo de intercalação ótima
    while (true) {
        // Encontrar o menor registro entre os registros atuais
        int menor_indice = -1;
        for (int i = 0; i < M; i++) {
            if (!fim_arquivo[i] && !arquivo_vazio[i] && (menor_indice == -1 || (registros[i] != NULL && registros[i]->cod < registros[menor_indice]->cod))) {
                menor_indice = i;
            }
        }

        // Se todos os arquivos de entrada estiverem no fim, sair do loop
        if (menor_indice == -1) {
            break;
        }

        // Escrever o menor registro no arquivo de saída
        escreve_registro(registros[menor_indice], arquivo_saida);

        // Ler o próximo registro do arquivo de onde veio o menor registro
        registros[menor_indice] = le_registro(arquivos_entrada[menor_indice]);
        if (registros[menor_indice] == NULL) {
            fim_arquivo[menor_indice] = true;
        }

        // Se o arquivo de onde veio o menor registro está vazio, indicar isso
        if (fim_arquivo[menor_indice] && proximos_indices[menor_indice] == nomes_arquivos_entrada[menor_indice].tamanho) {
            arquivo_vazio[menor_indice] = true;
        }

        // Se o arquivo de onde veio o menor registro não está no fim, avançar para o próximo índice
        if (!fim_arquivo[menor_indice]) {
            proximos_indices[menor_indice]++;
        }

        // Se o próximo índice alcançou o tamanho do arquivo de entrada, indicar fim de arquivo
        if (proximos_indices[menor_indice] == nomes_arquivos_entrada[menor_indice].tamanho) {
            fim_arquivo[menor_indice] = true;
        }
    }

    // Fechar todos os arquivos de entrada e o arquivo de saída
    for (int i = 0; i < M; i++) {
        fclose(arquivos_entrada[i]);
    }
    fclose(arquivo_saida);

    // Liberar a memória alocada
    free(arquivos_entrada);
    free(registros);
    free(proximos_indices);
    free(fim_arquivo);
    free(arquivo_vazio);
}