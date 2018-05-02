#ifndef TLC5940_H
#define TLC5940_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

#define NUM_TLCS 3
#define COUNT_TLC 3
/*SPI1

  *  GPIOA
  *  MISO -> GPIO_Pin_6
  *  MOSI -> GPIO_Pin_7
  *  SCL  -> GPIO_Pin_5
 */

 #define TLC_RCC_AHB1 RCC_AHB1Periph_GPIOB
 #define TLC5940_GPIO 	GPIOB
 #define PIN_BLANK		GPIO_Pin_5
 #define PIN_VPRG 		GPIO_Pin_9
 #define PIN_XLAT 		GPIO_Pin_6
 #define PIN_GSCLK		GPIO_Pin_3


#define TLC5940_GPIO 	GPIOB
#define PORTx_SPI 	 GPIOA
#define DD_MOSI 	 GPIO_Pin_7		//SIN -> MOSI
#define DD_SCK 		 GPIO_Pin_5		//Clock SCLK


#define PORTx_TLC5940 	GPIOB
#define DD_XLAT		 	GPIO_Pin_6
#define DD_BLANK	 	GPIO_Pin_5
#define DD_VPRG			GPIO_Pin_9		//MISO -> VPRG


#define PORTx_GSCLK 	GPIOB
#define DD_GSCLK 		GPIO_Pin_3	//ta je PWM ju definiran

#define PORTx_PIN_LVL 	GPIOD
#define PIN_LEVEL_1  	GPIO_Pin_7
#define PIN_LEVEL_2	    GPIO_Pin_5
#define PIN_LEVEL_3 	GPIO_Pin_3
#define PIN_LEVEL_4 	GPIO_Pin_1
/**
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
 *
 *
 *
 *
 *
 * */


/**
 * @brief  Sets pin(s) high
 * @note   Defined as macro to get maximum speed using register access
 * @param  GPIOx: GPIOx PORT where you want to set pin high
 * @param  GPIO_Pin: Select GPIO pin(s). You can select more pins with | (OR) operator to set them high
 * @retval None
 */
#define GPIO_SetPinHigh(GPIOx, GPIO_Pin) 		((GPIOx)->BSRRL = (GPIO_Pin))

/**
 * @brief 	Set pin(s) low
 * @note 	Define as a macro to get maximum speed using register acess
 * @param	GPIOx: Port for which pin is set low
 * @param	GPIO_Pin: Select GPIO pin. You can select more pins with | (OR) operator to set them low
 * @retval	None
 */
#define GPIO_SetPinLow(GPIOx, GPIO_Pin) 		((GPIOx)->BSRRH = (GPIO_Pin))


//array holding the data to be shifted in the driver
uint16_t leds[15];

uint16_t data_lvl1[COUNT_TLC * 16];
uint16_t data_lvl2[COUNT_TLC * 16];
uint16_t data_lvl3[COUNT_TLC * 16];
uint16_t data_lvl4[COUNT_TLC * 16];

//value for timer 2 GSCLK ?!? for now
volatile uint16_t clkCnt;
//timer for the PWM output
void TIM2_IRQHandler(void);

//Define functions
void TLC_AllClock_Init(void);
void setTLCChannel(uint8_t channel, uint16_t val);
void Timer_Init(void);
void TLC_AllClock_Init(void);
void TLC_Pin_Init(void);
void PWM_Timer_Init_TLC(void);
void resetTLC();
void writeTLC(uint8_t data);
void updateTLC();

/*Nove Funkcije*/

void init_tim7(void);
void TIM7_IRQHandler(void);
uint8_t SPI1_send(uint8_t data);
void init_SPI1(void);
void Tlc5940_setAllDC(uint8_t value);
void Blank_Pulse(void);
void TLC_One_Led_On(uint8_t led_num);
void TLC_here(void);
void GSCLK_Pulzes(void);
#endif
