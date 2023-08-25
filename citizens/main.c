#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[10 + 1];   // Identificador único
    int hacking;   // Nível de habilidade em hacking (0-100)
    int crime;     // Nível de criminalidade (0-10)
} citizen_t;

int main() {
    FILE *file = fopen("citizens.bin", "rb"); // Abre o arquivo binário para leitura
    
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fseek(file, 0, SEEK_END); // Move o cursor para o final do arquivo
    long file_size = ftell(file); // Obtém o tamanho total do arquivo
    int num_citizens = file_size / sizeof(citizen_t); // Calcula o número de cidadãos no arquivo

    printf("Número de bytes no arquivo: %ld\n", file_size);

    rewind(file); // Retorna o cursor para o início do arquivo

    citizen_t citizens[num_citizens]; // Cria um array para armazenar os cidadãos

    fread(citizens, sizeof(citizen_t), num_citizens, file); // Lê os dados dos cidadãos no arquivo

    fclose(file); // Fecha o arquivo

    // Exibe as informações solicitadas
    printf("Quantidade total de cidadãos: %d\n", num_citizens);

    printf("Identificadores e níveis de hacking dos cidadãos:\n");
    for (int i = 0; i < num_citizens; i++) {
        printf("Cidadão %d: ID = %s, Hacking = %d\n", i + 1, citizens[i].id, citizens[i].hacking);
    }

    int sum_hacking = 0;
    int num_above_5 = 0;

    for (int i = 0; i < num_citizens; i++) {
        sum_hacking += citizens[i].hacking;
        if (citizens[i].crime > 5) {
            num_above_5++;
        }
    }

    float avg_hacking = (float)sum_hacking / num_citizens;

    printf("Média dos níveis de hacking: %.2f\n", avg_hacking);
    printf("Número de cidadãos com níveis de criminalidade acima de 5: %d\n", num_above_5);

    return 0;
}