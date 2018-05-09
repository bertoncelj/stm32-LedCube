/*
 * tlc_animations.c
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */


#include "tlc_animations.h"


//TODO
const int LedArray3D[4][4] = {

	{ 0, 1, 2, 3},
	{ 4, 5, 6, 7},
	{ 8, 9,10,11},
	{12,13,14,15}
};


void Pin_on(int x, int y, int z, int color)
{



	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;
	switch(z){
	case 0:
		data_lvl1[LedArray3D[x][y] + 0] = color_r;
		data_lvl1[LedArray3D[x][y] + 16] = color_g;
		data_lvl1[LedArray3D[x][y] + 32] = color_b;
	break;

	case 1:
		data_lvl2[LedArray3D[x][y] + 0] = color_r;
		data_lvl2[LedArray3D[x][y] + 16] = color_g;
		data_lvl2[LedArray3D[x][y] + 32] = color_b;
	break;

	case 2:
		data_lvl3[LedArray3D[x][y] + 0] = color_r;
		data_lvl3[LedArray3D[x][y] + 16] = color_g;
		data_lvl3[LedArray3D[x][y] + 32] = color_b;
	break;

	case 3:
		data_lvl4[LedArray3D[x][y] + 0] = color_r;
		data_lvl4[LedArray3D[x][y] + 16] = color_g;
		data_lvl4[LedArray3D[x][y] + 32] = color_b;
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
