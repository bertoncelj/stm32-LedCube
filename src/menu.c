/*
 * menu.c
 *
 *  Created on: May 18, 2018
 *      Author: tine
 */

#include "tic_tac_toe.h"
#include "tlc_animations.h"
#include "tm_stm32f4_button.h"

extern	int led_button_choosen;
uint8_t flag_read_ADC;


void menu()
{
	int dummy;

	static int slider_read_value;
	int	slider_read_new;

	char *text_display[2] = {"Animations", "Game"};


	//goes in ones just ot fill old values with 0
	if(flag_read_ADC == 0){
		slider_read_value = 0;
		flag_read_ADC = 1;
	}

	led_button_choosen = 0;
	TM_HD44780_Puts(0,0,"Menu");
	TM_HD44780_Puts(0,1,"Animations");

	while(led_button_choosen == 0){


		dummy = TM_ADC_Read(ADC1, ADC_Channel_4);
		slider_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);

		slider_read_new = Read_ADC_difference(&slider_read_value, &slider_read_new);
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

	if(slider_read_value == 1) chooseGameType(slider_read_value);
	else Anim_TrikotDriveBy(STOLPEC,NAPREJ,1, ROSE);

}


void chooseGameType(int slider_read_value)
{
	led_button_choosen = 0;
	int	slider_read_new;
	int dummy;
	char *text_display[3] = {"PvsP", "PvsC","CvsC"};

	while(led_button_choosen == 0){
		dummy = TM_ADC_Read(ADC1, ADC_Channel_4);
		slider_read_new = TM_ADC_Read(ADC1, ADC_Channel_0);

		slider_read_new = Read_ADC_difference(&slider_read_value, &slider_read_new);
		slider_read_new = slider_read_new/ 1050;

		if(slider_read_value != slider_read_new){
				TM_HD44780_Clear();
				TM_HD44780_Puts(0,0,"Menu");

				slider_read_value = slider_read_new;
				TM_HD44780_Puts(0,1,text_display[slider_read_value]);
		}
		TM_BUTTON_Update();
	}
	switch(slider_read_value){
		case 0:

			RunGame(GetHumanMove, GetHumanMove);

		break;

		case 1:
			//ChooseComputorLvlMode();
			RunGame(GetHumanMove, GetComputerMove());

		break;

		case 2:
			RunGame(GetComputerMove, GetComputerMove);

		break;
	}
}

int ChooseComputorLvlMode()
{
	int hardLvl;



	return hardLvl;
}

