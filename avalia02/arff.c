#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arff.h"

//Função do A1 (com modificações para o atributo de categorias)
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
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < quantidade - 1) printf("------------------------------\n");
    }

    printf("===============================\n");
}

// Função do A1
int conta_atributos(FILE *arff) {
    int cont = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), arff) != NULL) { // lê uma linha do arquivo e armazena em buffer
        if (strstr(buffer, "@data") != NULL) { // para se a linha contém "@data"
            break;
        } else if (strstr(buffer, "@attribute") == buffer) {
            cont++;
        }
    }

    return cont;
}

// Recebe uma string com as categorias e atualiza o elemento com um vetor de strings 
void processa_categorias(atributo *elemento, char *categorias) {

}

// Função do A1 (com modificações para o atributo de categorias)
/*  Problema de tratamento de espaços em branco no T2 (gera null). 
    Não exibiu nenhuma mensagem de erro em T3 a T6(o processou como se fosse um arquivo bem formado). */
atributo* processa_atributos(FILE *arff, int quantidade) {
    atributo *infos = malloc(quantidade * sizeof(atributo));

    if (infos == NULL) {
        perror("Erro ao alocar memória para os atributos");
        return NULL;
    }

    char buffer[1024], rotulo[1024], tipo[1024], categorias[1024];

    for (int i = 0; i < quantidade; i++) {
        if (!fgets(buffer, sizeof(buffer), arff)) {
            perror("Erro ao ler atributo");
            free(infos);
            return NULL;
        }

        if (strstr(buffer, "@attribute") == buffer) { // executa se a linha contém "@attribute"
            if (sscanf(buffer, "@attribute %s %s", rotulo, tipo) >= 2) {
                infos[i].rotulo = strdup(rotulo);
                infos[i].tipo = strdup(tipo);

                if (strcmp(tipo, "numeric") != 0 && strcmp(tipo, "string") != 0) { // executa se o tipo não for "numeric" nem "string"
                    char *abre_chaves = strchr(buffer, '{'); // procura pela primeira ocorrência de "{"
                    
                    if (abre_chaves != NULL) {
                        char *fecha_chaves = strchr(abre_chaves, '}'); // procura pela primeira ocorrência de "}"
                        
                        if (fecha_chaves != NULL) {
                            *fecha_chaves = '\0';
                            infos[i].categorias = strdup(abre_chaves + 1);
                        }
                    }
                } else {
                    infos[i].categorias = NULL;
                }
            } else {
                continue;
            }
        }
    }

    return infos;
}

/*  Recebe um arquivo ARFF com um ponteiro de leitura posicionado antes de @data
    valida cada elemento em todas as linhas de dados em relação a um vetor de atributos
    fornecido como argumento. */
void valida_arff(FILE *arff, atributo *atributos, int quantidade) {

}