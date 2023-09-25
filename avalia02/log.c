#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arff.h"
#include "log.h"

// Gerar um relatório de todos os ataques ocorridos e o número de ocorrências no
// conjunto de dados (nome do arquivo de saída: "R_ATAQUES.txt"); 
void contabiliza_ataques(atributo *infos, int qtd_atrib_linha, FILE *arff) {
    char buffer[2048];
    int encontrou_data = 0, total_ataques = 0, testa_existencia = 0;

    rewind(arff);

    infos_ataque *novoAtk = (infos_ataque *) malloc(sizeof(infos_ataque));

    // Ler o arquivo até encontrar a linha "@data"
    while (fgets(buffer, sizeof(buffer), arff)) {
        if (!strcmp(buffer, "@data")) {
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
        // Verifica se a linha está em branco e a ignora
        if (buffer[0] == '\n') 
            continue;

        char *token = strtok(buffer, ",");

        // Loop para validar cada atributo na linha
        for (int i = 0; i < qtd_atrib_linha; i++) { 
            testa_existencia = 0;

            if (!strcmp(infos[i].rotulo, "PKT_CLASS")) { // se o atributo for o PKT_CLASS
                if (strcmp(token, "Normal") != 0) { // se o valor do PKT_CLASS for diferente de "Normal"
                    
                    for (int k = 0; k < total_ataques; k++) { // percorre a struct de ataques
                        if (!strcmp(infos[k].rotulo, token)) { // se o ataque já existe na struct
                            novoAtk[k].qtd_ataques++; // incrementa a quantidade de ataques
                            testa_existencia = 1;
                            break;
                        }
                    }
                    
                    if (!testa_existencia) { // se o ataque não existe na struct
                        total_ataques++; // incrementa a quantidade total de ataques
                        novoAtk[total_ataques].rotulo = strdup(token); // salva o nome do ataque
                        novoAtk[total_ataques].qtd_ataques++; // incrementa a quantidade de ataques
                    }
                }
            }
        }
    }

    // Criar o arquivo para escrita
    FILE *arquivo_ataque = fopen("R_ATAQUES.txt", "w");

    if (arquivo_ataque == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < total_ataques; i++)
        fprintf(arquivo_ataque, "%s;%d\n", novoAtk[i].rotulo, novoAtk[i].qtd_ataques);
    
    fclose(arquivo_ataque);
    printf("Dados gravados com sucesso no arquivo R_ATAQUES.txt.\n");

    return;
}

// Gerar um relatório dos endereços de origem maliciosos, potencialmente
// maliciosos e benignos (nome do arquivo de saída: "R_ENTIDADES.txt")
void contabiliza_enderecos();

// Gerar um relatório com a média da média do tamanho dos pacotes para cada
// tipo de ataque (nome do arquivo de saída: "R_TAMANHO.txt");

// ler, se for um dos valores esquisitos, salva o valor da média e a quantidade de vezes
void media_ataques(atributo *infos, int qtd_atrib_linha, FILE *arff) {
    char buffer[2048];
    int encontrou_data = 0, total_ataques = 0, testa_existencia = 0;
    double bufferMedia;

    rewind(arff);

    infos_medias_ataques *novoMedias = (infos_medias_ataques *) malloc(sizeof(infos_medias_ataques));

    // Ler o arquivo até encontrar a linha "@data"
    while (fgets(buffer, sizeof(buffer), arff)) {
        if (!strcmp(buffer, "@data")) {
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
        // Verifica se a linha está em branco e a ignora
        if (buffer[0] == '\n') 
            continue;

        char *token = strtok(buffer, ",");

        // Loop para validar cada atributo na linha
        for (int i = 0; i < qtd_atrib_linha; i++) { 
            testa_existencia = 0;

            if (!strcmp(infos[i].rotulo, "PKT_AVG_SIZE")) { // se o atributo for o PKT_AVG_SIZE
                bufferMedia = atof(token); // salva o valor da média

                if (strcmp(token, "Normal") != 0) { // se o valor do PKT_CLASS for diferente de "Normal"
                    
                    for (int k = 0; k < total_ataques; k++) { // percorre a struct de ataques
                        if (!strcmp(infos[k].rotulo, token)) { // se o ataque já existe na struct
                            novoMedias[k].qtd_ataques++; // incrementa a quantidade de ataques
                            novoMedias[k].media += bufferMedia; // incrementa a média
                            testa_existencia = 1;
                            break;
                        }
                    }
                    
                    if (!testa_existencia) { // se o ataque não existe na struct
                        total_ataques++; // incrementa a quantidade total de ataques
                        novoMedias[total_ataques].rotulo = strdup(token); // salva o nome do ataque
                        novoMedias[total_ataques].qtd_ataques++; // incrementa a quantidade de ataques
                        novoMedias[total_ataques].media += bufferMedia; // incrementa a média
                    }
                }
            }
        }
    }

    for (int i = 0; i < total_ataques; i++)
        novoMedias[i].media /= novoMedias[i].qtd_ataques;

    // Criar o arquivo para escrita
    FILE *arquivo_ataque = fopen("R_TAMANHO.txt", "w");

    if (arquivo_ataque == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < total_ataques; i++)
        fprintf(arquivo_ataque, "%s;%d\n", novoMedias[i].rotulo, novoMedias[i].media);
    
    fclose(arquivo_ataque);
    printf("Dados gravados com sucesso no arquivo R_TAMANHO.txt.\n");

    return;
}

// Gerar uma lista negra (blacklist) de endereços de origem considerados
// maliciosos (nome do arquivo de saída: "BLACKLIST.bl").
void gera_blacklist();