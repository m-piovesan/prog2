#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arff.h"

// Função do A1 (com modificações para o atributo de categorias)
void exibe_atributos(atributo *infos, int quantidade) {
    if (infos == NULL) {
        printf("O arquivo ARFF fornecido é inválido!\n");
        exit(0);
    }

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for (int i = 0; i < quantidade; i++) {
        printf("-> Atributo #%d\n", i + 1);
        printf("Rótulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        
        if (infos[i].categorias) {
            printf("Categorias: ");
            
            for (int j = 0; j < infos[i].quantidade_categorias; j++) {
                printf("%s", infos[i].categorias[j]);
                if (j < infos[i].quantidade_categorias - 1) printf(", ");
            }

            printf("\n");
        }
        
        if (i < quantidade - 1) printf("------------------------------\n");
    }

    printf("===============================\n");
}

// Função do A1
int conta_atributos(FILE *arff) {
    int cont, testaFim, contaEspacos;
    char buffer[1024];
    char *posicao;

    cont = testaFim = 0;

    while (fgets(buffer, sizeof(buffer), arff) != NULL) { // lê uma linha do arquivo e armazena em buffer
        if (strstr(buffer, "@data") != NULL) { // para se a linha contém "@data"
            testaFim = 1;
            break;
        } else if (strstr(buffer, "@attribute") == buffer) {
            contaEspacos = 0;
            
            posicao = strchr(buffer, ' '); // Encontra a primeira ocorrência do espaço

            while (posicao != NULL) {
                posicao = strchr(posicao + 1, ' '); // Encontra a próxima ocorrência do espaço
                contaEspacos++;
            }

            if (contaEspacos == 2) 
                cont++;
        }
    }
    
    if (!testaFim) {
        fprintf(stderr, "Erro: Arquivo ARFF inválido.\n");
        return -1;
    }

    return cont;
}

// Recebe uma string com as categorias e atualiza o elemento com um vetor de strings 
void processa_categorias(atributo *elemento, char *categorias) {
    char buffer[1025];
    int cont = 0;

    strcpy(buffer, categorias);
    char *token = strtok(buffer, ",");

    while (token != NULL) {
        elemento->categorias[cont] = strdup(token);
        token = strtok(NULL, ",");
        cont++;
    }

    elemento->quantidade_categorias = cont;

    return;
}

// Função do A1 (com modificações para o atributo de categorias)
atributo* processa_atributos(FILE *arff, int tamanho) {
    atributo *infos = malloc(tamanho * sizeof(atributo));

    if (infos == NULL) {
        perror("Erro ao alocar memória para os atributos");
        return NULL;
    }

    char buffer[1024], rotulo[1024], tipo[1024], categorias[1024], testaErro[1024];

    for (int i = 0; i < tamanho; i++) {
        if (!fgets(buffer, sizeof(buffer), arff)) {
            printf("Erro ao ler atributo");
            free(infos);
            break;
        }

        int linha_em_branco = 1;

        if(buffer[0] != '\n') 
            linha_em_branco = 0;
        
        if (linha_em_branco) {
            fprintf(stderr, "Erro: Linha em branco encontrada.\n");
            i--; // Volte uma posição no loop para processar o próximo atributo
            continue;
        }

        if ((sscanf(buffer, "@attribute %s %s %s", rotulo, tipo, testaErro)) == 2) {
            infos[i].rotulo = strdup(rotulo);
            infos[i].tipo = strdup(tipo);

            if (strcmp(tipo, "numeric") != 0 && strcmp(tipo, "string") != 0) { // executa se o tipo não for "numeric" nem "string"
                char *abre_chaves = strchr(buffer, '{'); // procura pela primeira ocorrência de "{"
                infos[i].tipo = strdup("categoric");

                if (abre_chaves != NULL) {
                    char *fecha_chaves = strchr(abre_chaves, '}'); // procura pela primeira ocorrência de "}"
                    
                    if (fecha_chaves != NULL) {
                        *fecha_chaves = '\0';
                        infos[i].categorias = strdup(abre_chaves + 1);
                    }
                }

                processa_categorias(&infos[i], infos[i].categorias); // atualiza o elemento com um vetor de strings
            } else 
                infos[i].categorias = NULL;
        } else {
            fprintf(stderr, "Erro: Formato inválido para o atributo\n");
            i--; // Volte uma posição no loop para processar o próximo atributo
            continue;
        }
    }

    return infos;
}

/*  Recebe um arquivo ARFF com um ponteiro de leitura posicionado antes de @data
    valida cada elemento em todas as linhas de dados em relação a um vetor de atributos
    fornecido como argumento. */
void valida_arff(FILE *arff, atributo *atributos, int quantidade) {

}