/*
 * tic_tac_toe.h
 *
 *  Created on: May 3, 2018
 *      Author: tine
 */

#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_button.h"
#include "tlc5940.h"

enum { NOUGHTS, CROSSES, BORDER, EMPTY };
enum { HUMANWIN, COMPWIN, DRAW };


//Button function
void BUTTON1_EventHandler(TM_BUTTON_PressType_t type);


#endif /* TIC_TAC_TOE_H_ */
