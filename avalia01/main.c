// ./a.out -i TestesA1/car.arff -p

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char *rotulo;
    char *tipo;
    char *categorias;
} atributo;

void exibe_atributos(atributo *infos, int tamanho) {
    if (infos == NULL) {
        printf("O arquivo ARFF fornecido é inválido!\n");
        exit(0);
    }

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("-> Atributo #%d\n", i + 1);
        printf("Rótulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < tamanho - 1) printf("------------------------------\n");
    }
    printf("===============================\n");
}
#include <stdio.h>
#include <string.h>

int conta_atributos(FILE *arff) {
    int cont = 0;
    int testaFim = 0;
    int contaEspacos = 0;
    char buffer[1024];
    char *posicao;

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
    
    if(!testaFim) {
        fprintf(stderr, "Erro: Arquivo ARFF inválido.\n");
        return -1;
    }

    return cont;
}

/*  Problema de tratamento de espaços em branco no T2 (gera null). 
    Não exibiu nenhuma mensagem de erro em T3 a T6(o processou como se fosse um arquivo bem formado). */
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

int main(int argc, char **argv) {
    int opt;
    char exibicao = 0;
    char *entrada = NULL;

    while ((opt = getopt(argc, argv, "pi:")) != -1) {
        switch (opt) {
            case 'i':
                entrada = strdup(optarg);
                break;
            case 'p':
                exibicao = 1;
                break;
            default:
                fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
                exit(1);
        }
    }

    if (entrada == NULL) {
        fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
        exit(2);
    }

    FILE *arff = fopen(entrada, "r+");

    if (arff == NULL) {
        perror("Erro ao abrir o arquivo");
        return 3;
    }

    int tamanho = conta_atributos(arff);
    fseek(arff, 0, SEEK_SET);

    atributo *infos = processa_atributos(arff, tamanho);

    if (infos == NULL) {
        return 4;
    }

    if (exibicao) {
        exibe_atributos(infos, tamanho);
    }

    for (int i = 0; i < tamanho; i++) {
        free(infos[i].rotulo);
        free(infos[i].tipo);
        if (infos[i].categorias) {
            free(infos[i].categorias);
        }
    }

    free(infos);
    fclose(arff);

    return 0;
}