#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arff.h"

void exibe_atributos(atributo *infos, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
}

int conta_atributos(FILE *arff){
  //Fun��o do A1
}

void processa_categorias(atributo *elemento, char *categorias){
  //Recbe uma string com as categorias e atualiza o elemento com um vetor de strings (modificar a struct)
}

atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
}

void valida_arff(FILE *arff, atributo *atributos, int quantidade){
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.
}