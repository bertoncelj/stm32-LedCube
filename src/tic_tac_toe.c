/*
 * tic_tac_toe.c
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"
#include "tlc_animations.h"

int led_value_16;
int led_value_adc_x;
int led_value_adc_y;
int led_value_adc_z;


uint8_t flag_cursor_read_ADC_values;
/*
int board[25] = {
	:,:,:,:,:,
	:,O,-,X,:,
	:,X,-,-,:,
	:,-,-,-,:,
	:,:,:,:,:,

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
	int dummy;
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
	dummy = TM_ADC_Read(ADC1, ADC_Channel_4);
	x_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);
	dummy = TM_ADC_Read(ADC1, ADC_Channel_4);
	y_read_new = TM_ADC_Read(ADC1, ADC_Channel_1);
	dummy = TM_ADC_Read(ADC1, ADC_Channel_4);
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

//TODO:
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

//OK
void InitialiseBoard(int *board)
{
	int index = 0;

	for(index = 0; index < 36*4; ++index) {
		board[index] = BORDER;
	}

	for(index = 0; index < 16*4; ++index) {
		board[ConvertTo36_4[index]] = EMPTY;
	}
}

int DetermineLayer(const int n )
{
	if(n < 16) return 1;
	if(16 <= n && n < 32) return 2;
	if(32 <= n && n < 48) return 3;
	if(48 <= n && n < 64) return 4;

	return 0;
}

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

//OK
int HasEmpty(const int *board)
{
	int index = 0;

	for(index = 0; index < 16*4; ++index) {
		if( board[ConvertTo36_4[index]] == EMPTY) return 1;
	}
	return 0;
}

//OK
void MakeMove(int *board, const int sq, int side)
{
	board[sq] = side;
}

int GetComputerMove(const int *board) {
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
}

/////////////////////////////////////////////////////////////////
int GetHumanMove(const int *board)
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

void RunGame(choosePlayer playerOne, choosePlayer playerTwo)
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

	//delete me
	led_button_choosen = -1;

	while(!GameOver) {
		if(Side==NOUGHTS) {
			LastMoveMade = playerOne(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=CROSSES;
			PrintBoard(&board[0]);



		} else {
			LastMoveMade = playerTwo(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}


		//Delayms(200);
		// if three in a row exists Game is over
		if( FindFourInARow(&board[0], LastMoveMade, Side ^ 1) == 4) {
			//printf("Game over!\n");
			GameOver = 1;
			if(Side==NOUGHTS) {
				//printf("Computer Wins\n");
			} else {
				//printf("Human Wins\n");
			}
		}

		// if no more moves, game is a draw
		if(!HasEmpty(board)) {
			//printf("Game over!\n");
			GameOver = 1;
			Anim_matrix();
			//printf("It's a draw\n");
		}
	}

	PrintBoard(&board[0]);
	DeleteAllLeds();
	free(board);
}


void Rungame_here()
{
	//TODO:magar puprav to ->trenutno je flag za adc, ker neznam drugac narest

	//RunGame();

}














