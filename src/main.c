
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_spi.h>
#include <misc.h>
#include "tm_stm32f4_delay.h"
#include "tlc_animations.h"
#include "tm_stm32f4_button.h"
#include "tlc5940.h"
#include "tic_tac_toe.h"


/*
 * Pin configuration
 *
 * 		OUT2		OUT1				tlc5940	tlc5940stm
 * 		OUT3	D-> VPRG		PB4		VPRG	MISO
 * 		OUT4		SIN			PD7		SS		MOSI
 * 		OUT5		SCLK		PD5		SCK 	CLK
 * 		OUT6	D->	XLAT		PD3		XLAT	XLAT
 * 		OUT7	D->	BLANK		PD1		BLANK	BLANK
 * 		OUT8	D->	GND
 * 		OUT9	D->	VCC
 * 		OUT10	D->	IREF
 * 		OUT11	D->	DCPRG
 * 		OUT12	D->	GSCLK		PA0		GSCL	PWM GPIO_Pin_0
 * 		OUT13		SOUT
 * 		OUT14		XERR
 * 		OUT15		OUT16
 */
 #define TLC_RCC_AHB1 RCC_AHB1Periph_GPIOB
 #define TLC5940_GPIO 	GPIOB
 #define PIN_BLANK		GPIO_Pin_5
 #define PIN_VPRG 		GPIO_Pin_9
 #define PIN_XLAT 		GPIO_Pin_6
 #define PIN_GSCLK		GPIO_Pin_3


int timer_update=0;
//b r g
//g b r
int main(void){
 SystemInit(); //168mhz
 //init_tim7();
 init_SPI1();
 Timer_Init(); //GSCLK + BLANK
 TM_DELAY_Init();
 // MCO2_INIT();

 uint_fast8_t n;
 //LED0
 u16 data[COUNT_TLC* 16]={0x0000 ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

 Tlc5940_setAllDC(63);	//max 63
 //GPIO_SetPinLow(TLC5940_GPIO,PIN_LEVEL_1);
 GPIO_SetPinLow(TLC5940_GPIO,PIN_VPRG);

    GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_1);
 	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_2);
 	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_3);
 	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_4);

 //Test
 /*GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);
 for(n=0; n<(COUNT_TLC *16); n=n+2)
		 {

			 data[n] = 4095/16*n;
			 data[n+1] = 4095/16*(n+1);
			 SPI1_send(data[n]>>4);
			 SPI1_send((data[n]<<4) | (data[n+1])>>8);
			 SPI1_send(data[n+1]);
		 }

		 GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 (CS) high
		 GPIO_ResetBits(GPIOD,GPIO_Pin_12);
		 GPIOE->BSRRH |= GPIO_Pin_7;

		 _delay(1);
		 GPIO_SetPinHigh(TLC5940_GPIO,PIN_XLAT);
		 _delay(1);
		 GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
		 GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);
		 _delay(1);
 //EndTest
			int i;
					 //generate GSCLK
				for (i = 0; i < 4096; i++){
					TLC5940_GPIO->BSRRL = PIN_GSCLK;
					 TLC5940_GPIO->BSRRH = PIN_GSCLK;
				}
				 Blank_Pulse();

		*/
 //ADC init
 //TM_ADC_Init(ADC1, ADC_Channel_4);
 //TM_ADC_Init(ADC1, ADC_Channel_1);
 //TM_ADC_Init(ADC1, ADC_Channel_2);

 //TM_BUTTON_Init(GPIOE, GPIO_Pin_4, 1, BUTTON1_EventHandler);

 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
 TIM_Cmd(TIM3, ENABLE);
int i;
// Anim_Loytra(0,0,0, ROZI);
int del_time = 2000;
 while(1){

	  Anim_Quatro_4Squars_Infinity();
	// Delayms(del_time);
	//BasicAnim_RGB_All();
	 //BasicAnim_RGB_All();

//NOTE: Porpravi timer 3 prescaler and period!


	 /*

	 while(timer_update>20){

		 	 data[15] +=16;
		 	 data[14] +=16;
			 data[13] +=16;
			 data[12] +=16;
			 data[11] +=16;
			 data[10] +=16;
			 data[9] +=16;
			 data[8] +=16;
			 data[7] +=16;
			 data[6] +=16;
			 data[5] +=16;
			 data[4] +=16;
			 data[3] +=16;
			 data[2] +=16;
			 data[1] +=16;





			 GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);

			 for(n=0; n<(COUNT_TLC *16); n=n+2)
			 {

				 data[n] = 4095/16*n;
				 data[n+1] = 4095/16*(n+1);
				 SPI1_send(data[n]>>4);
				 SPI1_send((data[n]<<4) | (data[n+1])>>8);
				 SPI1_send(data[n+1]);
			 }

			 GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 (CS) high
			 GPIO_ResetBits(GPIOD,GPIO_Pin_12);
			 GPIOE->BSRRH |= GPIO_Pin_7;



			 _delay(1);
			 GPIO_SetPinHigh(TLC5940_GPIO,PIN_XLAT);
			 _delay(1);
			 GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
			 GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);
			 _delay(1);

			 timer_update=0;
		}

		int i;
			 //generate GSCLK
		for (i = 0; i < 4096; i++){
			TLC5940_GPIO->BSRRL = PIN_GSCLK;
			 TLC5940_GPIO->BSRRH = PIN_GSCLK;
		}
			 Blank_Pulse();
			 */
 }
}
