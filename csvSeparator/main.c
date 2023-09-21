#include <stdio.h>
#include <string.h>

int main() {
    char csv_line[1000]; // Defina um tamanho adequado para a linha CSV
    printf("Digite uma linha CSV: ");
    fgets(csv_line, sizeof(csv_line), stdin); // Lê a linha do teclado

    int column_number = 1; // Inicializa o número da coluna
    char *token = strtok(csv_line, ","); // Divide a linha usando vírgulas como delimitador

    while (token != NULL) {
        printf("Coluna %d: %s\n", column_number, token);
        token = strtok(NULL, ","); // Continua a divisão para a próxima coluna
        column_number++; // Incrementa o número da coluna
    }

    return 0;
}