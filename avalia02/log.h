typedef struct {
    char *rotulo;
    int *qtd_ataques;
} infos_ataque;

typedef struct {
    char *rotulo;
    int *qtd_ataques;
    double *media;
} infos_medias_ataques;

// Gerar um relatório de todos os ataques ocorridos e o número de ocorrências no
// conjunto de dados (nome do arquivo de saída: "R_ATAQUES.txt"); 
void contabiliza_ataques();

// Gerar um relatório dos endereços de origem maliciosos, potencialmente
// maliciosos e benignos (nome do arquivo de saída: "R_ENTIDADES.txt")
void contabiliza_enderecos();

// Gerar um relatório com a média da média do tamanho dos pacotes para cada
// tipo de ataque (nome do arquivo de saída: "R_TAMANHO.txt");
void media_ataques();

// Gerar uma lista negra (blacklist) de endereços de origem considerados
// maliciosos (nome do arquivo de saída: "BLACKLIST.bl").
void gera_blacklist();