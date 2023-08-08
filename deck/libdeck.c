/* do a list struct where you just push/pop at the start and end of it */

/* TO-DO

    - the function to add values should add at the end or at the start of the list,
      this specific concept is missing, i'm adding only in the end of the list

*/

#include <stdio.h>
#include <stdlib.h>
#include "libdeck.h"

/* 
 * Cria uma Lista vazia 
*/
lista_t *lista_cria () {
    lista_t *nova_lista = malloc(sizeof(lista_t));

    if (nova_lista == NULL)
        return NULL;

    nova_lista->ini = NULL;
    nova_lista->fim = NULL;
    nova_lista->tam = 0;

    return nova_lista;
}

/* 
 * Adiciona um elemento em ordem de acordo com o valor elemento->chave na Lista. 
 * Retorna 1 em caso de sucesso e 0 caso contrario.
*/
int lista_insere(lista_t *l, int valor) {
    if (lista_vazia(l))
        return 0;
    
    nodo_t *novo = malloc(sizeof(nodo_t));
    novo->num = valor;
    novo->ant = NULL;
    novo->prox = NULL;

    if (novo == NULL)
        return 0;

    /* caso lista vazia */
    if (l->ini == NULL) {
        l->ini = novo;
        l->fim = novo;
        l->tam++;
    
        return 1;
    }

    int opcao;
    printf("Deseja inserir a carta no início ou fim? (0 para início, 1 para fim)\n");
    scanf("%d", &opcao);

    if (opcao) {
        l->fim->prox = novo;
        novo->ant = l->fim;
        l->fim = novo;
        l->tam++;

        return 1;
    }

    l->ini->ant = novo;
    novo->prox = l->ini;
    l->ini = novo;
    l->tam++;

    return 1;
}

int testa_palindromo (lista_t *l) {
    if (lista_vazia(l))
        return 0;
    
    if (l->ini == NULL)
        return 0;
    
    if (l->fim == NULL)
        return 0;
    
    int tam_lista = l->tam;
    nodo_t *aux1 = l->ini;
    nodo_t *aux2 = l->fim;

    while (tam_lista > 0) {
        if (aux1->num != aux2->num)
            return 0;
            
        aux1 = aux1->prox;
        aux2 = aux2->ant;

        tam_lista = tam_lista - 2;
    }

    return 1;
}

int lista_vazia(lista_t *l) {
    if (l == NULL)
        return 1;
    return 0;
}