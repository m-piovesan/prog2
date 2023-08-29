#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *rotulo;
  char *tipo;
  char *categorias;
} atributo;

void exibe_atributos(atributo *infos, int tamanho) {
    if (infos == 0){
        printf("O arquivo ARFF fornecido é inválido!\n");
        exit(0);
    }

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for(int i = 0; i < tamanho; i++){
        printf("-> Atributo #%d\n", i+1);
        printf("Rótulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < tamanho-1) printf("------------------------------\n");
    }
    printf("===============================\n");
}

// conta quantos atributos tem no arquivo
int conta_atributos(FILE *arff){
  char buffer[1024 + 1], *campos[2]; // buffer + vet para armazenar os campos (tamanho 2 pra testar se começa com @attribute)
  int cont = 0;

  // FILE *file = fopen(arff, "r+");

  if (arff == NULL) {
    perror("Erro ao abrir o arquivo");
    return 1;
  }

  while (!feof(arff)) {
    // Lendo o arquivo e descartando o caractere de nova linha (\n)
    fscanf(arff, "%1024[^\n]", buffer);
    fgetc(arff);

    campos[0] = strtok(buffer, " ");
      
    for (int i = 0; i < 1; i++)
      campos[i + 1] = strtok(NULL, " "); // O segundo parâmetro é NULL para continuar a partir da última posição
    
    if (!strcmp(campos[0], "@attribute"))
      cont++;
  }
  
  fclose(arff);
  return cont;
}

// para cada atributo, armazena o seu rótulo, tipo e categorias em uma estrutura
atributo* processa_atributos(FILE *arff) {
  if (arff == NULL) {
    perror("Erro ao abrir o arquivo");
    return NULL;
  }

  int tamanho = conta_atributos(arff);
  atributo *infos = malloc(tamanho * sizeof(atributo));

  if (infos == NULL) {
    perror("Erro ao alocar memória");
    return NULL;
  }

  char buffer[1024 + 1], *campos[4]; // buffer + vet para armazenar os campos (tamanho 4 pra salvar os valores)

  // ler todos os valores da linha, se for @attribute, salvar os valores
  while(!feof(arff)) {
    fscanf(arff, "%1024[^\n]", buffer);
    fgetc(arff);

    campos[0] = strtok(buffer, " ");

    if(!strcmp(campos[0], "@attribute")) {
      for (int i = 0; i < 3; i++) {
        campos[i + 1] = strtok(NULL, " ");

        infos[i].rotulo = strdup(campos[1]);
        infos[i].tipo = strdup(campos[2]);
        infos[i].categorias = strdup(campos[3]);
      }   
    }
  }

  fclose(arff);
  return infos;
}

int main(int argc, char **argv){
  int opt, tamanho;
  char exibicao = 0;
  char *entrada = 0;
  atributo *infos;

  while ((opt = getopt(argc, argv, "pi:")) != -1) {
    switch (opt) { 
    case 'i':
      entrada = strdup(optarg);
      break;
    case 'p':
      exibicao = 1;
      break;
    default:
      fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
      exit(1);
    }
  }

  if ( ! entrada ) {
    fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
    exit(2);
  }

  // VOC� DEVE IMPLEMENTAR AS ROTINAS NECESS�RIAS E A CHAMADA DE FUN��ES PARA PROCESSAR OS ATRIBUTOS ARFF AQUI
  FILE *file = fopen("car.arff", "r+");

  tamanho = conta_atributos(file);

  if (entrada) {
    infos = processa_atributos(file);
  }

  if (exibicao){
    exibe_atributos(infos, tamanho);
  }

  return 0 ;
}