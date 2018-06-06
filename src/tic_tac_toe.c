/*
 * tic_tac_toe.c
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"
//#include "tlc_animations.h"

int led_value_16;
int led_value_adc_x;
int led_value_adc_y;
int led_value_adc_z;



/*Tic Tac Toe variables*/
int player = CROSSES;
int opponent = NOUGHTS;

//make it static in function Combo...
int oldScore = 0;

int true = 1;
int false = 0;

Move findBestMove(int *board, int side,int max_depth);

uint8_t flag_cursor_read_ADC_values;
/*
int board[25] = {
	:,:,:,:,:,;
	:,O,-,X,-,:,
	:,X,-,-,-,:,
	:,-,0,-,X,:,
	:,-,-,-,O,:,
	:,:,:,:,:,:,

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

void BUTTON_OK_EventHandler(TM_BUTTON_PressType_t type)
{
	/* Check button */
	if (type == TM_BUTTON_PressType_OnPressed) {
		//data_lvl1[led_value] = 4095;

		//delte me, just leave normal
		//led_button_choosen = 1;
	} else if (type == TM_BUTTON_PressType_Normal) {
		//data_lvl1[led_value + 16] = 4095;
		led_button_choosen = 1;
		//led_button_choosen = led_value_lvl;
	} else {
		//data_lvl1[led_value + 16] = 0;
	}
}

/*
 * @brif: Draw/erase Crusor
 * @param: ON_CRUSOR, OFF_CRUSOR
 * @param: color in 16
 * */
void DisplayCrusor(int on, int color)
{
	int led_on_off;
	//TODO: change on off and give defines;  change parameters
	if(on == 1){
		led_on_off = 1;
	} else {
		led_on_off = 0;
	}
	//give color or erase it
	Pin_on_crusor(led_value_adc_x, led_value_adc_y, led_value_adc_z, led_on_off);
}

int Read_ADC_difference(int *old, int *new)
{
	if(*new - *old > 120 ||  *old - *new > 120){
		return *new;
	}
	else return *old;
}

void Cursor_read_ADC_values()
{
	static int x_adc_read;
	static int y_adc_read;
	static int z_adc_read;

	int x_read_new;
	int y_read_new;
	int z_read_new;
	//goes in ones just ot fill old values with 0
	if(flag_cursor_read_ADC_values == 0){
		x_adc_read = 0;
		y_adc_read = 0;
		z_adc_read = 0;
		flag_cursor_read_ADC_values = 1;
	}
	TM_ADC_Read(ADC1, ADC_Channel_4); //dummy ADC
	x_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);
	TM_ADC_Read(ADC1, ADC_Channel_4); //dummy ADC
	y_read_new = TM_ADC_Read(ADC1, ADC_Channel_1);
	TM_ADC_Read(ADC1, ADC_Channel_4); //dummy ADC
	z_read_new = TM_ADC_Read(ADC1, ADC_Channel_2);

	x_adc_read = Read_ADC_difference(&x_adc_read, &x_read_new);
	y_adc_read = Read_ADC_difference(&y_adc_read, &y_read_new);
	z_adc_read = Read_ADC_difference(&z_adc_read, &z_read_new);

	led_value_adc_x = x_adc_read / 1050;
	led_value_adc_y = y_adc_read / 1050;
	led_value_adc_z = z_adc_read / 1050;
}

void read_ADC_led()
{
	int color = 0x0000FF; //Blue standart color

	led_button_choosen = -1;

	Cursor_read_ADC_values();

	//Led Curzor on
	DisplayCrusor(1, color);
	//data_lvl1[led_value] = 4095;

	TM_BUTTON_Update();
	Delayms(100);

	//Led Curzor off
	DisplayCrusor(0, color);
}

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
 * @brief  Console application -> Print board
 * @param  Array to print
 * @param  Length of array
 * @retval None
 */
void PrintBoard(const int *board)
{
	int n;
	int board_value;
	int color = -1;
	for(n = 0; n < 16*4; n++){
		board_value = board[ConvertTo36_4[n]];
		if(board_value == NOUGHTS )
			color = 16;
			//data_lvl1[n + 16] = 4095;
		if(board_value == CROSSES )
			color = 0;
		 	//data_lvl1[n + 32] = 4095;

		if(color >= 0){
			switch(DetermineLayer(n)){

				case 1:
					data_lvl1[LedArrayOneLvl[n] + color] = 4095;
					color = -1;
				break;

				case 2:
					data_lvl2[LedArrayOneLvl[n-16] + color] = 4095;
					color = -1;
				break;

				case 3:
					data_lvl3[LedArrayOneLvl[n-32] + color] = 4095;
					color = -1;
				break;

				case 4:
					data_lvl4[LedArrayOneLvl[n-48] + color] = 4095;
					color = -1;
				break;
			}
		}
	}
}

/**
 * @brief  Empty all board
 * @param  None
 * @retval None
 */
void EmptyPrintBoard()
{
	int i;
	for(i = 0; i < 46; i++){
		data_lvl1[i] = 0;
		data_lvl2[i] = 0;
		data_lvl3[i] = 0;
		data_lvl4[i] = 0;
	}
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
 * @brief  CHECK FOR WIN: find if a player has a 4 in row
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

	for(DirIndex = 0; DirIndex < 11; ++DirIndex) {
		Dir = Directions[DirIndex];
		FourCount += GetNumForDir(ourindex + Dir, Dir, board, us);
		FourCount += GetNumForDir(ourindex + Dir * (-1), Dir * (-1), board, us);
		if(FourCount == 4) {
			DisplayWinner(board, ourindex, us, DirIndex);
			break;
		}
		FourCount = 1;
	}
	return FourCount;
}

/**
 * @brief  Display Winner
 * @param  Playing board
 * @param  Single position witch is confirm in the winning row
 * @param  Check crosses or noughts, depends on what our given position tile value is
 * @param  Direction to find all other 3 winning positions
 * @retval None
 */
void DisplayWinner( int *board, const int ourIndex, const int us, const int DirIndex)
{
	//TODO:
	int win_array[4];
	int n_p = 0;
	int n_m = 1;
	int i;
	int startSq;

	//Fill winning array with 4 board positions
	while(board[ourIndex + (n_p * Directions[DirIndex])] != BORDER){
		startSq = ourIndex + (n_p * Directions[DirIndex]);
		if(startSq > 144 || startSq < 0){ break;}
		win_array[n_p] = startSq;
		n_p++;
	}

	while(board[ourIndex - (n_m * Directions[DirIndex])] != BORDER){
		startSq = ourIndex - (n_m * Directions[DirIndex]);
		if(startSq > 144 || startSq < 0) break;
		win_array[n_m + (n_p - 1)] = startSq;
		n_m++;
	}
	for(i = 0; i < 5; i++){
		for(n_p = 0; n_p < 4;n_p++){
			board[win_array[n_p]] = 4;
		}
		EmptyPrintBoard();
		PrintBoard(&board[0]);
		Delayms(400);
		for(n_p = 0; n_p < 4;n_p++){
			board[win_array[n_p]] = us;
		}

		EmptyPrintBoard();
		PrintBoard(&board[0]);
		Delayms(400);
	}
}

///////////////////////////MINMAX////////////////////////////////////////

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
	return bestMove;
}

/**
 * @brief  Computer calculate his move
 * @param  Playing board
 * @param  Check crosses or noughts, depends on what our given position tile value is
 * @param  MinMax max. depth
 * @retval Move
 */
int GetComputerMove(int *board, const int side,const int max_depth)
{
	TIM_Cmd(TIM3, DISABLE);
	Move bestMove = findBestMove(board, side, max_depth);
	TIM_Cmd(TIM3, ENABLE);
	return bestMove.Move_position;
	/* RANDOM
	int index = 0;
	int numFree = 0;
	int availableMoves[16*4];
	int randMove = 0;

	for(index = 0; index < 16*4; ++index) {
		if( board[ConvertTo36_4[index]] == EMPTY) {
			availableMoves[numFree++] = ConvertTo36_4[index];
		};
	}

	randMove = (TM_RNG_Get() % numFree);
	return availableMoves[randMove];
	*/
}

/**
 * @brief  Console application-> Human adds his move
 * @param  Playing board
 * @retval human move
 */
int GetHumanMove(int *board, int Side, int depth)
{

	int moveOk = 0;
	//TODO:Poprav tale led_button_choosen!
	led_button_choosen = -1;

	while (moveOk == 0) {
		while(led_button_choosen < 0){
			read_ADC_led();
		}
		/*
		printf("Please enter a move from 1 to 16:");
		fgets(userInput, 3, stdin);
		fflush(stdin);

		if(strlen(userInput) != 2) {
			printf("Invalid strlen()\n");
			continue;
		}

		if( sscanf(userInput, "%d", &move) != 1) {
			move = -1;
			printf("Invalid sscanf()\n");
			continue;
		}

		if( move < 1 || move > 16) {
			move = -1;
			printf("Invalid range\n");
			continue;
		}
		*/
		// move--; // Zero indexing

		if( board[LedArray3D_TicTacToe[led_value_adc_z][led_value_adc_x][led_value_adc_y]]!= EMPTY) {
			//printf("Square not available\n");
			led_button_choosen = -1;
			continue;
		}
		moveOk = 1;
	}
	//printf("Making Move...%d\n",(move+1));
	return LedArray3D_TicTacToe[led_value_adc_z][led_value_adc_x][led_value_adc_y];
}

void RunGame(choosePlayer playerOne, choosePlayer playerTwo, int depth_ply1, int depth_ply2)
{
	flag_cursor_read_ADC_values = 0;
 	TM_RNG_Init();
	int GameOver = 0;
	int Side = NOUGHTS;
	int LastMoveMade = 0;

	int *board =  malloc(36*4* sizeof(int));
	if(!board) return;

	InitialiseBoard(&board[0]);
	PrintBoard(&board[0]);

	while(!GameOver) {
		if(Side==NOUGHTS) {
			LastMoveMade = playerOne(&board[0], Side, depth_ply2);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=CROSSES;
			PrintBoard(&board[0]);
		} else {
			LastMoveMade = playerTwo(&board[0], Side, depth_ply2);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}

		if( FindFourInARow(&board[0], LastMoveMade, Side ^ 1) == 4) {
			GameOver = 1;
		}

		// if no more moves, game is a draw
		if(!HasEmpty(board)) {
			GameOver = 1;
			Anim_TrikotDriveBy(0,0,1, get_random_color());
		}
	}

	PrintBoard(&board[0]);
	DeleteAllLeds();
	free(board);
}








