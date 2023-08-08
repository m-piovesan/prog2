#include <stdio.h>
#include <stdlib.h>
#include "libdeck.h"

int main () {
    lista_t *l = lista_cria();
    int n;

    printf("Quantos cartas você deseja ter no baralho?\n");
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        int carta;
        printf("Digite o número da carta:\n");
        scanf("%d", &carta);
        lista_insere(l, carta);
    }

    if(testa_palindromo(l)) 
        printf("É palíndromo!\n");
    else 
        printf("Não é palíndromo!\n");

    return 0;
}