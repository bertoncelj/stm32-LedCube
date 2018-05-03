/*
 * tic_tac_toe.c
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"

int led_value;
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

	 0, 1, 2, 3, 4,
	 5, 6, 7, 8, 9,
	10,11,12,13,14,
	15,16,17,18,19,
	20,21,22,23,24
}
*/
int board[36];
const int Directions[4] = { 1, 6, 5, 7};

const int ConvertTo36[16] = {
	 7, 8, 9,10,
	13,14,15,16,
	19,20,21,22,
	25,25,27,28
};

void BUTTON1_EventHandler(TM_BUTTON_PressType_t type) {
	/* Check button */
	if (type == TM_BUTTON_PressType_OnPressed) {
		//data_lvl1[led_value] = 4095;
	} else if (type == TM_BUTTON_PressType_Normal) {
		data_lvl1[led_value + 16] = 4095;
		led_button_choosen = led_value;
	} else {
		data_lvl1[led_value + 16] = 0;

	}
}

void read_ADC_led(){

	int adc_rvalue;
	led_button_choosen = -1;

	adc_rvalue = TM_ADC_Read(ADC1, ADC_Channel_0);

	led_value = adc_rvalue/ 256;
	data_lvl1[led_value] = 4095;
	TM_BUTTON_Update();
	Delayms(100);

	data_lvl1[led_value] = 0;
}


int GetNumForDir(int startSq, const int dir, const int *board, const int us) {
	int found = 0;
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
int FindFourInARow(const int *board, const int ourindex, const int us) {

	int DirIndex = 0;
	int Dir = 0;
	int FourCount = 1;

	for(DirIndex = 0; DirIndex < 4; ++DirIndex) {
		Dir = Directions[DirIndex];
		FourCount += GetNumForDir(ourindex + Dir, Dir, board, us);
		FourCount += GetNumForDir(ourindex + Dir * -1, Dir * -1, board, us);
		if(FourCount == 4) {
			break;
		}
		FourCount = 1;
	}
	return FourCount;
}
//OK
void InitialiseBoard(int *board) {
	int index = 0;

	for(index = 0; index < 36; ++index) {
		board[index] = BORDER;
	}

	for(index = 0; index < 16; ++index) {
		board[ConvertTo36[index]] = EMPTY;
	}
}
//OK
void PrintBoard(const int *board) {

	int n;
	int board_value;
	for(n = 0; n < 16; n++){
		board_value = board[ConvertTo36[n]];
		if(board_value == NOUGHTS )
			data_lvl1[led_value + 16] = 4095;
		if(board_value == CROSSES )
			data_lvl1[led_value + 32] = 4095;
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
//OK
int HasEmpty(const int *board) {
	int index = 0;

	for(index = 0; index < 16; ++index) {
		if( board[ConvertTo36[index]] == EMPTY) return 1;
	}
	return 0;
}
//OK
void MakeMove(int *board, const int sq, const side) {
	board[sq] = side;
}
//OK

/*

	 0, 1, 2, 3, 4, 5
	 6, 7, 8, 9,10,11
	12,13,14,15,16,17
	18,19,20,21,22,23
	24,25,26,27,28,29
	30,31,32,33,34,35

     0, 1, 2, 3, 4, 5
	 6, X, 8, 9,10,11
	12,13, X,15,16,17
	18,19,20,21,22,23
	24, X, X,27,28,29
	30,31,32,33,34,35

 */
int GetComputerMove(const int *board) {
	int index = 0;
	int numFree = 0;
	int availableMoves[16];
	int randMove = 0;

	for(index = 0; index < 16; ++index) {
		if( board[ConvertTo36[index]] == EMPTY) {
			availableMoves[numFree++] = ConvertTo36[index];
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

		if( board[ConvertTo36[led_button_choosen]]!=EMPTY) {
			//printf("Square not available\n");
			led_button_choosen = -1;
			continue;
		}
		moveOk = 1;
	}
	//printf("Making Move...%d\n",(move+1));
	return ConvertTo36[led_button_choosen];
}

void RunGame() {

	int GameOver = 0;
	int Side = NOUGHTS;
	int LastMoveMade = 0;


	InitialiseBoard(&board[0]);
	PrintBoard(&board[0]);

	while(!GameOver) {
		if(Side==NOUGHTS) {
			LastMoveMade = GetHumanMove(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=CROSSES;
		} else {
			LastMoveMade = GetComputerMove(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side);
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}

		// if three in a row exists Game is over
		if( FindFourInARow(board, LastMoveMade, Side ^ 1) == 4) {
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
}


void Rungame_here()
{
	TM_RNG_Init();
	RunGame();
}














