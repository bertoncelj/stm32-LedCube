#ifndef TLC5940_H
#define TLC5940_H

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

//
/**
 * Pin configuration TLC5940
 *
 * 		OUT2		OUT1				tlc5940	tlc5940stm
 * 		OUT3	D-> VPRG		PIN_VPRG	 GPIO_Pin_9
 * 		OUT4	D->	SIN			PIN_MOSI 	 GPIO_Pin_7
 * 		OUT5	D->	SCLK		PIN_SCK  	 GPIO_Pin_5
 * 		OUT6	D->	XLAT		PIN_XLAT     GPIO_Pin_6
 * 		OUT7	D->	BLANK		PIN_BLANK	 GPIO_Pin_5
 * 		OUT8		GND			GND
 * 		OUT9		VCC			3V
 * 		OUT10		IREF		4kOhm TO GND
 * 		OUT11		DCPRG		3V
 * 		OUT12	D->	GSCLK		PIN_GSCLK	 GPIO_Pin_3
 * 		OUT13		SOUT
 * 		OUT14		XERR
 * 		OUT15		OUT16
 */

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

//MAIN 4 ARRAYS FOR LAYERS
uint16_t data_lvl1[NUM_TLCS * 16];
uint16_t data_lvl2[NUM_TLCS * 16];
uint16_t data_lvl3[NUM_TLCS * 16];
uint16_t data_lvl4[NUM_TLCS * 16];

volatile uint16_t clkCnt;

/* FUNCTIONS */
void Timer_Init(void);
uint8_t SPI1_send(uint8_t data);
void init_TLC_Pins_SPI(void);
void Tlc5940_setAllDC(uint8_t value);
void TLC_Update_lvl(uint16_t *data_lvl);
void TLC_Cube_Update(void);
void GSCLK_Pulzes(void);

void _delay(int value);

//One pulzs
void Blank_Pulse(void);
void GSCLK_Pulse(void);
void Xlat_Pulse(void);

#endif
