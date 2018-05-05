/*
 * tic_tac_toe.c
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"

int led_value_16;
int led_value_lvl;
int led_button_choosen;
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
const int Directions[9] = { 1, 6, 5, 7, 36, 37, 35, 43, 41};

//NOTE: REVERSE TO MATCH LED DIRECTIONS
const int ConvertTo36_4[16*4] = {
	10, 9, 8,7,
	13,14,15,16,
	22,21,20,19,
	25,26,27,28,

	46,45,44,43,
	49,50,51,52,
	58,57,56,55,
	61,62,63,64,

	83,82,81,80,
	85,86,87,88,
	94,93,92,91,
	97,98,99,100,

	118,117,116,115,
	121,122,123,124,
	130,129,128,127,
	133,134,135,136

};

void BUTTON1_EventHandler(TM_BUTTON_PressType_t type) {
	/* Check button */
	if (type == TM_BUTTON_PressType_OnPressed) {
		//data_lvl1[led_value] = 4095;
	} else if (type == TM_BUTTON_PressType_Normal) {
		//data_lvl1[led_value + 16] = 4095;
		led_button_choosen = led_value_16;
		//led_button_choosen = led_value_lvl;
	} else {
		//data_lvl1[led_value + 16] = 0;

	}
}
void DisplayCrusor(int on)
{
	int led_on_off;
	if(on == 1){
		led_on_off = 4095;
	} else {
		led_on_off = 0;
	}

	switch(led_value_lvl){

	case 0:
		data_lvl1[led_value_16] = led_on_off;
	break;

	case 1:
		data_lvl2[led_value_16] = led_on_off;
	break;

	case 2:
		data_lvl3[led_value_16] = led_on_off;
	break;

	case 3:
		data_lvl4[led_value_16] = led_on_off;
	break;


	}


}

void read_ADC_led(){

	int adc_rvalue_16;
	int adc_rvalue_lvl;
	led_button_choosen = -1;

	adc_rvalue_16 = TM_ADC_Read(ADC1, ADC_Channel_0);
	adc_rvalue_lvl = TM_ADC_Read(ADC1, ADC_Channel_1);

	led_value_16 = adc_rvalue_16/ 256;
	led_value_lvl = adc_rvalue_lvl/1000;

	//Led Curzor on
	DisplayCrusor(1);
	//data_lvl1[led_value] = 4095;

	TM_BUTTON_Update();
	Delayms(100);

	//Led Curzor off
	DisplayCrusor(0);
}

void DisplayWinner( int *board, const int ourIndex, const int us, const int DirIndex)
{

	//TODO:
	int win_array[4];
	int n_p = 0;
	int n_m = 1;
	int i;

	//Fill winning array with 4 board positions
	while(board[ourIndex + (n_p * Directions[DirIndex])] != BORDER){
		win_array[n_p] = ourIndex + (n_p * Directions[DirIndex]);
		n_p++;
	}

	while(board[ourIndex - (n_m * Directions[DirIndex])] != BORDER){
		win_array[n_m] = ourIndex - (n_m * Directions[DirIndex]);
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

int GetNumForDir(int startSq, const int dir, const int *board, const int us) {
	int found = 0;
	if(startSq > 144 || startSq < 0) return found;
	while(board[startSq] != BORDER) {
		if(board[startSq] != us) {
			break;
		}
		found++;
		startSq += dir;
	}
	return found;
}
//TODO:
int FindFourInARow( int *board, const int ourindex, const int us) {

	int DirIndex = 0;
	int Dir = 0;
	int FourCount = 1;

	for(DirIndex = 0; DirIndex < 9; ++DirIndex) {
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
void InitialiseBoard(int *board) {
	int index = 0;

	for(index = 0; index < 36*4; ++index) {
		board[index] = BORDER;
	}

	for(index = 0; index < 16*4; ++index) {
		board[ConvertTo36_4[index]] = EMPTY;
	}
}

int DetermineLayer(const int n ){
	if(n < 16) return 1;
	if(16 <= n && n < 32) return 2;
	if(32 <= n && n < 48) return 3;
	if(48 <= n && n < 64) return 4;

	return 0;
}

void PrintBoard(const int *board) {

	int n;
	int board_value;
	int color = 0;
	for(n = 0; n < 16*4; n++){
		board_value = board[ConvertTo36_4[n]];
		if(board_value == NOUGHTS )
			color = 16;
			//data_lvl1[n + 16] = 4095;
		if(board_value == CROSSES )
			color = 32;
		 	//data_lvl1[n + 32] = 4095;

		if(color > 0){
			switch(DetermineLayer(n)){

				case 1:
					data_lvl1[n + color] = 4095;
					color = 0;
				break;

				case 2:
					data_lvl2[(n-16) + color] = 4095;
					color = 0;
				break;

				case 3:
					data_lvl3[(n-32) + color] = 4095;
					color = 0;
				break;

				case 4:
					data_lvl4[(n-48) + color] = 4095;
					color = 0;
				break;
			}
		}
	}

	/*
	int index = 0;
	char pceChars[] = "OX|-";

	printf("\n\nBoard:\n\n");
	for(index = 0; index < 16; ++index) {
		if(index != 0 && index%4 == 0) { //index%4 ??
			printf("\n\n");
		}
		printf("%4c",pceChars[board[ConvertTo36[index]]]);
	}
	printf("\n");
	*/
}

void EmptyPrintBoard(){
	int i;
	for(i = 0; i < 46; i++){
		data_lvl1[i] = 0;
		data_lvl2[i] = 0;
		data_lvl3[i] = 0;
		data_lvl4[i] = 0;
	}
}
//OK
int HasEmpty(const int *board) {
	int index = 0;

	for(index = 0; index < 16*4; ++index) {
		if( board[ConvertTo36_4[index]] == EMPTY) return 1;
	}
	return 0;
}
//OK
void MakeMove(int *board, const int sq, const side) {
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
int GetHumanMove(const int *board) {
	int moveOk = 0;
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

		if( board[ConvertTo36_4[led_value_16 + led_value_lvl*16]]!= EMPTY) {
			//printf("Square not available\n");
			led_button_choosen = -1;
			continue;
		}
		moveOk = 1;
	}
	//printf("Making Move...%d\n",(move+1));
	return ConvertTo36_4[led_value_16 + led_value_lvl*16];
}

void RunGame() {

	int GameOver = 0;
	int Side = NOUGHTS;
	int LastMoveMade = 0;

	int *board =  malloc(36*4* sizeof(int));
	if(!board) return;

	InitialiseBoard(&board[0]);
	PrintBoard(&board[0]);

	while(!GameOver) {
		if(Side==NOUGHTS) {
			LastMoveMade = GetHumanMove(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=CROSSES;
			PrintBoard(&board[0]);
		} else {
			LastMoveMade = GetHumanMove(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}
		Delayms(200);

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
			//printf("It's a draw\n");
		}
	}

	PrintBoard(&board[0]);
	free(board);
}


void Rungame_here()
{
 	TM_RNG_Init();
	RunGame();
}














