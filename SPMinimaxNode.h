#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"
#include<stdbool.h>

typedef struct sp_fiar_minimax_node_t{
	SPFiarGame* game;
	int level;
	int value;
	struct sp_fiar_minimax_node_t **children;
	bool leaf;
}MinimaxNode;

/**
 * Creates a Minimax Tree for a specific state of the game.
 * @param game - The current game state
 * @param level - The current level of the Minimax tree
 * @param maxDepth - The restriction on the depth of the Minimax tree which consistent
 * with the game level
 * @return
 * NULL if there was an allocation problem
 * otherwise the root of the Minimax tree
 */
MinimaxNode* spMinimaxNodeCreate(SPFiarGame* game , int level, int maxDepth);

/**
 * Frees all memory allocation associated with a given Minimax node. If node==NULL
 * the function does nothing.
 *
 * @param node - the source node
 */
void destroyMinimaxNode(MinimaxNode* node);

/**
 * computes the score of a specific node of the Minimax tree where @param game
 * is it's game state.
 *
 * @param game - the current game state
 * @return
 * the score of the node where the @param game is it's game state
 */
int scoringFunction(SPFiarGame* game);


/**
 * choosing the max\min value of the source node's children according to @param max.
 *
 * @param node - the source node
 * @return
 * a value which represents the max\min value of the source node's children
 */
int findMinOrMax(MinimaxNode* node);

/**
 * computes the score of each Minimax node in the Minimax tree.
 * this function uses the sub-functions minMaxChild and scoringFunction from above
 *
 * @param node - the root of the Minimax tree
 */
void setScores(MinimaxNode* node);

/**
 * get the node level in the Minimax tree and returns -1 if the level is even or 1 otherwise
 */
int scoreSign(int nodeLevel);


#endif
