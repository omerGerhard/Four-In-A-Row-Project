

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SPMainAux.h"
#include "SPFIARGame.h"
#include "SPFIARParser.h"

FuncError err = NONE;

int main()
{
	bool restart;
	char winner;
	SP_COMMAND cmd , gameO;
	SPFiarGame* game = NULL;
	int difficulty = init();
	while(difficulty){
		restart = false;
		if((game = spFiarGameCreate(20)) == NULL){
			if(err == CALLOC){
				printf("Error: calloc has failed");
				return 0;
			}
			else if(err == MALLOC){
				printf("Error: malloc has failed");
				return 0;
			}
		}
		while(!restart){
			spFiarGamePrintBoard(game);
			winner = spFiarCheckWinner(game);
			switch(winner){
				case SP_FIAR_GAME_PLAYER_2_SYMBOL:
					gameO = gameOver(game,SP_FIAR_GAME_PLAYER_2_SYMBOL);
					switch(gameO){
						case SP_QUIT:
							spFiarGameDestroy(game);
							printf("Exiting...\n");
							return 0;
						case SP_RESTART:
							restart = true;
							break;
						default:
							spFiarGamePrintBoard(game);
							cmd = playUserTurn(game,difficulty);
							switch(cmd){
								case SP_QUIT:
									spFiarGameDestroy(game);
									printf("Exiting...\n");
									return 0;
								case SP_RESTART:
									restart = true;
									break;
								default:
									break;
							}
							break;
					}
					break;
				case SP_FIAR_GAME_TIE_SYMBOL:
					//gameO = gameOver(game,SP_FIAR_GAME_PLAYER_2_SYMBOL);
					gameO = gameOver(game,SP_FIAR_GAME_TIE_SYMBOL);
					switch(gameO){
						case SP_QUIT:
							spFiarGameDestroy(game);
							printf("Exiting...\n");
							return 0;
						case SP_RESTART:
							restart = true;
							break;
						default:
							spFiarGamePrintBoard(game);
							cmd = playUserTurn(game,difficulty);
							switch(cmd){
								case SP_QUIT:
									spFiarGameDestroy(game);
									printf("Exiting...\n");
									return 0;
								case SP_RESTART:
									restart = true;
									break;
								default:
									break;
							}
							break;
					}
					break;
				default:
					cmd = playUserTurn(game,difficulty);
					switch(cmd){
						case SP_QUIT:
							spFiarGameDestroy(game);
							printf("Exiting...\n");
							return 0;
						case SP_RESTART:
							restart = true;
							break;
						default:
							break;
					}
				break;
			}
			if(restart!=true){
				winner = spFiarCheckWinner(game);
				if(winner == SP_FIAR_GAME_PLAYER_1_SYMBOL){
					gameO = gameOver(game,SP_FIAR_GAME_PLAYER_1_SYMBOL);
					switch(gameO){
						case SP_QUIT:
							spFiarGameDestroy(game);
							printf("Exiting...\n");
							return 0;
						case SP_RESTART:
							restart = true;
							break;
						default:
							break;
						}
				}
				else{
					if(playComputerTurn(game,difficulty) != SUCCESS){
						if(err == CALLOC){
							printf("Error: calloc has failed");
							return 0;
						}
						else if(err == MALLOC){
							printf("Error: malloc has failed");
							return 0;
						}
					}
				}
			}
		}
		spFiarGameDestroy(game);
		printf("Game restarted!\n");
		difficulty = init();
	}

	return 0;
}

