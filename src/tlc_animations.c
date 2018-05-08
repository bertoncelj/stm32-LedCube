/*
 * tlc_animations.c
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */


#include "tlc_animations.h"


//TODO
const int LedArray3D[4][4][4] = {
	{
		{0, 1, 2, 3},
		{4, 5, 6, 7},
		{8 , 9,10,11},
		{12,13,14,15}
	},
	{
		{15,16,17,18},
		{11,50,51,52},
		{58,57,56,55},
		{61,62,63,64}
	},
	{
		{83,82,81,80},
		{85,86,87,88},
		{94,93,92,91},
		{97,98,99,100}
	},
	{
		{118,117,116,115},
		{121,122,123,124},
		{130,129,128,127},
		{133,134,135,136}
	},
};


void Pin_on(int x, int y, int z, int color)
{
	switch(z){
	case 0:
		data_lvl1[LedArray3D[x][y][0]] = color;
	break;

	case 1:
		data_lvl2[LedArray3D[x][y][1]] = color;
	break;

	case 2:
		data_lvl3[LedArray3D[x][y][2]] = color;
	break;

	case 3:
		data_lvl4[LedArray3D[x][y][3]] = color;
	break;
	};
}


void BasicAnim_Colors()
{
	TIM_Cmd(TIM2, DISABLE);
	int n = 0;
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 4095;
		data_lvl2[n] = 4095;
		data_lvl3[n] = 4095;
		data_lvl4[n] = 4095;
	}
	TIM_Cmd(TIM2, ENABLE);
	Delayms(3000);
	TIM_Cmd(TIM2, DISABLE);
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 0;
		data_lvl2[n] = 0;
		data_lvl3[n] = 0;
		data_lvl4[n] = 0;
	}
	TIM_Cmd(TIM2, ENABLE);

}
void BasicAnim_RGB_All()
{
	int n;
	for(n = 0; n < 16; n++){
		data_lvl1[n] = 4095;
		data_lvl2[n] = 4095;
		data_lvl3[n] = 4095;
		data_lvl4[n] = 4095;

	}
	Delayms(1000);
	for(n = 0; n < 16; n++){
			data_lvl1[n] = 0;
			data_lvl2[n] = 0;
			data_lvl3[n] = 0;
			data_lvl4[n] = 0;

		}


	for(n = 16; n < 32; n++){
		data_lvl1[n] = 4095;
		data_lvl2[n] = 4095;
		data_lvl3[n] = 4095;
		data_lvl4[n] = 4095;
	}

	Delayms(1000);
	for(n = 16; n < 32; n++){
			data_lvl1[n] = 0;
			data_lvl2[n] = 0;
			data_lvl3[n] = 0;
			data_lvl4[n] = 0;
		}

		for(n = 32; n < 48; n++){
			data_lvl1[n] = 4095;
			data_lvl2[n] = 4095;
			data_lvl3[n] = 4095;
			data_lvl4[n] = 4095;
		}
		Delayms(1000);
		for(n = 32; n < 48; n++){
			data_lvl1[n] = 0;
			data_lvl2[n] = 0;
			data_lvl3[n] = 0;
			data_lvl4[n] = 0;
		}
}
void BasicAnim_One_startToEnd()
{
	int n;
	for(n = 0; n < 48; n++){
		data_lvl1[n] = 4095;
		Delayms(200);
		data_lvl1[n] = 0;
	}
}

void Matrix()
{

	//random

	//Am I too late?
	//Hi, this is urgent!,
	//I am from the future, send back to stop you
	//Soon your software will spawn computer consciousness
	//And it will spread quickly around the world,

}
