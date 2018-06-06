
#include <stm32f4xx_rcc.h>
#include "tm_stm32f4_delay.h"
#include "tlc5940.h"
#include "tic_tac_toe.h"
#include <stdio.h>
#include "tm_stm32f4_rng.h"
#include "menu.h"
#include "tm_stm32f4_hd44780.h"

int timer_update=0;

void initTLC()
{
	 SystemInit(); //168mhz
	 init_TLC_Pins_SPI();
	 Timer_Init(); //GSCLK + BLANK

	 // 64-step constant_current sink.The dot correction adjusts the brightness
	 // variations between LED channels and other LED  drivers.
	 Tlc5940_setAllDC(63);	//max 63

	 //VPRG low, so we can send GSCLK data
	 GPIO_SetPinLow(PORTx_TLC5940,PIN_VPRG);

	 //make sure cube is off in start
	 GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_1);
	 GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_2);
	 GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_3);
	 GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_4);
}


int main(void)
{
	//DELAY_INIT
	TM_DELAY_Init();

	//RANDOM INIT
	TM_RNG_Init();

	//TLC INIT
	initTLC();

	//LCD INIT
	TM_HD44780_Init(16, 2);

	//POTENCIOMETER INIT
	TM_ADC_Init(ADC1, ADC_Channel_4);
	TM_ADC_Init(ADC1, ADC_Channel_0);
	TM_ADC_Init(ADC1, ADC_Channel_1);
	TM_ADC_Init(ADC1, ADC_Channel_2);

	//BUTTON_OK INIT
	TM_BUTTON_Init(GPIOE, GPIO_Pin_4, 1, BUTTON_OK_EventHandler);

	//Start refreshing cube
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	while(1) menu();
}
