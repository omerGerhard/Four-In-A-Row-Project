/*
 * SPFIARGame.c
 *
 *  Created on: 20 בדצמ 2017
 *      Author: omerg
 */
#include <stdlib.h>
#include <stdio.h>
#include "SPFIARGame.h"
#include "SPMainAux.h"

SPFiarGame* spFiarGameCreate(int historySize){
	if(historySize<=0)
		return NULL;
	SPFiarGame *game = (SPFiarGame *)malloc(sizeof(SPFiarGame));
	if(game==NULL){
		err = MALLOC;
		return NULL;
	}
	game->turns = spArrayListCreate(historySize);
	if(game->turns==NULL){
		free(game);
		return NULL;
	}
	game-> currentPlayer=SP_FIAR_GAME_PLAYER_1_SYMBOL;
	for(int i=0 ; i<SP_FIAR_GAME_N_COLUMNS ;i++)
		game->tops[i] = 0;
	for(int i=0 ; i<SP_FIAR_GAME_N_ROWS ; i++){
		for(int j=0 ; j<SP_FIAR_GAME_N_COLUMNS ; j++)
			game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
	}
	return game;
}


SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	if(src==NULL)
		return NULL;
	SPFiarGame *copy = spFiarGameCreate(src->turns->maxSize);
	if(copy==NULL)
		return NULL;
	spArrayListDestroy(copy->turns);
	copy->turns = spArrayListCopy(src->turns);
	if(copy->turns==NULL){
		free(copy);
		return NULL;
	}
	copy->currentPlayer = src->currentPlayer;
	for(int i=0; i<SP_FIAR_GAME_N_ROWS ; i++){
		for(int j=0 ; j<SP_FIAR_GAME_N_COLUMNS ; j++)
			copy->gameBoard[i][j] = src->gameBoard[i][j];
	}
	for(int i=0 ; i<SP_FIAR_GAME_N_COLUMNS ; i++)
		copy->tops[i] = src->tops[i];
	return copy;
}

void spFiarGameDestroy(SPFiarGame* src){
	if(src!=NULL){
		spArrayListDestroy(src->turns);
		free(src);
	}
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	//printf("%d",src->tops[col]);
	return (src->tops[col] < SP_FIAR_GAME_N_ROWS);
}

char changeTurn(char currentPlayer){
	if(currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	return SP_FIAR_GAME_PLAYER_1_SYMBOL;
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if(src==NULL || col<0 || col >= SP_FIAR_GAME_N_COLUMNS)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if(!(spFiarGameIsValidMove(src,col)))
		return SP_FIAR_GAME_INVALID_MOVE;
	src->gameBoard[src->tops[col]][col] = src->currentPlayer;
	src->tops[col]++;
	if(src->turns->actualSize==src->turns->maxSize)
		spArrayListRemoveFirst(src->turns);
	spArrayListAddLast(src->turns , col);
	src->currentPlayer = changeTurn(src->currentPlayer);
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if(src==NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if(src->turns->actualSize==0)
		return SP_FIAR_GAME_NO_HISTORY;
	int actualSize = src->turns->actualSize;
	int colUndo = src->turns->elements[actualSize-1];
	src->tops[colUndo] --;
	int rowUndo = src->tops[colUndo];
	src->gameBoard[rowUndo][colUndo] = SP_FIAR_GAME_EMPTY_ENTRY;
	spArrayListRemoveLast(src->turns);
	src->currentPlayer=changeTurn(src->currentPlayer);
	return SP_FIAR_GAME_SUCCESS;
}


void printRow(char *row){
	printf("| ");
	for(int i=0 ; i<SP_FIAR_GAME_N_COLUMNS ; i++){
		switch(row[i]){
			case SP_FIAR_GAME_PLAYER_1_SYMBOL:
				printf("%c ",SP_FIAR_GAME_PLAYER_1_SYMBOL);
				break;
			case SP_FIAR_GAME_PLAYER_2_SYMBOL:
				printf("%c ",SP_FIAR_GAME_PLAYER_2_SYMBOL);
				break;
			default:
				printf("%c ",SP_FIAR_GAME_EMPTY_ENTRY);
				break;
		}
	}
	printf("|\n");
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if(src==NULL)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for(int i=SP_FIAR_GAME_N_ROWS-1 ; i>=0 ; i--)
		printRow(src->gameBoard[i]);
	for(int i=0; i<17 ; i++)
		printf("%c",SP_FIAR_GAME_TIE_SYMBOL);
	printf("\n");
	printf("  1 2 3 4 5 6 7  \n");
	return SP_FIAR_GAME_SUCCESS;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if(src==NULL)
		return SP_FIAR_GAME_EMPTY_ENTRY;
	switch(src->currentPlayer){
		case SP_FIAR_GAME_PLAYER_1_SYMBOL:
			return SP_FIAR_GAME_PLAYER_1_SYMBOL;
		case SP_FIAR_GAME_PLAYER_2_SYMBOL:
			return SP_FIAR_GAME_PLAYER_2_SYMBOL;
		default:
			return SP_FIAR_GAME_EMPTY_ENTRY;
	}
}


char spFiarCheckWinner(SPFiarGame* src){
	if(src==NULL)
		return '\0';
	SPAN_DIRECTION directions[] = {UP , RIGHT , RIGHT_UP , RIGHT_DOWN};
	for(int i=0 ; i<SP_FIAR_GAME_N_ROWS ; i++){
		for(int j=0 ; j<SP_FIAR_GAME_N_COLUMNS ; j++){
			if(checkAllSpans(src,i,j,SP_FIAR_GAME_PLAYER_1_SYMBOL, directions))
				return SP_FIAR_GAME_PLAYER_1_SYMBOL;
			if(checkAllSpans(src,i,j,SP_FIAR_GAME_PLAYER_2_SYMBOL, directions))
				return SP_FIAR_GAME_PLAYER_2_SYMBOL;
		}
	}
	if(isFullBoard(src))
		return SP_FIAR_GAME_TIE_SYMBOL;
	return '\0';
}




