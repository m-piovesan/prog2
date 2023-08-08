#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libjornal.h"

int main () {
    lista_t *listaBN = lista_cria();
    lista_t *listaInf = lista_cria();

    int opcao = 10;
    int tipoNoticia;
    char titulo[33];
    char texto[513];

    while(opcao != 3) {
        printf("\nO que deseja fazer? \n1- Cadastrar notícia\n2- Fechar edição\n3- Sair\n");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:            
                requisita(titulo, texto);
            
                // seleciona o tipo de notícia (em qual fila cadastrar)
                printf("Qual o tipo da notícia?\n1- Informe\n0- Breaking News\n");
                scanf("%d", &tipoNoticia);
        
                switch (tipoNoticia) {
                    case 1:
                        lista_insere(listaInf, titulo, texto);
                        break;

                    case 0:
                        lista_insere(listaBN, titulo, texto);
                        break;

                    default:
                        printf("Opção inválida!\n");
                        break;
                }

                break;
        
            case 2:
                fechamento_edicao(listaBN, listaInf);
                break;
        
            case 3:
                lista_destroi(listaBN);
                lista_destroi(listaInf);
                exit(0);
                break;
            
            default:
                printf("Opção inválida!\n");
                break;
        }
    }

    return 0;
}