
/*
 * tic_tac_toe.c
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <errno.h>

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })


enum { NOUGHTS, CROSSES, BORDER, EMPTY, LEDOFFN, LEDOFFC};
enum { HUMANWIN, COMPWIN, DRAW };

typedef int (*choosePlayer)(const int *board);
/* ARRAY BOARD*/

int led_button_choosen;

/*Tic Tac Toe variables*/
int player = CROSSES;
int opponent = NOUGHTS;

typedef struct move
{
	int Move_position;
	int bestVal;
}Move;

int true = 1;
int false = 0;


/*Functions*/
int GetNumForDir(int startSq, const int dir, const int *board, const int us);
int FindFourInARow(int *board, const int ourindex, const int us);
void InitialiseBoard(int *board) ;

int HasEmpty(const int *board);
void MakeMove(int *board, const int sq,  int side);
int GetHumanMove(const int *board);
int GetComputerMove(int *board, const int side, int max_depth);

void RunGame();
int eval_comp_move(int *board, int moveImake, const int side);
int FindCleanInARoW(int *board, int my_move, int side);

//Console application
void PrintBoard(const int *board);

//CPP MinMax

int minimax(int *board, int depth, int isMax, int moveImake, int save_score, int max_depth);
int GetNumForDirClean(int startSq, const int dir, const int *board, int us);

int isItCorner(int moveImake);
int detecCombo(int newScore);


//make it static in function Combo...
int oldScore = 0;


Move findBestMove(int *board, int side,int max_depth);

/*
int board[36*4] = {

	 0, 1, 2, 3, 4, 5
	 6, 7, 8, 9,10,11
	12,13,14,15,16,17
	18,19,20,21,22,23
	24,25,26,27,28,29
	30,31,32,33,34,35

	36,37,38,39,40,41,
	42,43,44,45,46,47,
	48,49,50,51,52,53,
	54,55,56,57,58,59,
	60,61,62,63,64,65,
	66,67,68,69,70,71,

	72,73,74,75,76,77,
	78,79,80,81,82,83,
	84,85,86,87,88,89,
	90,91,92,93,94,95,
	96,97,98,99,100,101,
	102,103,104,105,106,107,

	108,109,110,111,112,113,
	114,115,116,117,118,119,
	120,121,122,123,124,125,
	126,127,128,129,130,131,
	132,133,134,135,136,137,
	138,139,140,141,142,143,


	 0, 1, 2, 3, 4,
	 5, 6, 7, 8, 9,
	10,11,12,13,14,
	15,16,17,18,19,
	20,21,22,23,24
}
*/
								   //   dol dpos dpos-, diag
const int Directions[11] = { 1, 6, 5, 7, 36, 37, 35, 43, 41, 42, 30};

//NOTE: REVERSE TO MATCH LED DIRECTIONS
const int ConvertTo36_4[16*4] = {
	7, 8, 9, 10,
	13,14,15,16,
	19,20,21,22,
	25,26,27,28,

	43,44,45,46,
	49,50,51,52,
	55,56,57,58,
	61,62,63,64,

	79,80,81,82,
	85,86,87,88,
	91,92,93,94,
	97,98,99,100,

	115,116,117,118,
	121,122,123,124,
	127,128,129,130,
	133,134,135,136
};

const int LedArray3D_TicTacToe[4][4][4] = {
	{
		{7, 8, 9, 10},
		{13,14,15,16},
		{19,20,21,22},
		{25,26,27,28}
	},
	{
		{43,44,45,46},
		{49,50,51,52},
		{55,56,57,58},
		{61,62,63,64}
	},
	{
		{79,80,81,82},
		{85,86,87,88},
		{91,92,93,94},
		{97,98,99,100}
	},
	{
		{115,116,117,118},
		{121,122,123,124},
		{127,128,129,130},
		{133,134,135,136}
	},
};

/**
 * @brief  Fill the start board, make boarders and empty tiles
 * @param  Empty array size 36*4
 * @retval None
 */
void InitialiseBoard(int *board)
{
	int index;
	for(index = 0; index < 36*4; ++index) {
		board[index] = BORDER;
	}

	for(index = 0; index < 16*4; ++index) {
		board[ConvertTo36_4[index]] = EMPTY;
	}
}

/**
 * @brief  RBGCube -> Find layer 1,..,4 from 1,...,64 input
 * @param  num 1,...,64
 * @retval return layer 1,...,4
 */
int DetermineLayer(const int n )
{
	if(n < 16) return 1;
	if(16 <= n && n < 32) return 2;
	if(32 <= n && n < 48) return 3;
	if(48 <= n && n < 64) return 4;

	return 0;
}

/**
 * @brief  Debugging ->Print arr
 * @param  Array to print
 * @param  Length of array
 * @retval None
 */
void Print_arr(int *arr, int length, const char *name)
{
	int i;
	printf("Array: %s \n", name);
	printf("length: %d \n", length);
	for(i = 0; i < length; i++) {
		printf("%d,",arr[i]);
	}

	printf("\n");
}

/**
 * @brief  Console application -> Print board
 * @param  Array to print
 * @param  Length of array
 * @retval None
 */
void PrintBoard(const int *board)
{
	int n;
	char pceChars[] = "OX|-";

	printf("\n\nBoard:\n\n");
	for(n = 0; n < 16*4; n++) {
		if(n!=0 && n%4==0) {
			printf("\n\n");
		}
		 if(n!=0 && n%16==0) {
			printf("\n\n");
		}
		printf("%4c",pceChars[board[ConvertTo36_4[n]]]);
	}
	printf("\n");
}

/**
 * @brief  Find Empty tile, return 1, else 0
 * @param  Board size 36*4, playing board
 * @retval find empty tile 1, else 0
 */
int HasEmpty(const int *board)
{
	int index ;
	for(index = 0; index < 16*4; ++index) {
		if( board[ConvertTo36_4[index]] == EMPTY) return 1;
	}
	return 0;
}

/**
 * @brief  Save move into board
 * @param  playing board
 * @param  position from ConvertTo36_4[0,...,63]
 * @param  cross or nought
 * @retval none
 */
void MakeMove(int *board, const int sq, int side)
{
	board[sq] = side;
}

/**
 * @brief  Find if positions has same neighbors as us; max. num 4 for all row equal
 * @param  Evaluating his neighbor
 * @param  dir adding factor (pos. or neg)
 * @param  playing board
 * @param  Evalute for crosses or noughts
 * @retval found 0,...,3
 */
int GetNumForDir(int startSq, const int dir, const int *board, const int us)
{
	int found = 0;
    if(startSq > 144 || startSq < 0) return found;
	while(board[startSq] != BORDER) {
		if(board[startSq] != us) {
			break;
		}
		found++;
		startSq += dir;
		if(startSq > 144 || startSq < 0) return found;
	}
	return found;
}

/**
 * @brief  Console application-> CHECK FOR WIN: find if a player has a 4 in row
 * @param  Playing board
 * @param  Single position, check all its neighbors for a win
 * @param  Check crosses or neughts, depends on what our given position tile value is
 * @retval If winner retrn 4 else, retrn 1
 */
int FindFourInARow( int *board, const int ourindex, const int us)
{
	int DirIndex = 0;
	int Dir = 0;
	int FourCount = 1;
	//printf("FiaR!\n");
	for(DirIndex = 0; DirIndex < 11; ++DirIndex) {
		Dir = Directions[DirIndex];
		FourCount += GetNumForDir(ourindex + Dir, Dir, board, us);
		FourCount += GetNumForDir(ourindex + Dir * (-1), Dir * (-1), board, us);
		if(FourCount == 4) {
		
			printf("Winner!!!\n");
			break;
		}
		FourCount = 1;
	}
	return FourCount;
}

/**
 * @brief  Adding extra points if position is on corners or vertex
 * @param  Player move
 * @retval score
 */
int isItCorner(int moveImake)
{
	int i;
    int score = 0;
	int vertex[8] =  {7,10,25,28,115,118,133,136};
	int corner[16] = {7,10,25,28,115,118,133,136,43,46,61,63,79, 82, 97, 100};

	for(i = 0; i < 8; i++) {
		if(moveImake == vertex[i]) score += 8;
	}

	for(i = 0; i < 16; i++) {
		if(corner[i] == moveImake) score +=4;
	}

	return score;
}

/**
 * @brief  Adding extra points if in func. FindCleanInARoW are same neighbors in one row
 * @param  Calculated score from neighbors
 * @retval score
 */
int detecCombo(int newScore)
{
    int score = 0;
    //TODO: add static oldscore here
    if(newScore-oldScore >= 200)  score+=100;    //majbe mal veci
    if(newScore-oldScore >= 300)  score+=10000;
    oldScore = newScore+score;
    return score;
}

/**
 * @brief  Evaluate single move by looking at his neighbors and giving POINTS for diff. combination
 * @param  Evaluating his neighbor
 * @param  dir is positive or negative one
 * @param  playing board
 * @param  Evalute for crosses or noughts
 * @retval points
 */
int GetNumForDirClean(int startSq, const int dir, const int *board,  int us)
{
	int points = 0;
	if(startSq > 144 || startSq < 0) return points;             //catch him if out of playing board
	while(board[startSq] != BORDER) {
		if(board[startSq] == EMPTY)	points+=1;                  //find empty; point -> 1
		if(board[startSq] != us && board[startSq] != EMPTY) {   //TODO: change to side^1
			break;                                              //If hit a neighbor whitch if opposite of him, imidiaty return
		}
		if(board[startSq] == us) points += 100;                 //Neighbor same as us; points -> 100
		startSq += dir;                                         //going to a neighbor
		if(startSq > 144 || startSq < 0) return points;         //catch him if out of playing board
	}
	return points;
}

/**
 * @brief  Evalute move, giving a points
 * @param  Playing board
 * @param  Move to evaluate
 * @param  cross or nought
 * @retval score
 */
 //TODO: board constant
int FindCleanInARoW(int *board, int my_move, int side)
{
	int DirIndex = 0;
	int Dir = 0;
	int score = 0;
	oldScore = 0;
	//printf("FiaR!\n");
	for(DirIndex = 0; DirIndex < 11; ++DirIndex) {
		Dir = Directions[DirIndex];
		score += GetNumForDirClean(my_move + Dir, Dir, board, side);
		score += GetNumForDirClean(my_move + Dir * (-1), Dir * (-1), board, side);
        score += detecCombo(score);

	}
    score += isItCorner(my_move);
	return score;
}

/**
 * @brief  Evalute move, giving a points IF US return POSITIVE value, ENEMY NEGATIVE value
 * @param  Playing board
 * @param  Move to evaluate
 * @param  cross or nought
 * @retval score
 */
 //TODO: board constant and maybe combine with FindCleanInARoW
int eval_comp_move(int *board, int moveImake,  int side)
{
	int retrn_val;
	retrn_val = FindCleanInARoW(board, moveImake,  side);
	if(side == opponent) retrn_val *= (-1);

	return retrn_val;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int minimax(int *board, int depth, int isMax, int moveImake, int save_score, int max_depth)
{

	int MoveList[16*4];
	int MoveCount = 0;
    int Move_n;
	int index;

	int side;
	(isMax == false) ? (side=player):(side=opponent);
	int score = eval_comp_move(board, moveImake, side);
    save_score = score;
	//if depth ==4 end it
	if (depth == max_depth)
		return score;

	// If Maximizer has won the game return his/her
	// evaluated score
	if(score >= 10000)
		return score;

    if(score <= -10000)
		return score;

	// If Minimizer has won the game return his/her
	// evaluated score
	//TODO: ce je se prosto mesto left

	// If this maximizer's move
	if (isMax)
	{
		int best = -10000;

		// Traverse all cells
		// fill Move List -> find empty places
		for(index = 0; index < 16*4; ++index) {
			if( board[ConvertTo36_4[index]] == EMPTY) {
				MoveList[MoveCount++] = ConvertTo36_4[index];
			}
		}

		// loop all moves
		for(index = 0; index < MoveCount; ++index) {
			Move_n = MoveList[index];
			// Make the move
			board[Move_n] = player;

			// Call minimax recursively and choose
			// the maximum value
			best = max( best, minimax(board, depth+1, !isMax, Move_n, save_score, max_depth) );
            //printf("\t\t Max:%d=%d + %d \n", best+save_score, save_score,best);

			// Undo the move
			board[Move_n] = EMPTY;
		}
		best = save_score + best;
		return best;
	}

	// If this minimizer's move
	else
	{
		int best = 10000;

			// Traverse all cells
		// fill Move List -> find empty places
		for(index = 0; index < 16*4; ++index) {
			if( board[ConvertTo36_4[index]] == EMPTY) {
				MoveList[MoveCount++] = ConvertTo36_4[index];
			}
		}

		// loop all moves
		for(index = 0; index < MoveCount; ++index) {
			Move_n = MoveList[index];
			// Make the move
			board[Move_n] = opponent;

			// Call minimax recursively and choose
			// the maximum value
			best = min( best, minimax(board, depth+1, !isMax, Move_n,save_score, max_depth));
            //printf("\tMin:%d=%d + %d \n", best+save_score, save_score,best);

			// Undo the move
			board[Move_n] = EMPTY;
		}
		best = save_score + best;
		return best;
	}
}

// This will return the best possible move for the player
Move findBestMove(int *board, int side, int max_depth)
{
	Move bestMove;
	bestMove.Move_position = -1;
	bestMove.bestVal = -1000000;

	int Move_n;
	int MoveList[16*4];
	int MoveCount = 0;
	int index;

	printf("FindBestMove, I am %d \n", side);

	// Traverse all cells, evalutae minimax function for
	// all empty cells. And return the cell with optimal
	// value.
		for(index = 0; index < 16*4; ++index) {
			if( board[ConvertTo36_4[index]] == EMPTY) {
				MoveList[MoveCount++] = ConvertTo36_4[index];
			}
		}

		for(index = 0; index < MoveCount; ++index) {
			Move_n = MoveList[index];

			// Make the move
			board[Move_n] = side;

			// compute evaluation function for this
			// move.
			int moveVal = minimax(board, 0, false, Move_n, 0, max_depth);
			//falses is opponent
			printf("%d -> %d \n", Move_n, moveVal);
			// Undo the move
			board[Move_n] = EMPTY;

			// If the value of the current move is
			// more than the best value, then update
			// best/
			if (moveVal > bestMove.bestVal) {
				bestMove.Move_position = Move_n;
				bestMove.bestVal = moveVal;
			}
		}

	printf("The value of the best Move is : %d\n\n",bestMove.bestVal);
	return bestMove;
}

/**
 * @brief  Computer calculate his move
 * @param  Playing board
 * @param  Check crosses or neughts, depends on what our given position tile value is
 * @param  MinMax max. depth
 * @retval Move
 */
int GetComputerMove(int *board, const int side, int max_depth)
{
	Move bestMove = findBestMove(board, side, max_depth);

	printf("The Optimal Move is :\n");
	printf("Position: %d Value: %d\n\n", bestMove.Move_position, bestMove.bestVal );

	return bestMove.Move_position;
}

/**
 * @brief  Console application-> Human adds his move
 * @param  Playing board
 * @retval human move
 */
int GetHumanMove(const int *board) {

	char userInput[4];

	int moveOk = 0;
	int move = -1;

	while (moveOk == 0) {

		printf("Please enter a move from 1 to 64:");
		fgets(userInput, 3, stdin);
		fflush(stdin);

		if(strlen(userInput) != 2) {
			//printf("Invalid strlen()\n");
			continue;
		}

		if( sscanf(userInput, "%d", &move) != 1) {
			move = -1;
			printf("Invalid sscanf()\n");
			continue;
		}

		if( move < 1 || move > 16*4) {
			move = -1;
			printf("Invalid range\n");
			continue;
		}

		move--; // Zero indexing

		if( board[ConvertTo36_4[move]]!=EMPTY) {
			move=-1;
			printf("Square not available\n");
			continue;
		}
		moveOk = 1;
	}
	printf("Making Move...%d\n",(move+1));
	return ConvertTo36_4[move];
}

/**
 * @brief  Main RunGame func.
 * @retval none
 */
void RunGame()
{
	int GameOver = 0;
	int Side = NOUGHTS;
	int LastMoveMade = 0;
	int board[36*4];
	int a = 1;
	printf("a=%d, a^1=%d\n", a, a^1);
	
	InitialiseBoard(&board[0]);
	PrintBoard(&board[0]);

		while(!GameOver) {
		if(Side==NOUGHTS) {
			LastMoveMade =GetHumanMove(&board[0] );
			MakeMove(&board[0],LastMoveMade,Side);
			Side=CROSSES;
		} else {
			LastMoveMade = GetComputerMove(&board[0], Side,2);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}

		// if three in a row exists Game is over
		if( FindFourInARow(board, LastMoveMade, Side ^ 1) == 4) {
			printf("Game over!\n");
			GameOver = 1;
			if(Side==NOUGHTS) {
				printf("Ply1->Crosses Wins\n");
			} else {
				printf("Ply1->NOUGHTS Wins\n");
			}
		}

		// if no more moves, game is a draw
		if(!HasEmpty(board)) {
			printf("Game over!\n");
			GameOver = 1;
			printf("It's a draw\n");
		}
	}
	PrintBoard(&board[0]);
    getchar();
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
    RunGame();

	return 0;
}











