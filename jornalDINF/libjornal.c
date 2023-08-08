#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libjornal.h"

// [X]
/* Cria uma lista vazia */
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
 * Adiciona uma nova notícia na lista. 
 * Retorna 1 em caso de sucesso e 0 caso contrario.
*/
int lista_insere(lista_t *l, char *titulo, char *texto) {
    if (lista_vazia(l))
        return 0;
    
    nodo_t *novo = malloc(sizeof(nodo_t));

    if (novo == NULL)
        return 0;

    novo->titulo = malloc(strlen(titulo) +1);
    novo->texto = malloc(strlen(texto) +1);

    if (novo->titulo == NULL || novo->texto == NULL)
        return 0;

    strcpy(novo->titulo, titulo);
    strcpy(novo->texto, texto);
    novo->idade = 0;
    novo->prox = NULL;

    /* caso lista vazia */
    if (l->ini == NULL) {
        l->ini = novo;
        l->fim = novo;
        l->tam++;
    
        return 1;
    }

    l->fim->prox = novo;
    l->fim = novo;
    l->tam++;

    return 1;
}

int lista_remove(lista_t *l) {
    if (lista_vazia(l))
        return 0;

    nodo_t *aux = l->ini;

    /* caso primeira notícia seja inválida (mais de 3 dias) */
    if (l->ini->idade > 3) {
        l->ini = l->ini->prox;

        free(aux->titulo);
        free(aux->texto);
        free(aux);
        
        l->tam--;
    }

    while (aux != NULL) {
        if (aux->idade > 3) {
            nodo_t *armazena = aux->prox;

            free(aux->titulo);
            free(aux->texto);
            free(aux);

            aux = armazena;
            l->tam--;            
        }

        aux = aux->prox;
    }

    return 1;
}

void envelhece_noticias(lista_t *l) {
    if (lista_vazia(l))
        return;

    nodo_t *aux = l->ini;

    while (aux != NULL) {
        aux->idade++;
        aux = aux->prox;
    }
}

int lista_vazia(lista_t *l) {
    if (l == NULL)
        return 1;
    return 0;
}

void requisita(char *titulo, char *texto) {
    getchar();
    printf("\nDigite o título: ");
    fgets(titulo, 33, stdin);
    printf("\nDigite o texto: ");
    fgets(texto, 513, stdin);
}

void fechamento_edicao(lista_t *listaBN, lista_t *listaInf) {
    if (listaBN->tam + listaInf->tam <= 0) {
        printf("Essa edição foi pulada por falta de notícias!\n");
        return;
    }

    if (listaBN->tam >= 2) {
        printf("\n%s", listaBN->ini->titulo);
        printf("\n%s\n", listaBN->ini->texto);

        printf("%s", listaBN->ini->prox->titulo);
        printf("\n%s\n", listaBN->ini->prox->texto);

        /* determinar como idade > 3 para que sejam limpas na próxima edição, visto que já foram publicadas */
        listaBN->ini->idade = 4;
        listaBN->ini->prox->idade = 4;

        envelhece_noticias(listaBN);
        envelhece_noticias(listaInf);

        lista_remove(listaBN);
        lista_remove(listaInf);

        return;
    }

    if (listaBN->tam == 1) {
        printf("\n%s", listaBN->ini->titulo);
        printf("\n%s\n", listaBN->ini->texto);

        printf("%s", listaInf->ini->titulo);
        printf("\n%s\n", listaInf->ini->texto);

        listaBN->ini->idade = 4;
        listaInf->ini->idade = 4;

        envelhece_noticias(listaBN);
        envelhece_noticias(listaInf);

        lista_remove(listaBN);
        lista_remove(listaInf);

        return;
    }

    /* if (listaBN->tam == 0) */
    printf("\n%s", listaInf->ini->titulo);
    printf("\n%s\n", listaInf->ini->texto);

    printf("%s", listaInf->ini->prox->titulo);
    printf("\n%s\n", listaInf->ini->prox->texto);

    listaInf->ini->idade = 4;
    listaInf->ini->prox->idade = 4;

    envelhece_noticias(listaBN);
    envelhece_noticias(listaInf);

    lista_remove(listaBN);
    lista_remove(listaInf);

    return;
}

void lista_destroi(lista_t *l) {
    if (lista_vazia(l))
        return;

    nodo_t *aux = l->ini;

    while (aux != NULL) {
        nodo_t *armazena = aux->prox;

        free(aux->titulo);
        free(aux->texto);
        free(aux);

        aux = armazena;
    }

    free(l);
}