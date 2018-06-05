/*
 * tic_tac_toe.h
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_rng.h"
#include "tlc5940.h"

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
typedef int (*choosePlayerLvl)(const int *board, const int side, const int max_depth);
/* ARRAY BOARD*/



typedef struct move
{
	int Move_position;
	int bestVal;
}Move;





LedArrayOneLvl[16];
int led_button_choosen;

extern void TM_RNG_Init(void);
extern uint32_t TM_RNG_Get(void);



/*Functions*/

/* Stm32 Cube*/

//Button function
void BUTTON_OK_EventHandler(TM_BUTTON_PressType_t type);
void read_ADC_led();
void EmptyPrintBoard(void);
int HasEmpty(const int *board);
void PrintBoard(const int *board);
void DisplayCrusor(int on, int color);

//int GetComputerMove(const int *board);
//int GetHumanMove(const int *board);

/*Tic tac toe*/
int GetNumForDir(int startSq, const int dir, const int *board, const int us);
int FindFourInARow(int *board, const int ourindex, const int us);
void InitialiseBoard(int *board) ;
void MakeMove(int *board, const int sq,  int side);
int GetHumanMove(const int *board);
int GetComputerMove(int *board, const int side, int max_depth);

void RunGame(choosePlayer playerOne, choosePlayer playerTwo);
void DisplayCrusor(int on, int color);
void DisplayWinner( int *board, const int ourIndex, const int us, const int DirIndex);


/*MinMax*/
int eval_comp_move(int *board, int moveImake, const int side);
int FindCleanInARoW(int *board, int my_move, int side);
int minimax(int *board, int depth, int isMax, int moveImake, int save_score, int max_depth);
int GetNumForDirClean(int startSq, const int dir, const int *board, int us);
int isItCorner(int moveImake);
int detecCombo(int newScore);

#endif /* TIC_TAC_TOE_H_ */
