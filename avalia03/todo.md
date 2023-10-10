- Você deverá implementar a simulação dos tiros usando listas ligadas com sentinela já implementadas em enemy.h / enemy.c

- o usuário irá fornecer:
    * as dimensões do tabuleiro
    * o número de linhas inimigas
    * a quantidade de iterações que o jogo irá evoluir/executar 
    * nome para o arquivo de saída (relatório).

# TO-DO:
    
- A3.c:
    * void execute_event(space *board, shot_sentinel *list)
        - Atualizar a lista de tiros.
        
        - Verificar se novos tiros devem ser realizados. Um novo tiro só pode ser
          criado caso um inimigo não tenha mais nenhum outro à sua “frente”
          (coluna) e não exista um tiro ativo na atuação (coluna).

- enemy.c:
    * shot* remove_shot(shot* current, shot* previous, shot sentinel *list)
        - remover o tiro current da lista, incluindo a sua desalocação.
        
        - Observe que para remoção deve se verificar se os ponteiros first e last da
          lista devem ser atualizados.
    
    * void update_shots(space *board, shot_sentinel *list)
        - atualizar a posição vertical dos tiros a cada iteração/round
          simulando a sua evolução em direção à terra.
        
        - Observe que os tiros devem ser removidos à medida que eles atinjam o
          solo, isto é, saem do tabuleiro.

    * shot* straight_shoot(space *board, shot_sentinel *list, enemy *shooter)
        - cria um novo tiro
    
    *  int add_enemy(space *board, int position_y, int position_x)
        - adiciona um inimigo/alien ao tabuleiro, fazendo a sua alocação.

    * int remove_enemy(space *board, int position_y, int position_x)
        - Esta função remove um inimigo/alien ao tabuleiro, realizando a sua desalocação.

    * As funções add_enemy e remove_enemy devem verificar se as coordenadas do inimigo/alien são válidas e retornar código diferente de zero em caso de erro.