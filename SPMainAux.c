/*
 * SPMainAux.c
 *
 *  Created on: 12 בדצמ 2017
 *      Author: omerg
 */
#include<stdbool.h>
#include <stdio.h>
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include "SPMinimax.h"
#include "SPMainAux.h"
#include <string.h>
#include <stdlib.h>


SP_COMMAND toSP_COMMAND(char *p){
	if(strcmp(p,"undo_move")==0)
		return SP_UNDO_MOVE;
	else if(strcmp(p,"add_disc")==0)
		return SP_ADD_DISC;
	else if(strcmp(p,"suggest_move")==0)
			return SP_SUGGEST_MOVE;
	else if(strcmp(p,"quit")==0)
			return SP_QUIT;
	else if(strcmp(p,"restart_game")==0)
			return SP_RESTART;
	else
		return SP_INVALID_LINE;
}

//iterates over elements of 'arr' which contains 'nElements' of size 'sizeOfElement'
//and return true if func(arr[i]) == true for every 0<=i && i<= 'nElements' - 1.
int forall(void *arr, int nElements, int (*func)(void *)){
	char *p = arr;
	for(int i=0 ; i<nElements ; i++){
		p+=1;
		if(!func(p))
			return 0;
	}
	return 1;
}

//return true if 'd' is a digit and false else.
int isDigit(void *d){
	if('0'<= *(char *)d && *(char *)d <='9')
		return 1;
	return 0;
}

bool intToBool(int n){
	if(n)
		return true;
	return false;
}

int checkSpan(SPFiarGame* src , int row, int col, char player, SPAN_DIRECTION direction){
	int counter = 0, up, right;
	bool b;
	switch(direction){
		case UP:
			b = (row<=2);
			up = 1;
			right =0;
			break;
		case RIGHT:
			b = (col<=3);
			up = 0;
			right = 1;
			break;
		case RIGHT_UP:
			b = (row<=2 && col<=3);
			up = 1;
			right = 1;
			break;
		case RIGHT_DOWN:
			b = (row>=3 && col<=3);
			up = -1;
			right = 1;
			break;
	}
	if(!b)
		return counter;
	for(int i=0; i<4 ; i++){
			if((src->gameBoard[row+(i*up)][col+(i*right)]) == player)
				counter++;
	}
	return counter;
}


bool checkAllSpans (SPFiarGame* src , int row, int col, char player, SPAN_DIRECTION* direction){
	for(int i=0;i<4;i++){
		if(checkSpan(src, row, col, player, direction[i]) == 4)
			return true;
	}
	return false;
}


bool isFullBoard (SPFiarGame* src){
	for(int i=0; i<SP_FIAR_GAME_N_COLUMNS ; i++){
		if(!(src->tops[i] == SP_FIAR_GAME_N_ROWS))
			return false;
	}
	return true;
}

int init(){
	int difficulty;
	SPCommand cmd;
	char *p,input[SP_MAX_LINE_LENGTH];
	bool valid = false;
	while(!valid){
		printf("Please enter the difficulty level between [1-7]:\n");
		fgets(input,SP_MAX_LINE_LENGTH,stdin);
		cmd = spParserPraseLine(input);
		if(cmd.cmd==SP_QUIT){
			printf("Exiting...\n");
			return 0;
		}
		p = strtok(input," \t\n\r");
		if(p==NULL)
			printf("Error: invalid level (should be between 1 to 7)\n");
		else if(spParserIsInt(p) && strtok(NULL," \t\n\r")==NULL){
			difficulty = atoi(input);
			if(difficulty >= 1 && difficulty <=7)
				valid=true;
			else
				printf("Error: invalid level (should be between 1 to 7)\n");
		}
		else
			printf("Error: invalid level (should be between 1 to 7)\n");
	}
	return difficulty;
}


SP_COMMAND playUserTurn(SPFiarGame* game, int maxDepth){
	SPCommand cmd;
	bool valid = false;
	printf("Please make the next move:\n");
	while(!valid){
		char inputStr[SP_MAX_LINE_LENGTH];
		fgets(inputStr,SP_MAX_LINE_LENGTH,stdin);
		cmd  = spParserPraseLine(inputStr);
		switch(cmd.cmd){
			case SP_SUGGEST_MOVE:
				printf("Suggested move: drop a disc to column %d\n", spMinimaxSuggestMove(game, maxDepth)+1);
				break;
			case SP_UNDO_MOVE:
				if(spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_SUCCESS){
					printf("Remove disc: remove computer's disc at column %d\n", game->turns->elements[game->turns->actualSize]+1);
					spFiarGameUndoPrevMove(game);
					printf("Remove disc: remove user's disc at column %d\n", game->turns->elements[game->turns->actualSize]+1);
					spFiarGamePrintBoard(game);
					printf("Please make the next move:\n");
				}
				else
					printf("Error: cannot undo previous move!\n");
				break;
			case SP_ADD_DISC:
				if(cmd.validArg){
					if(!(cmd.arg>=1 && cmd.arg<=7))
						printf("Error: column number must be in range 1-7\n");
					else if((game->tops[cmd.arg-1]) == 6)
						printf("Error: column %d is full\n" , cmd.arg);
					else{
						spFiarGameSetMove(game,cmd.arg-1);
						valid = true;
					}
				}
				else
					printf("Error: column number must be in range 1-7\n");
				break;
			case SP_QUIT:
				return SP_QUIT;
			case SP_RESTART:
				return SP_RESTART;
			case SP_INVALID_LINE:
				printf("Error: invalid command\n");
				break;
			}
	}
	return SP_ADD_DISC;
}

CompTurn playComputerTurn(SPFiarGame* game, int maxDepth){
	int move = spMinimaxSuggestMove(game, maxDepth);
	if(move == -1)
		return FAIL;
	spFiarGameSetMove(game, move);
	printf("Computer move: add disc to column %d\n",move+1);
	return SUCCESS;
}


SP_COMMAND gameOver(SPFiarGame* game, char player){
	SPCommand cmd;
	bool valid = false;
	if(player == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		spFiarGamePrintBoard(game);
	printf("Game over: ");
	switch(player){
		case SP_FIAR_GAME_PLAYER_1_SYMBOL:
			printf("you win\n");
			break;
		case SP_FIAR_GAME_PLAYER_2_SYMBOL:
			printf("computer wins\n");
			break;
		default:
			printf("it's a tie\n");
			break;
	}
	printf("Please enter 'quit' to exit or 'restart' to start a new game!\n");
	while(!valid){
		char inputStr[SP_MAX_LINE_LENGTH];
		fgets(inputStr,SP_MAX_LINE_LENGTH,stdin);
		cmd  = spParserPraseLine(inputStr);
		switch(cmd.cmd){
			case SP_QUIT:
				return SP_QUIT;
			case SP_RESTART:
				return SP_RESTART;
			case SP_UNDO_MOVE:
				if(player==SP_FIAR_GAME_PLAYER_1_SYMBOL){
					spFiarGameUndoPrevMove(game);
					printf("Remove disc: remove user's disc at column %d\n", game->turns->elements[game->turns->actualSize]+1);
				}
				else{
					spFiarGameUndoPrevMove(game);
					printf("Remove disc: remove computer's disc at column %d\n", game->turns->elements[game->turns->actualSize]+1);
					spFiarGameUndoPrevMove(game);
					printf("Remove disc: remove user's disc at column %d\n", game->turns->elements[game->turns->actualSize]+1);
				}
				valid = true;
				break;
			default:
				printf("Error: the game is over\n");
				break;
		}
	}
	return SP_UNDO_MOVE;
}


