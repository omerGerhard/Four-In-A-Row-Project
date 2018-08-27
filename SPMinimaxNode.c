
#include "SPMinimaxNode.h"
#include "SPFIARGame.h"
#include "SPMainAux.h"
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

MinimaxNode* spMinimaxNodeCreate(SPFiarGame* game, int level, int maxDepth){
	MinimaxNode* node = (MinimaxNode *)malloc(sizeof(MinimaxNode));
	if(node==NULL){
		err = MALLOC;
		return NULL;
	}
	if( (level==maxDepth) || (spFiarCheckWinner(game) != '\0')){
		node->leaf=true;
		node->level=level;
		node->game=spFiarGameCopy(game);
		if(node->game == NULL){
			free(node);
			return NULL;
		}
		return node;
	}
	node->leaf = false;
	node->level = level;
	node->game = spFiarGameCopy(game);
	if(node->game == NULL){
		free(node);
		return NULL;
	}
	node->children = (MinimaxNode **)calloc(SP_FIAR_GAME_N_COLUMNS,sizeof(MinimaxNode *));
	if(node->children==NULL){
		err = CALLOC;
		spFiarGameDestroy(node->game);
		free(node);
		return NULL;
	}
	for(int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){
		if(spFiarGameSetMove(node->game,i) != SP_FIAR_GAME_SUCCESS)
			node->children[i] = NULL;
		else{
			node->children[i] = spMinimaxNodeCreate(node->game,level+1,maxDepth);
			spFiarGameUndoPrevMove(node->game);
		}
	}
	return node;
}

void destroyMinimaxNode(MinimaxNode* node){
	if(node!=NULL){
		if(node->leaf){
			spFiarGameDestroy(node->game);
			free(node);
			return;
		}
		for(int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++)
			destroyMinimaxNode(node->children[i]);
		free(node->children);
		spFiarGameDestroy(node->game);
		free(node);
	}
}


int scoringFunction(SPFiarGame* game){
	char player = changeTurn(game->currentPlayer) , opponent = game->currentPlayer;
	SPAN_DIRECTION directions[] = {UP,RIGHT,RIGHT_UP,RIGHT_DOWN};
	int playerSpan , opponentSpan, diff, res=0;
	int weights[6] = {-5,-2,-1,1,2,5};
	int spans[6] = {0,0,0,0,0,0};
	for(int row=0; row<SP_FIAR_GAME_N_ROWS; row++){
		for(int col=0; col<SP_FIAR_GAME_N_COLUMNS; col++){
			for(int k=0; k<4; k++){
				playerSpan = checkSpan(game , row, col, player, directions[k]);
				opponentSpan = checkSpan(game , row, col, opponent, directions[k]);
				diff = playerSpan-opponentSpan;
				switch(diff){
					case -4:
						return INT_MIN;
					case -3:
						spans[0]++;
						break;
					case -2:
						spans[1]++;
						break;
					case -1:
						spans[2]++;
						break;
					case 1:
						spans[3]++;
						break;
					case 2:
						spans[4]++;
						break;
					case 3:
						spans[5]++;
						break;
					case 4:
						return INT_MAX;
					default:
						break;
				}
			}
		}
	}
	for(int i=0; i<6; i++)
		res += spans[i]*weights[i];
	return res;
}


int findMinOrMax(MinimaxNode* node){
	bool max = (node->level % 2)==0;
	int res;
	if(max){
		res = INT_MIN;
		for(int i=0; i<7; i++){
			if(node->children[i]!=NULL){
				if(node->children[i]->value > res)
					res = node->children[i]->value;
			}
		}
	}
	else{
		res = INT_MAX;
		for(int i=0; i<7; i++){
			if(node->children[i]!=NULL){
				if(node->children[i]->value < res)
					res = node->children[i]->value;
			}
		}
	}
	return res;
}

int scoreSign(int level){
	if((level % 2) ==0)
		return -1;
	return 1;
}

void setScores(MinimaxNode* node){
	int sign;
	if(node==NULL){
		return;
	}
	if(node->leaf){
		sign = scoreSign(node->level);
		node->value = sign*scoringFunction(node->game);
		return;
	}
	for(int i=0; i<7 ; i++)
		setScores(node->children[i]);
	node->value = findMinOrMax(node);
}

