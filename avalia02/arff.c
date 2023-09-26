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

// Função que recebe um arquivo arff e conta as linhas (atributos) válidas
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
    int i = 0;
    char buffer[1024];

    if (elemento == NULL || categorias == NULL) {
        perror("Erro: elemento vazio!\n");
        return;
    }

    strcpy(buffer, categorias); 
    elemento->quantidade_categorias = contar_valores(categorias);
    elemento->categorias = malloc(elemento->quantidade_categorias * sizeof(char *));

    if (elemento->categorias == NULL) {
        perror("Erro ao alocar memória para categorias!\n");
        return;
    }
    
    char *token = strtok(buffer, ",");

    while (token != NULL) {
        elemento->categorias[i] = strdup(token); // Aloca memória para a categoria e copia o token    
        if (elemento->categorias[i] == NULL) {
            perror("Erro ao alocar memória para categoria!\n");
            return;
        }        

        token = strtok(NULL, ",");
        i++;
    }

    return;
}

// Função do A1 (com modificações para o atributo de categorias)
atributo* processa_atributos(FILE *arff, int tamanho) {
    atributo *infos = malloc(tamanho * sizeof(atributo));

    if (infos == NULL) {
        perror("Erro ao alocar memória para os atributos\n");
        return NULL;
    }

    char buffer[1024], rotulo[1024], tipo[1024], testaErro[1024];

    rewind(arff);

    for (int i = 0; i < tamanho; i++) {
        if (!fgets(buffer, sizeof(buffer), arff)) {
            printf("Erro ao ler atributo\n");
            free(infos);
            break;
        }

        int linha_em_branco = 1;

        if (buffer[0] != '\n') 
            linha_em_branco = 0;
        
        if (linha_em_branco) {
            fprintf(stderr, "Erro: Linha em branco encontrada.\n");
            i--; // Volte uma posição no loop para processar o próximo atributo
            continue;
        }

        if ((sscanf(buffer, "@attribute %s %s %s", rotulo, tipo, testaErro)) == 2) {
            infos[i].rotulo = strdup(rotulo);
            infos[i].tipo = strdup(tipo);

            if (strcmp(tipo, "numeric") != 0 && strcmp(tipo, "string") != 0) {
                char *abre_chaves = strchr(buffer, '{');

                if (abre_chaves != NULL) {
                    char *fecha_chaves = strchr(abre_chaves, '}');

                    if (fecha_chaves != NULL) {
                        *fecha_chaves = '\0';
                        char *categorias = strdup(abre_chaves + 1);

                        processa_categorias(&infos[i], categorias);
                        free(categorias); // Liberar a memória alocada para categorias após o processamento
                    }
                }

                infos[i].tipo = strdup("categoric");
            } else 
                infos[i].categorias = NULL;
        } else {
            return NULL;
        }
    }

    return infos;
}

/*  Recebe um arquivo ARFF com um ponteiro de leitura posicionado antes de @data
    valida cada elemento em todas as linhas de dados em relação a um vetor de atributos
    fornecido como argumento. */
void valida_arff(FILE *arff, atributo *atributos, int quantidade) {
    char buffer[2048];
    int linha = 0;
    int encontrou_data = 0;

    // Ler o arquivo até encontrar a linha "@data"
    while (fgets(buffer, sizeof(buffer), arff)) {
        if (strstr(buffer, "@data") != NULL) { // para se a linha contém "@data"
            encontrou_data = 1;
            break;
        }
    }

    if (!encontrou_data) {
        printf("Erro: Arquivo com formato inválido!\n");
        return;
    }

    // Lê cada linha de dados do arquivo
    while (fgets(buffer, sizeof(buffer), arff) != NULL) {
        linha++;

        // Verifica se a linha está em branco e a ignora
        if (buffer[0] == '\n') continue;

        char *token = strtok(buffer, ",");

        // Loop para validar cada atributo na linha
        for (int i = 0; i < quantidade; i++) {
            // Verifica se não há tokens suficientes na linha
            if (token == NULL) {
                fprintf(stderr, "Erro: Linha %d: Quantidade de atributos inválida.\n", linha);
                return;
            }

            // Verifica se o tipo de atributo é "numeric"
            if (strstr(atributos[i].tipo, "numeric") != NULL) {
                char *endptr;
                long number = strtol(token, &endptr, 10); // Converte a string para um número

                // Verifica se o token não é um número válido ou se não é um número inteiro (erro se for)
                if ((token == endptr)) {
                    fprintf(stderr, "Erro: valor inválido!\n");
                    return;
                }
            }
            // Verifica se o tipo de atributo é "categoric"
            else if (strstr(atributos[i].tipo, "categoric") != NULL) {
                int j;
                
                // Loop para verificar se o token corresponde a uma categoria válida
                for (j = 0; j < atributos[i].quantidade_categorias; j++)
                    if (strstr(token, atributos[i].categorias[j]) != NULL) break;
                
                // Se o loop terminar sem encontrar uma correspondência, é relatado um erro
                if (j == atributos[i].quantidade_categorias) {
                    fprintf(stderr, "Erro: Linha %d: Valor inválido para o atributo %s.\n", linha, atributos[i].rotulo);
                    return;
                }
            }

            // Avança para o próximo token
            token = strtok(NULL, ",");
        }

        // Verifica se ainda há tokens não consumidos na linha (erro se houver)
        if (token != NULL) {
            fprintf(stderr, "Erro: Linha %d: Quantidade de atributos inválida.\n", linha);
            return;
        }
    }

    printf("Arquivo ARFF válido!\n");
}

// Função para contar quantos valores estão no buffer (separados por vírgula)
int contar_valores(char *buffer) {
    int cont = 0;
    char *token = strtok(buffer, ",");
    
    while (token != NULL) {
        cont++;
        token = strtok(NULL, ",");
    }

    return cont;
}
