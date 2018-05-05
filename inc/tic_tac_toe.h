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

enum { NOUGHTS, CROSSES, BORDER, EMPTY, LEDOFFN, LEDOFFC};
enum { HUMANWIN, COMPWIN, DRAW };


/* ARRAY BOARD*/



extern void TM_RNG_Init(void);
extern uint32_t TM_RNG_Get(void);

//Button function
void BUTTON1_EventHandler(TM_BUTTON_PressType_t type);

/*Functions*/

void read_ADC_led();
int GetNumForDir(int startSq, const int dir, const int *board, const int us);
int FindFourInARow(int *board, const int ourindex, const int us);
void InitialiseBoard(int *board) ;
void PrintBoard(const int *board);
int HasEmpty(const int *board);
void MakeMove(int *board, const int sq, const side);
int GetComputerMove(const int *board);
int GetHumanMove(const int *board);
void RunGame();
void DisplayCrusor( int on);
void DisplayWinner( int *board, const int ourIndex, const int us, const int DirIndex);
void EmptyPrintBoard();
#endif /* TIC_TAC_TOE_H_ */
