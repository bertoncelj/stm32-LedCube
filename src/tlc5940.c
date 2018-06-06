#include "tlc5940.h"
#include "tm_stm32f4_delay.h"
#include "defines.h"

/** \mainpage
    The <"http://www.ti.com/lit/gpn/TLC5940">Texas Instruments TLC5940
    is a 16-channel, constant-current sink LED driver.  Each channel has
    an individually adjustable 4096-step grayscale PWM brightness control and
    a 64-step, constant-current sink (no LED resistors needed!).  This chip
    is a current sink, so be sure to use common anode RGB LEDs.

    All the options for the library are located in defines.h, including
    #NUM_TLCS, what pins to use, and the PWM period.
*/
uint8_t TLC5940_count;
uint16_t *TLC5940_grayScale;

extern int timer_update;

//replace TIM3 with your desired Timer...
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TLC_Cube_Update();
}

/**
 * Cycle through 0-4045 pulse width modulation (PWM) values for grayscale brightness control.
 * First all data loaded into Input Shift Register is pushed onward by VPRG pulse.
 * Then GCLK is pulsing 4096 times to get over all his data values. This signal can be as high as 30 MHz.
 * Setting BLANK to high and then low causes the counter to be set to zero.
 * An incoming clock pulse causes the counter to be incremented by 1.
 * The chip then turns on all of the outputs which have a greyscale value which is non-zero.
 * Each subsequent pulse cause the chip to do the following:
 *	1.) Increment the counter
	2.)For each output compare the counter with the greyscale value for the output pin.
	If the value is greater than the counter then turn the output on, else turn the output off.
 * */
void GSCLK_Pulzes()
{
	int i;
	PORTx_TLC5940->BSRRL = PIN_VPRG;
	PORTx_TLC5940->BSRRH = PIN_VPRG;
	for (i = 0; i < 4096; i++){
		PORTx_TLC5940->BSRRL = PIN_GSCLK;
		PORTx_TLC5940->BSRRH = PIN_GSCLK;
	}
	Blank_Pulse();
}
/**
 * Change TIM3 to your preferred timer.
 * TIMER_PERIOD and TIMER_PERIOD are set in defines.h
 * They are set for 20ms timer interrupt.
 * */
void Timer_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_Period = TIMER_PERIOD;  	   		//     for 20ms interrupt
    TIM_TimeBase_InitStructure.TIM_Prescaler = TIMER_PRESCALER ;   //
    TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);
}

/**Setting DOT Correction
 * The TLC5940 has the capability to fine adjust the output current of each channel OUT0 to OUT15 independently.
 *	This is also called dot correction. This feature is used to adjust the brightness deviations of LEDs connected to
 *	the output channels OUT0 to OUT15. Each of the 16 channels can be programmed with a 6-bit word. The
 *	channel output can be adjusted in 64 steps from 0% to 100% of the maximum output current Imax. Dot correction
 *	for all channels must be entered at the same time. Equation 7 determines the output current for each output n.
 *
 *	VPRG must be set HIGH, and BLANK HIGH to shut down output. As data is send in 8-bits it must be stitch together
 *	by 6 bit from first data plus 2 bit from second, next is other left 4-bits from second data and 2-bits from third.
 *
 *	A low-high XLAT pulse on this pin latches the data received from the microcontroller into the DC register.
 * */
void Tlc5940_setAllDC(uint8_t value)
{
	int i;
    uint8_t firstByte = value << 2 | value >> 4;
    uint8_t secondByte = value << 4 | value >> 2;
    uint8_t thirdByte = value << 6 | value;

	GPIO_SetPinHigh(PORTx_TLC5940,PIN_VPRG);
	GPIO_SetPinHigh(PORTx_TLC5940,PIN_BLANK);

    for (i = 0; i < NUM_TLCS * 12; i += 3) {
    	SPI1_send(firstByte);
    	SPI1_send(secondByte);
    	SPI1_send(thirdByte);
    }

    //pulse XLAT
	GPIO_SetPinHigh(PORTx_TLC5940,PIN_XLAT);
	 _delay(1);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_BLANK);
	 _delay(1);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_XLAT);
}

/**
 * The TLC5940 uses 12 bits to determine the output level for the LEDs connected.
 * Each LED is controlled independently and the full array of 12 bits requires a total of 192 bits (12 bits x 16 LEDs)
 *  of data to determine the output levels for the LEDs. The 12 bits for each LED gives a total of 4096
 *  levels for each LED (0 to 4095 inclusive). The TLC5940 uses this data and the current counter to determine the output state of each OUT pin.
 *
 * Here we are sending data trough SPI 8-bit data channel.
 * 1.We take our first 8-bits data and send it.
 * 2.From first data we take last 4-bits and stitch together 4-bits from second data.
 * 3.We then send rest of second 8-bits.
 *
 * A low-high XLAT pulse on this pin latches the data received from the microcontroller into the GS register.
 * When BLANK pin is high the outputs are turned off. Setting the pin low turns the outputs back on.
 * */
void TLC_Update_lvl(uint16_t *data_lvl)
{
	uint8_t n;
	uint8_t tim = 3;

	GPIO_SetPinHigh(PORTx_TLC5940,PIN_BLANK);
	_delay(tim);
	for(n=0; n<(NUM_TLCS *16); n=n+2){
		SPI1_send(data_lvl[n]>>4);
		SPI1_send((data_lvl[n]<<4) | (data_lvl[n+1])>>8);
		SPI1_send(data_lvl[n+1]);
	}

	_delay(tim);
	GPIO_SetPinHigh(PORTx_TLC5940,PIN_XLAT);
	_delay(tim);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_BLANK);
	_delay(tim);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_XLAT);
	_delay(tim);

	 GSCLK_Pulzes();
}

/**
 * Updating all 4 cube layers separately.
 * Starting to turn on MOSFET voltage on LED, so that TLC can control on/off state
 * by pulling to the ground. Then we update on layer, flash all LEDs and turn that layer MOSFET off.
 * */
void TLC_Cube_Update()
{
	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_4);
	TLC_Update_lvl(&data_lvl1[0]);
	GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_4);


	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_3);
	TLC_Update_lvl(&data_lvl2[0]);
	GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_3);


	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_2);
	TLC_Update_lvl(&data_lvl3[0]);
	GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_2);


	GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_1);
	TLC_Update_lvl(&data_lvl4[0]);
	GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_1);
}

void init_TLC_Pins_SPI(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(TLC_RCC_AHA1 | TLC_RCC_AHD1| TLC_RCC_AHB1, ENABLE);

	//GPIO D -> LEVEL PINS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(PORTx_PIN_LVL, &GPIO_InitStruct);

	//GPIO B -> VPRG, BLANK, XLAT, GSCLK
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORTx_TLC5940, &GPIO_InitStruct);

	/** SPI PINS
	* ---------------------------------------------------
	* 						SPI
	* ---------------------------------------------------
	*  configure pins used by SPI1
	*  http://stm32f4-discovery.net/2014/04/library-05-spi-for-stm32f4xx/
	*  GPIOA
	*  MOSI -> GPIO_Pin_7
	*  SCL  -> GPIO_Pin_5
	*/
	//GPIO A -> SCLK, MOSI
	GPIO_InitStruct.GPIO_Pin = PIN_SCK | PIN_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORTx_SPI, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(PORTx_SPI, GPIO_PinSource5, GPIO_AF_SPI1); //SCK
	GPIO_PinAFConfig(PORTx_SPI, GPIO_PinSource7, GPIO_AF_SPI1);	//MOSI

	// enable peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* configure SPI1 in Mode 0
	* CPOL = 0 --> clock is low when idle
	* CPHA = 0 --> data is sampled at the first edge
	*/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 	// set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;					 		// transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 						// one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; 							// clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; 							// data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;	// set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 	// SPI frequency is APB2 frequency / 16
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}

//STAY
/* This funtion is used to transmit and receive data
* with SPI1
* data --> data to be transmitted
* returns received value
*/
uint8_t SPI1_send(uint8_t data){

	SPI_I2S_SendData(SPI1,data);
	//SPI1->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI1->DR; // return received data from SPI data register
}

void _delay(int value)
{
	while(value--);
}

void Blank_Pulse()
{
	GPIO_SetPinHigh(PORTx_TLC5940, PIN_BLANK);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_BLANK);
	GPIO_SetPinHigh(PORTx_TLC5940,PIN_BLANK);;
}

void Xlat_Pulse()
{
	GPIO_SetPinHigh(PORTx_TLC5940, PIN_XLAT);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_XLAT);

}

void GSCLK_Pulse()
{
	GPIO_SetPinHigh(PORTx_TLC5940,PIN_GSCLK);
	GPIO_SetPinLow(PORTx_TLC5940,PIN_GSCLK);
}

