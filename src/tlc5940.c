#include "tlc5940.h"
#include "tm_stm32f4_delay.h"

uint8_t TLC5940_count;
uint16_t *TLC5940_grayScale;

extern int timer_update;

//replace TIM3 with your desired Timer...
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TLC_here();
}


//STAY
void GSCLK_Pulzes()
{
	//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	int i;
	TLC5940_GPIO->BSRRL = PIN_VPRG;
	TLC5940_GPIO->BSRRH = PIN_VPRG;
	for (i = 0; i < 4096; i++){
		TLC5940_GPIO->BSRRL = PIN_GSCLK;
		TLC5940_GPIO->BSRRH = PIN_GSCLK;
	}
		 Blank_Pulse();
}


//STAY
void Timer_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_Period = 27;  	   			//  27    for 20ms interrupt
    TIM_TimeBase_InitStructure.TIM_Prescaler = 59999 ;			 // 59999
    TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);
}


//STAY
void Tlc5940_setAllDC(uint8_t value)
{
	int i;
    uint8_t firstByte = value << 2 | value >> 4;
    uint8_t secondByte = value << 4 | value >> 2;
    uint8_t thirdByte = value << 6 | value;

	GPIO_SetPinHigh(TLC5940_GPIO,DD_VPRG);
	GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);

    for (i = 0; i < NUM_TLCS * 12; i += 3) {
    	SPI1_send(firstByte);
    	SPI1_send(secondByte);
    	SPI1_send(thirdByte);
    }

    //pulse XLAT
	GPIO_SetPinHigh(TLC5940_GPIO,DD_XLAT);
	 _delay(1);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
	 _delay(1);
	GPIO_SetPinLow(TLC5940_GPIO,DD_XLAT);
}


//STAY
void TLC_Update_lvl(uint16_t *data_lvl)
{
	uint8_t n;
	uint8_t tim = 3;

	 GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);
	 _delay(tim);
	 for(n=0; n<(COUNT_TLC *16); n=n+2){
		 SPI1_send(data_lvl[n]>>4);
		 SPI1_send((data_lvl[n]<<4) | (data_lvl[n+1])>>8);
		 SPI1_send(data_lvl[n+1]);
	 }

	 _delay(tim);
	 GPIO_SetPinHigh(TLC5940_GPIO,PIN_XLAT);
	 _delay(tim);
	 GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
	 _delay(tim);
	 GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);
	 _delay(tim);

	  GSCLK_Pulzes();
	// TIM_Cmd(TIM3, ENABLE);
}

//STAY
void TLC_here()
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

//STAY
void init_TLC_Pins_SPI(void){

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD| TLC_RCC_AHB1, ENABLE);

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
	GPIO_Init(TLC5940_GPIO, &GPIO_InitStruct);

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
	GPIO_InitStruct.GPIO_Pin = DD_SCK | DD_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORTx_SPI, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(PORTx_SPI, GPIO_PinSource5, GPIO_AF_SPI1); //SCK
	GPIO_PinAFConfig(PORTx_SPI, GPIO_PinSource7, GPIO_AF_SPI1);	//MOSI

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

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

//STAY
void _delay(int value)
{
	while(value--);
}

//STAY
void Blank_Pulse()
{
	GPIO_SetPinHigh(TLC5940_GPIO, PIN_BLANK);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
	GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);;
}

void Xlat_Pulse()
{
	GPIO_SetPinHigh(TLC5940_GPIO, PIN_XLAT);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);

}

void GSCLK_Pulse()
{
	GPIO_SetPinHigh(TLC5940_GPIO,PIN_GSCLK);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_GSCLK);
}

