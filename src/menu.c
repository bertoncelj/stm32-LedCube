/*
 * menu.c
 *
 *  Created on: May 18, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"
#include "tlc_animations.h"
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_hd44780.h"

extern	int led_button_choosen;
uint8_t flag_read_ADC;
int depth_lvl_1;
int depth_lvl_2;

void chooseGameType(int slider_read_value);

int ChooseDepthComp(char *message, int slider_read_value)
{
	led_button_choosen = 0;
	int	slider_read_new;
	char *text_display[4] = {"1", "2","3"};
	TM_HD44780_Puts(5,0,"Comp Lvl:");
	TM_HD44780_Puts(5,1,message);
	while(led_button_choosen == 0){
		TM_ADC_Read(ADC1, ADC_Channel_4);
		Delayms(1);
		slider_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);
		Delayms(1);

		slider_read_new = Read_ADC_differenceMenu(&slider_read_value, &slider_read_new);
		slider_read_new = slider_read_new/ 1400;

	if(slider_read_value != slider_read_new){
				slider_read_value = slider_read_new;
				TM_HD44780_Puts(5,1,message);
				TM_HD44780_Puts(8,1,text_display[slider_read_value]);
		}
		Delayms(20);
		TM_BUTTON_Update();
	}
	//0 depth is useless, so here layers are from 1 to 3
	return slider_read_value;

}

int Read_ADC_differenceMenu(int *old, int *new)
{
	if(*new - *old > 220 ||  *old - *new > 220){
		return *new;
	}
	else return *old;
}

void menu()
{
	int slider_read_value=-1;
	int	slider_read_new;

	char *text_display[2] = {"Game", "Animations"};
	led_button_choosen = 0;
	TM_HD44780_Clear();
	TM_HD44780_Puts(0,0,"Menu");

	while(led_button_choosen == 0){
		TM_ADC_Read(ADC1, ADC_Channel_4);
		Delayms(1);
		slider_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);
		Delayms(1);

		slider_read_new = Read_ADC_differenceMenu(&slider_read_value, &slider_read_new);
		slider_read_new = slider_read_new/ 2300;

		if(slider_read_value != slider_read_new){
			TM_HD44780_Clear();
			TM_HD44780_Puts(0,0,"Menu");

			slider_read_value = slider_read_new;
			if(slider_read_value == 1)TM_HD44780_Puts(0, 1, text_display[1]);
			else TM_HD44780_Puts(0,1, text_display[0]);

		}
		TM_BUTTON_Update();
	}
	if(slider_read_value == 0) chooseGameType(slider_read_value);
	else ANIM_LIST_constat();
}

void chooseGameType(int slider_read_value)
{
	led_button_choosen = 0;
	int	slider_read_new;
	char *text_display[4] = {"PvsP", "PvsC ","CvsC","Empty"};

	TM_HD44780_Puts(0,1," vs ");
	while(led_button_choosen == 0){
		TM_ADC_Read(ADC1, ADC_Channel_4);
		Delayms(1);
		slider_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);
		Delayms(1);

		slider_read_new = Read_ADC_differenceMenu(&slider_read_value, &slider_read_new);
		slider_read_new = slider_read_new/ 1400;

		if(slider_read_value != slider_read_new){
				TM_HD44780_Puts(0,0,"Menu");

				slider_read_value = slider_read_new;
				TM_HD44780_Puts(0,1,text_display[slider_read_value]);
		}
		Delayms(20);
		TM_BUTTON_Update();
	}
	switch(slider_read_value){
		case 0:
			RunGame(GetHumanMove, GetHumanMove,1,1);

		break;

		case 1:
			RunGame(GetHumanMove, GetComputerMove,1,ChooseDepthComp("C1:", slider_read_value));

		break;

		case 2:
			RunGame(GetComputerMove, GetComputerMove,ChooseDepthComp("C1:", slider_read_value),ChooseDepthComp("C2:", slider_read_value));
		break;
	}
}


