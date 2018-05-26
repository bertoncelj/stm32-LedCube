#include "tlc5940.h"
#include "tm_stm32f4_delay.h"

uint8_t TLC5940_count;
uint16_t *TLC5940_grayScale;

extern int timer_update;

void TLC_Pin_Init(void);

//replace TIM2 with your desired Timer...
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	GPIO_SetPinHigh(GPIOD, GPIO_Pin_6);
	GPIO_SetPinLow(GPIOD, GPIO_Pin_6);
	 TLC_here();
	 GPIO_SetPinHigh(GPIOD, GPIO_Pin_2);
	 GPIO_SetPinLow(GPIOD, GPIO_Pin_2);
	// GSCLK_Pulzes();
}

void TIM2_IRQHandler(void)
{
  int a = 1;
  a++;
}

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

void setTLCChannel(uint8_t channel, uint16_t val)
{
    leds[channel] = (val & 0x0FFF);
}

//Tle tocn nvm zakaj je potrebno timer TIM3 enablat
void Timer_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //init 1 ms timer
    //NOTE: Frequency is 84 Mhz
    TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBase_InitStructure.TIM_Period = 27;  	   //  27    za 20ms
    TIM_TimeBase_InitStructure.TIM_Prescaler = 59999 ; // 59999
    TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);



}

void TLC_AllClock_Init()
{
	/* Clock for GPIOD & GPIOB & GPIOC */
	/**PAZI
	* Tle je ptrebno clocke zamenjat ce hoces da to dela pravilno zarad SPI-ja PWM-ja.
	* */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //Clock on for SPI1
    RCC_AHB1PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    //PWM Timer Clock Init
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

}

void TLC_Pin_Init(void)
{
	//Before running Init in main must me defined TM_stm32f4_delay!
	clkCnt = 0;
	GPIO_InitTypeDef GPIO_InitStruct_GPIOD;
	GPIO_InitTypeDef GPIO_InitStruct_SPI;

	//SPI structure
	SPI_InitTypeDef SPI_InitStruct;


	//Tle clocke das ce nimas klicane funkcije TLC_AllClock_Init


	/** GPIO */
	/* Set pins XLAT, BLANK, VPRG -> GPIOD */
	GPIO_InitStruct_GPIOD.GPIO_Pin = DD_XLAT | DD_BLANK ;
	GPIO_InitStruct_GPIOD.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_GPIOD.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_GPIOD.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct_GPIOD);

    /** SPI PINS
     * ---------------------------------------------------
     * 						SPI
     * ---------------------------------------------------
     *  SPI1
     *  To pomen da mamo na: http://stm32f4-discovery.net/2014/04/library-05-spi-for-stm32f4xx/
     *  GPIOA
     *  MISO -> GPIO_Pin_6
     *  MOSI -> GPIO_Pin_7
     *  SCL  -> GPIO_Pin_5
     * */

    //Vsi pini init ZA GPIO B
     GPIO_InitStruct_SPI.GPIO_Pin = GPIO_Pin_All;
     GPIO_InitStruct_SPI.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStruct_SPI.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStruct_SPI.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStruct_SPI.GPIO_PuPd = GPIO_PuPd_NOPULL;
     GPIO_Init(TLC5940_GPIO, &GPIO_InitStruct_SPI);

     //Vsi pini init ZA GPIO D -> LEVEL PINS
      GPIO_InitStruct_SPI.GPIO_Pin = GPIO_Pin_All;
      GPIO_InitStruct_SPI.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStruct_SPI.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStruct_SPI.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStruct_SPI.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_Init(PORTx_PIN_LVL, &GPIO_InitStruct_SPI);

	/* Set pins SCLK, MOSI->"SIN" -> GPIOA */
     /* configure pins used by SPI1
     * PA5 = SCK
     * PA6 = MISO
     * PA7 = MOSI
     */
    GPIO_InitStruct_SPI.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
    GPIO_InitStruct_SPI.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct_SPI.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct_SPI.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct_SPI.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORTx_SPI, &GPIO_InitStruct_SPI);				//GPIOA


    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    //SPI

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // transmit in master mode, NSS pin has to be always high
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // clock is low when idle
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // data sampled at first edge
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI frequency is APB2 frequency / 4
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
    SPI_Init(SPI1, &SPI_InitStruct);

    //enable SPI
    SPI_Cmd(SPI1, ENABLE);

    //initial setup
     // GPIO_WriteBit(PORTx_TLC5940, DD_BLANK | DD_XLAT, RESET);

    //disable all outputs
   // resetTLC();

}
void PWM_Timer_Init_TLC(void)
{

	/*****************************************************
				Init of the PWM Timer
	******************************************************/
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2EN, ENABLE);
	//Druga ura
	 RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//Define structure names
	GPIO_InitTypeDef GPIO_InitStruct_GSCLK;
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_OCInitTypeDef TIM_OC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	  //YOUR PWM Pin here - PA1 in this case
	GPIO_InitStruct_GSCLK.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct_GSCLK.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct_GSCLK.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_GSCLK.GPIO_PuPd = GPIO_PuPd_DOWN;
	//GSCLK
	GPIO_InitStruct_GSCLK.GPIO_Speed = GPIO_Speed_50MHz;
	 //replace GPIOA with the GPIO of your OC Pin
	 GPIO_Init(GPIOA, &GPIO_InitStruct_GSCLK);
	/* Timer Init TIM2 */


	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


	// Timer starten
	TIM_Cmd(TIM2, ENABLE);


	TIM_TimeBase_InitStructure.TIM_Prescaler = 71;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 3;
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);	//50% duty cycle

    TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OC_InitStructure.TIM_Pulse = 2;	//50% duty cycle
    //replace TIM2 with your desired timer and TIM_OCxInit to match your PWM port. In this case TIM2_CH2 is used (PA1)
    TIM_OC1Init(TIM2, &TIM_OC_InitStructure);


	//replace TIM2 with your desired timer and TIM_OCxInit to match your PWM port. In this case TIM2_CH2 is used (PA0)
	//TIM_OC1Init(TIM2, &TIM_OC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_Init(&NVIC_InitStructure);

	//replace TIM2 with your desired timer...
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	//insert hand timer to init struct


	//To bomo dal vn
	//initTLC(&tlcInit);
}



void resetTLC()
{
	uint8_t i;
	for(i = 0; i < 16; i++){
		leds[i] = 0x0000;
	}
	updateTLC();
}

//write a byte to the chip
void writeTLC(uint8_t data)
{
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
}

void updateTLC()
{
	//wait till GSCLK cycle to finish
	while(clkCnt);
	//disable GSCLK output
	TIM_Cmd(TIM2, DISABLE);
	GPIO_WriteBit(PORTx_TLC5940, DD_BLANK, SET);
	Delayms(1);

	Delayms(1);
	    int8_t i;
	    for (i = 15; i >= 0; i-=2)
	    {
	        uint8_t send1 = 0;
	        uint8_t send = leds[i] >> 4;
	        writeTLC(send);

	        send = (leds[i] & 0x000F);
	        send <<= 4;
	        send1 = (leds[i-1]) >> 8;

	        send |= send1;
	        writeTLC(send);

	        send = leds[i-1];
	        writeTLC(send);
	    }
	    //delay(1);
	    GPIO_WriteBit(PORTx_TLC5940, DD_XLAT, SET);
	    Delayms(1);
	    GPIO_WriteBit(PORTx_TLC5940, DD_BLANK, RESET);
	    GPIO_WriteBit(PORTx_TLC5940, DD_XLAT, RESET);
	    Delayms(1);
	    clkCnt = 0;

	    TIM_Cmd(TIM2, ENABLE);

}

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

void TLC_Update_lvl(uint16_t *data_lvl)
{
	uint8_t n;
	uint8_t tim = 3;
	//u16 data[COUNT_TLC* 16]={0x0000 ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

	//TIM_Cmd(TIM3, DISABLE);


	//GPIO_SetPinLow(TLC5940_GPIO,PIN_VPRG);
	 //Test
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


void TLC_One_Led_On(uint8_t led_num)
{
	uint8_t n;
	u16 data[COUNT_TLC* 16]={0x0000 ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};


	//TIM_Cmd(TIM3, DISABLE);
	//GPIO_SetPinLow(TLC5940_GPIO,PIN_VPRG);

	data[led_num] = 4095;
	 //Test
	 GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);
	 for(n=0; n<(COUNT_TLC *16); n=n+2){
		 SPI1_send(data[n]>>4);
		 SPI1_send((data[n]<<4) | (data[n+1])>>8);
		 SPI1_send(data[n+1]);
	 }

	 _delay(1);
	 GPIO_SetPinHigh(TLC5940_GPIO,PIN_XLAT);
	 _delay(1);
	 GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
	 GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);

	 GSCLK_Pulzes();
	 //TIM_Cmd(TIM3, ENABLE);
}

void TLC_here()
{



			GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_4);
			TLC_Update_lvl(&data_lvl1[0]);
			GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_4);


			GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_3);
			TLC_Update_lvl(&data_lvl2[0]);
			GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_3);


			GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_2);
			TLC_Update_lvl(&data_lvl3[0]);
			GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_2);


			GPIO_SetPinLow(PORTx_PIN_LVL, PIN_LEVEL_1);
			TLC_Update_lvl(&data_lvl4[0]);
			GPIO_SetPinHigh(PORTx_PIN_LVL, PIN_LEVEL_1);


}


void init_tim7(void){
TIM_TimeBaseInitTypeDef TIM_TBInitStruct;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7EN,ENABLE); // Timer saat kaynağını aç
TIM_TBInitStruct.TIM_Prescaler =2000; // ÖnBölücü (PSC) değeri 4199
TIM_TBInitStruct.TIM_Period =40; // Otomatik geri yükleme (ARR) eşiği 1
TIM_TimeBaseInit(TIM7,&TIM_TBInitStruct); // Timeri init et

TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // Timer Update olayında interrupt istesin.
NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);


TIM_Cmd(TIM7, ENABLE);// Timeri başlat
}

void TIM7_IRQHandler(void){

TIM7->SR=0;
GPIOD->ODR ^= GPIO_Pin_12;

timer_update +=1;
}

void init_SPI1(void){

GPIO_InitTypeDef GPIO_InitStruct;
SPI_InitTypeDef SPI_InitStruct;

// enable clock for used IO pins
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD| TLC_RCC_AHB1, ENABLE);

//Vsi pini init ZA GPIO D -> LEVEL PINS
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(PORTx_PIN_LVL, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(TLC5940_GPIO, &GPIO_InitStruct);

/* configure pins used by SPI1
* PA5 = SCK
* PA6 = MISO
* PA7 = MOSI
*/
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA, &GPIO_InitStruct);

// connect SPI1 pins to SPI alternate function
GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);

GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

// enable clock for used IO pins
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

/* Configure the chip select pin
in this case we will use PE7 */
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOE, &GPIO_InitStruct);

GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 high

// enable peripheral clock
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

/* configure SPI1 in Mode 0
* CPOL = 0 --> clock is low when idle
* CPHA = 0 --> data is sampled at the first edge
*/
SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // transmit in master mode, NSS pin has to be always high
SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // clock is low when idle
SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // data sampled at first edge
SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // SPI frequency is APB2 frequency / 4
SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
SPI_Init(SPI1, &SPI_InitStruct);

SPI_Cmd(SPI1, ENABLE); // enable SPI1
}

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

void _delay(int value){
while(value--)
{

}
}

void Blank_Pulse(){

	GPIO_SetPinHigh(TLC5940_GPIO, PIN_BLANK);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_BLANK);
	GPIO_SetPinHigh(TLC5940_GPIO,PIN_BLANK);
//TLC5940_GPIO->ODR |=BLANK_PIN;
//TLC5940_GPIO->ODR &=0x00000000;
}

void Xlat_Pulse(){

	GPIO_SetPinHigh(TLC5940_GPIO, PIN_XLAT);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_XLAT);

//TLC5940_GPIO->ODR |=XLAT_PIN;
//TLC5940_GPIO->ODR &=0x00000000;

}

void GSCLK_Pulse(){
	GPIO_SetPinHigh(TLC5940_GPIO,PIN_GSCLK);
	GPIO_SetPinLow(TLC5940_GPIO,PIN_GSCLK);
//TLC5940_GPIO->ODR |=GSCLK_PIN;
//TLC5940_GPIO->ODR &=0x00000000;

}

//CHECK CORE CLK
void MCO2_INIT(){
GPIO_InitTypeDef GPIO_InitStructure;

// Output HSE clock (8 MHz) on MCO pin (PC9)
RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_5);

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

GPIO_StructInit(&GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);
GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_MCO);

}


void tim_gsclk_init(void) {

	// TIM1 clock enable
	RCC_APB2PeriphClockCmd(SIG_GSCLK_TIMER_CLK, ENABLE);

	// GPIO clock enable
	RCC_AHB1PeriphClockCmd(SIG_GSCLK_PORT_CLK, ENABLE);

	// GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(SIG_GSCLK_PORT, &GPIO_InitStructure);

	// Connect PIN8 of PORT A to alternate function (TIM1)
	GPIO_PinAFConfig(SIG_GSCLK_PORT, SIG_GSCLK_PINSOURCE, SIG_GSCLK_PIN_AF);


	// Configure the timer
	// Generate GSCLK signals

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0020;
	TIM_TimeBaseStructure.TIM_Period = 0x0003;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(SIG_GSCLK_TIMER, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channel1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0x0002;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(SIG_GSCLK_TIMER, &TIM_OCInitStructure);

	// Be a slave of SIG_BLANK_TIMER
	TIM_SelectInputTrigger(SIG_GSCLK_TIMER, TIM_TS_ITR2);
	TIM_SelectSlaveMode(SIG_GSCLK_TIMER, TIM_SlaveMode_Gated);
	TIM_SelectMasterSlaveMode(SIG_GSCLK_TIMER, TIM_MasterSlaveMode_Enable);

	// enable the counter
	TIM_Cmd(SIG_GSCLK_TIMER, ENABLE);

	// Main Output Enable
	TIM_CtrlPWMOutputs(SIG_GSCLK_TIMER, ENABLE);
}
