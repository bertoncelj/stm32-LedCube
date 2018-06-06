/**
 *  Defines for your entire project at one place
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

//
/**
 * * ---------------------------------------------------
 * 						TLC5940
 *   ---------------------------------------------------
 * Pin configuration TLC5940
 *
 * 		OUT2		OUT1		tlc5940	     stm32f4Discovery
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

#define NUM_TLCS 3

#define TLC_RCC_AHA1 RCC_AHB1Periph_GPIOA
#define TLC_RCC_AHB1 RCC_AHB1Periph_GPIOB
#define TLC_RCC_AHD1 RCC_AHB1Periph_GPIOD

#define PORTx_TLC5940 	GPIOB
#define PIN_BLANK		GPIO_Pin_5
#define PIN_VPRG 		GPIO_Pin_9
#define PIN_XLAT 		GPIO_Pin_6

#define PORTx_GSCLK 	GPIOB
#define PIN_GSCLK		GPIO_Pin_3

#define PORTx_SPI 		GPIOA
#define PIN_MOSI 	 	GPIO_Pin_7		//SIN -> MOSI
#define PIN_SCK  	 	GPIO_Pin_5	    //Clock SCLK

#define PORTx_PIN_LVL 	GPIOD
#define PIN_LEVEL_1  	GPIO_Pin_7
#define PIN_LEVEL_2	    GPIO_Pin_0
#define PIN_LEVEL_3 	GPIO_Pin_3
#define PIN_LEVEL_4 	GPIO_Pin_1

//Timer refresh rate
#define TIMER_PERIOD 27
#define TIMER_PRESCALER 59999


/*
 *   ---------------------------------------------------
 * 						HD44780
 *   ---------------------------------------------------
 */

/* Control pins for HD44780 */
/* RS - Register select pin */
#define HD44780_RS_PORT			GPIOB
#define HD44780_RS_PIN			GPIO_Pin_11
/* E - Enable pin */
#define HD44780_E_PORT			GPIOE
#define HD44780_E_PIN			GPIO_Pin_15

/* D4 - Data 4 pin */
#define HD44780_D4_PORT			GPIOE
#define HD44780_D4_PIN			GPIO_Pin_13
/* D5 - Data 5 pin */
#define HD44780_D5_PORT			GPIOE
#define HD44780_D5_PIN			GPIO_Pin_11
/* D6 - Data 6 pin */
#define HD44780_D6_PORT			GPIOE
#define HD44780_D6_PIN			GPIO_Pin_9
/* D7 - Data 7 pin */
#define HD44780_D7_PORT			GPIOE
#define HD44780_D7_PIN			GPIO_Pin_7


#endif
