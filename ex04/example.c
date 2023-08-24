#include <stdio.h>

int main() {
    FILE *arquivo;
    char byte_lido, byte_escrito;

    arquivo = fopen("arquivo.txt", "r+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    byte_lido = fgetc(arquivo);
    printf("Byte lido: %c\n", byte_lido);
    
    byte_escrito = 'A';
    fputc(byte_escrito, arquivo);
    printf("Byte escrito: %c\n", byte_escrito);
    
    fclose(arquivo);
    return 0;
}