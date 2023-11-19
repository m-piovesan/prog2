#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"

// aloca o elemento sentinela e inicializa os ponteiros first e last com NULL.
shot_sentinel* create_shotlist(void) {
	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
	
	if (list == NULL)
		return NULL;
	
	list->first = NULL;
	list->last  = NULL;
	
	return list;
}

// Remove os tiros da lista (quando o tiro sai do mapa ou acerta um alvo)
shot* remove_shot(shot* current, shot* previous, shot_sentinel *list) {
	if (current == NULL || list == NULL) {
		perror("Erro ao remover tiro da lista: tiro não existe!\n");
		return NULL;
	}

	// se o tiro a ser removido for o primeiro da lista, atualiza o ponteiro first
	if (previous == NULL) {
		list->first = current->next;
		free(current);

		return list->first;
	}

	// se o tiro a ser removido for o último da lista, atualiza o ponteiro last
	if (current->next == NULL) {
		list->last = previous;
		free(current);

		return list->last;
	}

	// se o tiro a ser removido estiver no meio da lista, atualiza o ponteiro next do tiro anterior
	previous->next = current->next;
	free(current);
	
	return previous->next;
}

// esvazia a lista de tiros usando a função remove_shot para desalocar tiro por tiro.
void clean_shots(shot_sentinel *list){
	if (list->first == NULL) {
		perror("Erro ao limpar lista de tiros: lista vazia!\n");
		return;
	}
	
	shot *p = (shot*) list->first;
	shot *q = NULL;

	while (p != NULL)
		p = remove_shot(p, q, list);

	list->first = NULL;
	list->last  = NULL;

	return;
}

//	Os tiros presentes no tabuleiro devem ser atualizados
//  Se o tiro acertar um alvo, ou sair do tabuleiro, ele deve ser removido da lista
//  Caso contrário, ele deve "andar" uma casa (sqm) à frente

// erroooooooo aqui eu acho
// pelo que eu percebi, erro quando o tiro acerta um inimigo
// ./A3 -y 10 -x 6 -e 1 -r 10 -o output.txt 
// (-e 1 indica que só tem uma linha de inimigos)
void update_shots(space *board, shot_sentinel *list){
	if (list->first == NULL) {
		perror("Erro ao atualizar lista de tiros: elemento vazio!\n");
		return;
	}

	if (board == NULL) {
		perror("Erro ao atualizar lista de tiros: mapa não existe!\n");
		return;
	}

	shot *p = (shot*) list->first; // ponteiro para o primeiro tiro da lista
	shot *q = NULL; // ponteiro para o tiro anterior ao tiro apontado por p

	while (p != NULL) {
		int new_position_y = p->position_y + 1; // Atualiza a nova posição do tiro

        if (new_position_y >= 0 && new_position_y <= board->max_y) {            
            board->map[p->position_y][p->position_x].entity = NULL; // remove o tiro da posição anterior no tabuleiro            
            
            if(board->map[new_position_y][p->position_x].entity != NULL) { // se o tiro acertar um alvo, remove o alvo do tabuleiro
                //remove_enemy(board, new_position_y, p->position_x); // remove o inimigo do tabuleiro
                //remove_shot(p, q, list); // remove o tiro da lista
                printf("Acertou um alvo\n");
            } else {
                //board->map[new_position_y][p->position_x].entity = p; // adiciona o tiro na nova posição no tabuleiro
                p->position_y = new_position_y; // atualiza a posição do tiro na lista
            }
        } else {
            p = remove_shot(p, q, list); // se o tiro saiu do tabuleiro, tira da lista
            printf("Saiu do tabuleiro\n");
        } 
        
		q = p;
		p = p->next;
	}

    printf("Atualizou tiros\n");

	return;
}

// Adiciona um novo tiro à lista. Neste momento, todos os tiros se movem apenas para frente
shot* straight_shot(space *board, shot_sentinel *list, enemy *shooter) {
	if (shooter == NULL || board == NULL) {
		perror("Erro ao adicionar tiro na lista: elemento não existe!\n");
		return NULL;
	}
	
	shot *new_shot = (shot*) malloc (sizeof(shot));
	
	if (new_shot == NULL)
		return NULL;
	
	new_shot->position_x = shooter->position_x;
	new_shot->position_y = shooter->position_y;
	new_shot->next = NULL;

	if (list->first == NULL) { // se lista vazia, novo tiro será o primeiro e o último da lista
		list->first = new_shot;
		list->last  = new_shot;
        
		return new_shot;
	} else if (list->first == list->last) { // se apenas um tiro na lista, o novo será o último da lista
        list->first->next = new_shot;
        list->last = new_shot;

        return new_shot;
    }

	list->last->next = new_shot;
	list->last = new_shot;

	return new_shot;	
}

// Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
int add_enemy(space *board, int position_y, int position_x){
	if (board->map[position_y][position_x].entity != NULL) {
		perror("Erro ao adicionar inimigo no mapa: espaço já ocupado!\n");
		return 0;
	}

	if (position_x > board->max_x || position_y > board->max_y) {
		perror("Erro ao adicionar inimigo no mapa: posição inválida!\n");
		return -1;
	}

	enemy *new_enemy = (enemy*) malloc (sizeof(enemy));
	
	if (new_enemy == NULL)
		return 0;
	
	new_enemy->position_x = position_x;
	new_enemy->position_y = position_y;
	board->map[position_y][position_x].entity = new_enemy;
	board->map[position_y][position_x].type = ENEMY;

	return 1;
}

//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
int remove_enemy(space *board, int position_y, int position_x){
	if (board == NULL) {
		perror("Erro ao remover inimigo do mapa: mapa não existe!\n");
		return 0;
	}

	if (board->map[position_y][position_x].entity == NULL) {
		perror("Erro ao remover inimigo do mapa: espaço vazio!\n");
		return 0;
	}


	if (position_x > board->max_x || position_y > board->max_y) {
		perror("Erro ao remover inimigo do mapa: posição inválida!\n");
		return -1;
	}

	free(board->map[position_y][position_x].entity);
	board->map[position_y][position_x].entity = NULL;
	
	return 1;
}
