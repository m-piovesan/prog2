typedef struct nodo {
    struct nodo *prox;
    int idade;
    int tipo;
    char *titulo;
    char *texto;
} nodo_t;

typedef struct lista {
    int tam;
    nodo_t *ini;
} lista_t;

/* Cria uma Lista vazia */
lista_t *lista_cria ();

/* Insere elemento na lista */
int lista_insere(lista_t *l, char *titulo, char *texto);

/* Remove a notícia da lita (caso tenha mais de 3 dias) */
int lista_remove(lista_t *l);

/* retorna 1 se a lista estiver vazia e 0 se não estiver */
int lista_vazia(lista_t *l);

/* função pra ler as notícias */
void requisita(char *titulo, char *texto);

/* função para envelhecer noticias */
void envelhece_noticias(lista_t *l);

/* função para fechar a edição do dia */
void fechamento_edicao(lista_t *listaBN, lista_t *listaInf);

/* função que destroi a lista */
void lista_destroi(lista_t *l);