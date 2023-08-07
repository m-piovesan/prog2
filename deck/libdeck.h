typedef struct nodo {
    int num;
    struct nodo *ant;
    struct nodo *prox;
} nodo_t;

typedef struct lista {
    int tam;
    nodo_t *ini;
    nodo_t *fim;
} lista_t;

/* Cria uma Lista vazia */
lista_t *lista_cria ();

/* Insere elemento na lista */
int lista_insere (lista_t *l);

/* 
 * Avança um elemento da lista (usar pro começo)
 * Retorna 1 em caso de sucesso e 0 cc.
*/
int prox_elemento (lista_t *l);

/* 
 * Retorna um elemento da lista (usar pro final)
 * Retorna 1 em caso de sucesso e 0 cc.
*/
int ant_elemento (lista_t *l);

/* Testa se o começo e fim são iguais */
int testa_palindromo (lista_t *l);

/* retorna 1 se a lista estiver vazia e 0 se não estiver */
int lista_vazia(lista_t *l);