#include <stdio.h>
#include <time.h>
#include "funcionario.h"
#include "buscaBinaria.h"
#include "buscaSequencial.h"
#include "ordenacao.h"
#include "selecaoNatural.h"
#include "lista.h"
#include "arvoreb.h"

void press_enter_to_continue() {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n'); // Aguarda até que o usuário pressione Enter
}

int main() {

    int tam_base;
    printf("Entre com o tamanho da base de dados que voce deseja criar de funcionarios: ");
    scanf("%d",&tam_base);

    // Criar e abrir um arquivo para a base de dados
    FILE *base_de_dados = fopen("base_de_dados.bin", "wb+");
    if (base_de_dados == NULL) {
        perror("Erro ao abrir arquivo de base de dados");
        return 1;
    }

    // Criar uma base de dados fictícia
    criarBase(base_de_dados, tam_base);

    // Imprimir a base de dados antes de qualquer operação de busca ou ordenação
    printf("Base de dados antes de qualquer operacao:\n");

    press_enter_to_continue();

    imprimirBase(base_de_dados);

    press_enter_to_continue();

    printf("\n\n ***** Demonstracao de busca sequencial e binaria em uma base ordenada de dados *****\n");

    press_enter_to_continue();

    // Testar busca sequencial    
    printf("\n**********************\n");
    int chave_sequencial = 5;
    printf("\nBusca sequencial pelo funcionario com codigo %d:\n", chave_sequencial);
    fseek(base_de_dados, 0, SEEK_SET);
    TFunc *resultado_sequencial = buscaSequencial(chave_sequencial, base_de_dados);
    if (resultado_sequencial != NULL) {
        imprime(resultado_sequencial);
    }
    
    press_enter_to_continue();

    // Testar busca binária
    printf("\n**********************\n");
    int chave_binaria = 7; // Supondo que a base de dados esteja ordenada por código
    printf("\nBusca binaria pelo funcionario com codigo %d:\n", chave_binaria);
    fseek(base_de_dados, 0, SEEK_SET);
    TFunc *resultado_binario = busca_binaria(chave_binaria,base_de_dados,tam_base);
    if (resultado_binario != NULL) {
        imprime(resultado_binario);
    }

    press_enter_to_continue();

    // Embaralha a base de dados
    embaralhar_base(base_de_dados);

    // Imprimir a base de dados depois de embaralhar
    printf("\n\n **************************** Base de dados embaralhada **************************** \n");

    press_enter_to_continue();

    imprimirBase(base_de_dados);

    press_enter_to_continue();

     printf("\n\n ***** Demonstracao de busca sequencial e binaria em uma base desordenada de dados *****\n");

     press_enter_to_continue();

    // Testar busca sequencial
    printf("\n**********************\n");
    chave_sequencial = 2;
    printf("\nBusca sequencial pelo funcionario com codigo %d:\n", chave_sequencial);
    fseek(base_de_dados, 0, SEEK_SET);
    resultado_sequencial = buscaSequencial(chave_sequencial, base_de_dados);
    if (resultado_sequencial != NULL) {
        imprime(resultado_sequencial);
    }

    press_enter_to_continue();

    printf("\n\n *************************************** Teste de funcionalidade de ordenação insertion sort em disco ***************************************\n");

    press_enter_to_continue();

    // Imprimir a base de dados depois de embaralhar
    printf("\n\n **************************** Base de dados embaralhada **************************** \n");

    press_enter_to_continue();

    imprimirBase(base_de_dados);

    press_enter_to_continue();

    fseek(base_de_dados, 0, SEEK_SET);
    insertion_sort_disco(base_de_dados, tam_base);

    // Imprimir a base de dados ordenada
    printf("\n@@@@@@@@@@@@@ Base de dados pos merge sort @@@@@@@@@@@@@\n");

    press_enter_to_continue();

    imprimirBase(base_de_dados);

    press_enter_to_continue();

    // Embaralha a base de dados
    embaralhar_base(base_de_dados);

    // Imprimir a base de dados depois de embaralhar
    printf("\n\n **************************** Base de dados embaralhada **************************** \n");
    imprimirBase(base_de_dados);

    press_enter_to_continue();

    printf("\n\n *************************************** Teste de funcionalidade de ordenação selecao natural ***************************************\n");

    press_enter_to_continue();

    fseek(base_de_dados, 0, SEEK_SET);


    Lista *nomes = cria("p1.bin", cria("p2.bin", cria("p3.bin", cria("p4.bin", cria("p5.bin", cria("p6.bin", cria("p7.bin", cria("p8.bin", cria("p9.bin", cria("p10.bin", NULL))))))))));
    int M = 10; // Tamanho do vetor para seleção
    int n = 5; // Capacidade do reservatório
    int nParticoes = 10; // Número de partições
    

    // Teste da função selecao_natural
    fseek(base_de_dados, 0, SEEK_SET);
    selecao_natural(base_de_dados, nomes, M, tam_base, n, &nParticoes);

    press_enter_to_continue();

    // printf("\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$ particao 1 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    // FILE *part1 = fopen("p1.bin", "rb");
    // if (base_de_dados == NULL) {
    //     perror("Erro ao abrir arquivo de base de dados");
    //     return 1;
    // }
    // imprimirBase(part1);

    // printf("\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$ particao 2 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    // FILE *part2 = fopen("p2.bin", "rb");
    // if (base_de_dados == NULL) {
    //     perror("Erro ao abrir arquivo de base de dados");
    //     return 1;
    // }
    // imprimirBase(part2);

    // printf("\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$ particao 3 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    // FILE *part3 = fopen("p3.bin", "rb");
    // if (base_de_dados == NULL) {
    //     perror("Erro ao abrir arquivo de base de dados");
    //     return 1;
    // }
    // imprimirBase(part3);


    // Criar e abrir um arquivo para a base de dados
    FILE *base_de_dados_intercalado = fopen("base_de_dados_intercalado.bin", "wb+");
    if (base_de_dados_intercalado == NULL) {
        perror("Erro ao abrir arquivo de base de dados intercalado");
        exit(EXIT_FAILURE);
    }

    // Chamar a função de intercalação ótima
    intercalacao_otima(nomes, base_de_dados_intercalado, nParticoes);
    fclose(base_de_dados_intercalado);

    FILE *base_de_dados_intercalado_leitura = fopen("base_de_dados_intercalado.bin", "rb");
    if (base_de_dados_intercalado == NULL) {
        perror("Erro ao abrir arquivo de base de dados intercalado");
        exit(EXIT_FAILURE);
    }

     // Imprimir a base de dados ordenada
    printf("\n@@@@@@@@@@@@@ Base de dados pos selecao natural e intercalacao otima @@@@@@@@@@@@@\n");

    press_enter_to_continue();

    fseek(base_de_dados_intercalado, 0, SEEK_SET);
    imprimirBase(base_de_dados_intercalado_leitura);

    press_enter_to_continue();



    fclose(base_de_dados);
    fclose(base_de_dados_intercalado_leitura);

    press_enter_to_continue();
    printf("\n\n *************************************** Teste de funcionalidade da arvore B ***************************************\n");


    FILE *arquivo = fopen("base_de_dados.bin", "rb"); // Abre o arquivo binário para leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    TreeNode *root = NULL;

    // Lógica para ler dados do arquivo e criar a árvore
    // Exemplo de leitura de TFuncs do arquivo e inserção na árvore
    TFunc funcionarios[tam_base]; // Supondo que MAX_FUNCIONARIOS seja o número máximo de funcionários no arquivo

    int g = 0; // Variável para manter o número atual de funcionários lidos

    while (fread(&funcionarios[g], sizeof(TFunc), 1, arquivo) == 1) {
        g++;
        if (g == tam_base) {
            printf("Número máximo de funcionários atingido.\n");
            break;
        }
    }


    // Fecha o arquivo após a leitura
    fclose(arquivo);

    // Cria a árvore com os funcionários lidos do arquivo
    create_tree_with_func(funcionarios, &root, tam_base);

    press_enter_to_continue();

    // Teste de busca na árvore
    int codigo_para_buscar = 23; // Código a ser buscado
    search_node(root, codigo_para_buscar);

    press_enter_to_continue();

    // Teste de remoção de um nó da árvore
    int codigo_para_remover = 6; // Código a ser removido
    remove_node(root, codigo_para_remover);

    press_enter_to_continue();

    // Teste de impressão da árvore
    printf("Árvore B:\n");
    print_tree(root);

    press_enter_to_continue();
    return 0;
}
