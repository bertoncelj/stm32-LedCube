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

typedef int (*choosePlayer)( int *board, const int side, const int max_dept);
/* ARRAY BOARD*/

typedef struct move
{
	int Move_position;
	int bestVal;
}Move;

typedef struct {
	int led_value_adc_x;
	int led_value_adc_y;
	int led_value_adc_z;
	int displayCrusorON_OFF;
}CrusorValues;

int LedArrayOneLvl[16];
int led_button_choosen;

extern void TM_RNG_Init(void);
extern uint32_t TM_RNG_Get(void);


/*Functions*/

//Button function
void BUTTON_OK_EventHandler(TM_BUTTON_PressType_t type);
void read_ADC_led(CrusorValues *newCrusorValues);
int  Read_ADC_difference(int *old, int *new);
void EmptyPrintBoard(void);
int  HasEmpty(const int *board);
void PrintBoard(const int *board);
void DisplayCrusor(CrusorValues *newCrusorValues);

/*Tic tac toe*/
int  GetNumForDir(int startSq, const int dir, const int *board, const int us);
int  FindFourInARow(int *board, const int ourindex, const int us);
void InitialiseBoard(int *board);
int  DetermineLayer(const int n);
void MakeMove(int *board, const int sq,  int side);
int  GetHumanMove   ( int *board, int Side, int depth);
int  GetComputerMove( int *board, const int side, int max_depth);

void RunGame(choosePlayer playerOne, choosePlayer playerTwo, int depth_ply1, int depth_ply2);
void DisplayWinner( int *board, const int ourIndex, const int us, const int DirIndex);

/*MinMax*/
int eval_comp_move(int *board, int moveImake, const int side);
int FindCleanInARoW(int *board, int my_move, int side);
int minimax(int *board, int depth, int isMax, int moveImake, int save_score, int max_depth);
int GetNumForDirClean(int startSq, const int dir, const int *board, int us);
int isItCorner(int moveImake);
int detecCombo(int newScore);

#endif /* TIC_TAC_TOE_H_ */
