# 1 - atributos categoria:
 *  em vez de armazenar uma única string contendo todas as categorias (por
    exemplo, "{a, b, c}"), devemos armazenar um **vetor de strings**, com cada categoria
    separada (por exemplo, na primeira posição "a", na segunda "b" e na terceira "c"). 

 *  essa separação deve ser feita por meio da função "processa_categorias" e deve ser
    chamada no contexto da função "processa_atributos" (observem que essa modificação
    terá impacto em outras funções, como "exibe_atributos").

# 2 - verificar a validade das linhas:
 *  verificar se a quantidade adequada de atributos existe e se cada um 
    desses atributos apresenta um dado compatível com o tipo designado
    para ele na seção de definição dos atributos.   

# 3 - arquivo de ataques: R_ATAQUES.txt
 *  Gerar um relatório de todos os ataques ocorridos e o número de ocorrências no
    conjunto de dados (nome do arquivo de saída: "R_ATAQUES.txt"); 
 
 *  Ataques são caracterizados pelo atributo PKT_CLASS. A única classe de pacotes que NÃO é
    considerada um ataque é a "Normal". Uma linha com um PKT_CLASS diferente de "Normal" é
    considerada uma ocorrência de ataque;

 *  O arquivo "R_ATAQUES.txt" deve conter APENAS linhas no seguinte formato:
    nome_do_ataque;numero_de_ocorrências

    // ler o PKT_CLASS, se não for normal, salvar numa lista com uma struct que tem
        o nome do ataque e a quantidade de vezes que ocorreu, depois imprimir

# 4 - arquivo de endereços: R_ENTIDADES.txt
 *  Gerar um relatório dos endereços de origem maliciosos, potencialmente
    maliciosos e benignos (nome do arquivo de saída: "R_ENTIDADES.txt")

 *  A origem é identificada pelo IP parcial presente no atributo chamado SRC_ADD.
    Uma linha é considerada uma ocorrência no arquivo

 *  Uma origem é considerada benigna se não apresentar nenhuma ocorrência de pacotes
    maliciosos. 

 *  Ela é considerada potencialmente maliciosa se apresentar até 5 ocorrências de
    pacotes maliciosos

 *  é considerada maliciosa se apresentar mais de 5 ocorrências de pacotes
    maliciosos.

 *  O arquivo "R_ENTIDADES.txt" deve conter APENAS linhas no seguinte formato:
    endereço_origem;classificação

    // let o SRC_ADD, cada linha é uma ocorrência, salvar numa lista também se pá
        não entendi direito, ver netlog pra ter exemplo

# 5 - arquivo de médias: R_TAMANHO.txt
 *  Gerar um relatório com a média da média do tamanho dos pacotes para cada
    tipo de ataque (nome do arquivo de saída: "R_TAMANHO.txt");

 *  A média do tamanho dos pacotes é determinada pelo atributo PKT_AVG_SIZE

 *  O arquivo "R_TAMANHO.txt" deve conter APENAS linhas no seguinte formato:
    nome_do_ataque; media_media_do_tamanho

    // ler o PKT_AVG_SIZE do ataque específico e ir somando tudo numa variável
        colocar um contador junto numa struct, no fim, divide e acha a média

# 6 - blacklist:
 *  Gerar uma lista negra (blacklist) de endereços de origem considerados
    maliciosos (nome do arquivo de saída: "BLACKLIST.bl").

 *  Os endereços que devem ser incluídos na lista negra são aqueles das origens
    consideradas maliciosas (R_ENTIDADES), devem ser listados sem repetição

 *  O endereço de origem fornecido pelo log é parcial; 
    portanto, nossa lista negra não será funcional.

 *  O arquivo "BLACKLIST.bl" deve conter APENAS linhas no seguinte formato:
    endereço_origem

    // pegar os endereços maliciosos que filtrou no outro arquivo lá
        e fazer a BL com eles

# extra:
 * lembrar de validar as linhas, detectar os erros e tals


---------------------------------------------------------------------------------------
# correções:
1 - ler a linha (máx 1024)
2 - se for vazia, ignora e pula para próxima + exibe mensagem de erro
3 - testar se começa com "attribute", cc: mensagem de erro
4 - guardar as coisas de acordo com o que se pede (acho que já tá fazendo)
5 - se tiver mais de 3 "palavras", mensagem de erro;