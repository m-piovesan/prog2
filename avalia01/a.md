# todo
1: abre um arquivo no formato ARFF;
2: lê a sua seção de atributos;
3: para cada atributo, armazena o seu rótulo, tipo e categorias em uma estrutura como a definida a seguir:

typedef struct {
    char *rotulo;
    char *tipo;
    char *categorias;
} atributo;

Se o tipo for numérico ou textual, em “tipo” deve constar, respectivamente, “numeric” e
“string”, e em “categorias” deve existir zero (NULL); se o tipo for categórico, em “tipo”
deve constar “categoric” e em categorias deve constar a string com todas as categorias
(no exemplo, seria “{BC,MC,Bacharel,Mestre,Doutor}”).


- tem que ir achando os @, indicando que cada um é uma linha

- Uma linha de atributo pode conter menos ou mais elementos, além dos necessários (o programa deve tratar esse possível erro!);