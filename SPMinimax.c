
#include "SPFIARGame.h"
#include "SPMinimaxNode.h"
#include "SPMainAux.h"
#include <stdio.h>
#include <stdlib.h>


int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth){
	if(currentGame == NULL || maxDepth<=0)
		return -1;
	MinimaxNode* root = spMinimaxNodeCreate(currentGame, 0, maxDepth);
	if(root==NULL)
		return -1;
	if(err!=NONE)
		return -1;
/*	if(root->leaf){
		destroyMinimaxNode(root);
		return -1;
	}*/
	setScores(root);
	for(int i=0; i<SP_FIAR_GAME_N_COLUMNS - 1; i++){
		if(root->children[i] != NULL){
			if(root->children[i]->value == root->value){
				destroyMinimaxNode(root);
				return i;
			}
		}
	}
	destroyMinimaxNode(root);
	return SP_FIAR_GAME_N_COLUMNS - 1;
}
