#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include "SPFIARParser.h"
#include "SPFIARGame.h"


/**
 * if there was a problem in memory allocation during a run of the program, this enum will determine
 * in which alloc function the problem occured.
 */
typedef enum sp_fiar_func_error_t{
	MALLOC,
	CALLOC,
	NONE,
}FuncError;

/**
 *enum that helps us determine if there was a problem during a computer turn in the game (such as problem while creating the
 *minimax tree, problem with the argument maxDeptch, etc.).
 *minimax */
typedef enum sp_fiar_comp_turn_t{
	SUCCESS,
	FAIL,
}CompTurn;


/**
 * this enum holds all the 4-span directions that needed to be check during checking for a winner
 * in a specific board, or computing the value of a board in the scoring function.
 */
typedef enum sp_fiar_game_direction_t{
	RIGHT,
	UP,
	RIGHT_UP,
	RIGHT_DOWN,
}SPAN_DIRECTION;

extern FuncError err;

/**
 * iterartes over 'arr' which contains 'nElements' elements
 * return 1 if func(element) != 0 for every element in arr
 * and 0 otherwise.
 */
int forall(void *arr, int nElements, int (*func)(void *));

/**
 * return 1 if a *d representing a digit between 0-9 and 0 otherwise
 */
int isDigit(void *d);

/**
 * return false if n == 0 and true otherwise.
 */
bool intToBool(int n);

/**
 * gets the game difficulty level from the user and checks if the difficulty level is valid
 * (i.e. between 1-7).
 * if not valid, return 0.
 */
int init();

/**
 *gets the next move from the user (add_disc, restart_game etc.). if it's a valid SP_Command
 *return the corresponding value, otherwise prints "Invalid command" and repeat itself.
 */
SP_COMMAND playUserTurn(SPFiarGame* game, int maxDepth);

/**
 *simulates the computer turn by calling to suggest move and play as it suggets.
 */
CompTurn playComputerTurn(SPFiarGame* game, int maxDepth);

/**
 *called after the game is over and prints corresponding message. waiting for the next move from
 *the user - 'quit', 'restart_game' or 'undo_move'. for every other command, the user is asked
 *the to enter a command again until one of the above.
 *returns the SP_Command that matches the user input (i.e. one of the three above)
 */
SP_COMMAND gameOver (SPFiarGame* game, char player);



/**
 * @param currentPlayer - the current player ('X' or 'O')
 * * @return
 * 'X' if the current player is 'O'
 * 'O' if the current player is 'X'
 */
char changeTurn(char currentPlayer);

/**
 * prints a specific row of the gameboard.
 * @param *row - reference to a specific row of the gameboard.
 */
void printRow(char *row);

/**
 * gets a specific cell of the board and a direction in which to check the span, and
 * return the number of SP_FIAR_GAME_PLAYER_SYMBOL ('player') that apears in the span.
 * @param src - the current game state
 * @param row - the row number of the specific cell
 * @param col - the column number of the specific cell
 * @param player - who's turn it is (what symbol to look for in the spans)
 * @param direction - the direction of the span that needed to be checked.
 * @return
 * the number of the @param player symbols in the specific span.
 */
int checkSpan(SPFiarGame* src , int row, int col, char player, SPAN_DIRECTION direction);


/**
 * gets a specific cell of the board and checks whether the is a winning span starting
 * from it in all directions.
 * @param src - the current game state
 * @param row - the row number of the specific cell
 * @param col - the column number of the specific cell
 * @param player - who's turn it is (what symbol to look for in the spans)
 * @param directions - all the directions that are needed to be checked
 * @return
 * true, if there is a winning span which starting from the specific cell
 * false, otherwise
 */
bool checkAllSpans (SPFiarGame* src , int row, int col, char player, SPAN_DIRECTION* direction);


/**
 * checks whether the board is full or not
 * @param src = the current game state
 * @return
 * true ,if the board is full
 * false, otherwise
 */
bool isFullBoard (SPFiarGame* src);


/**
 * return the corespondong command for the string in argument p.
 * return SP_INVALID_LINE if the string does not represent a valid command.
 */
SP_COMMAND toSP_COMMAND(char *p);

#endif
