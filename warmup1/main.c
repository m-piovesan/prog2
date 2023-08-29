#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct organizaPoema {
    int posicao;
    char letra;
} organizaPoema_t;

int main() {
    FILE *file = fopen("poema.bin", "rb+");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fseek(file, 0, SEEK_END); // Move o cursor para o final do arquivo
    long file_size = ftell(file); // Obtém o tamanho total do arquivo
    int totalC = file_size / (sizeof(char) + sizeof(int));

    char *novaString = malloc(totalC + 1);
    organizaPoema_t aux[totalC];

    rewind(file);
    
    fread(aux, sizeof(organizaPoema_t), totalC, file);
    fclose(file);

    printf("Número de bytes no arquivo: %ld\n", file_size);
    printf("Número de caracteres no arquivo: %d\n", totalC);

    for (int i = 0; i < totalC; i++) {
        int posicao = aux[i].posicao;
        char letra = aux[i].letra;

        novaString[posicao] = letra;
    }

    novaString[totalC] = '\0'; // Adiciona o caractere nulo para terminar a string

    printf("Poema: %s\n", novaString);

    free(novaString);

    return 0;
}