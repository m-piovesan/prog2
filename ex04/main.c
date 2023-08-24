#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char buffer[101], *campos[3], *mr_produto = 0, *mv_produto = 0;
    int qntd, m_venda = -1;
    float valor, faturamento = 0, m_receita = -1;

    FILE *arquivo = fopen("tabela.csv", "r+"); // Abrindo o arquivo para leitura e escrita

    if (!arquivo) {
        printf("O arquivo não pode ser aberto!");
        return 1;
    }

    // Lendo o cabeçalho do arquivo e descartando o caractere de nova linha (\n)
    fscanf(arquivo, "%100[^\n]", buffer);
    fgetc(arquivo);

    while (!feof(arquivo)) {
        fscanf(arquivo, "%100[^\n]", buffer);
        fgetc(arquivo);

        // Separando os campos da linha usando o delimitador "|"
        campos[0] = strtok(buffer, "|");
        for (int i = 0; i < 2; i++) {
            campos[i + 1] = strtok(NULL, "|"); // O segundo parâmetro é NULL para continuar a partir da última posição
        }

        // Convertendo valores dos campos para os tipos apropriados (float e int)
        sscanf(campos[1], "%f", &valor);
        sscanf(campos[2], "%d", &qntd);

        // Calculando o faturamento total
        faturamento += valor * qntd;

        // Verificando a maior venda
        if (qntd > m_venda) {
            if (!mv_produto) free(mv_produto);
            m_venda = qntd;
            mv_produto = strdup(campos[0]);
        }

        // Verificando a maior receita
        if (qntd * valor > m_receita) {
            if (!mr_produto) free(mr_produto);
            m_receita = qntd * valor;
            mr_produto = strdup(campos[0]);
        }
    }

    // Exibindo os resultados
    printf("Faturamento total: %f\n", faturamento);
    printf("Maior receita: %s\n", mr_produto);
    printf("Maior venda: %s\n", mv_produto);

    // Liberando memória alocada
    if (mr_produto) free(mr_produto);
    if (mv_produto) free(mv_produto);

    // Fechando o arquivo
    fclose(arquivo);

    return 0;
}