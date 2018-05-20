/*
 * tlc_animations.c
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */


#include "tlc_animations.h"

struct RGB
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

struct HSV
{
	double H;
	double S;
	double V;
};


int count_up(int curr_idx, int count_to)
{
	if(curr_idx == count_to){
		return -2;
	} else {
		curr_idx = curr_idx + 1;
		return curr_idx;
	}
}

int count_down(int curr_idx, int count_to)
{
	int val;
	if(curr_idx == -1) curr_idx = count_to + 1;
	val = (curr_idx - 1);
	if(val == -1){
		return -2;
	} else {
		return val ;
	}
}

const int LedArrayOneLvl[16] = {
	3, 2, 1, 0,
	4, 5, 6, 7,
	11,10, 9, 8,
	12,13,14,15
};

const int LedArrayOneLvl2D[4][4] = {

	{ 3, 2, 1, 0},
	{ 4, 5, 6, 7},
	{11,10, 9, 8},
	{12,13,14,15}
};

/*rumena
 * 	r_ 4030
 *  g_ 714
 *  b_ 4095
 *
 *
 *svetlo rozi
 	r_ 3124
 *  g_
 *
 *  b_ 0
 */


struct RGB HSVToRGB(struct HSV *hsv) {
	double r = 0, g = 0, b = 0;
	double d;
	if (hsv->S == 0)
	{
		r = hsv->V;
		g = hsv->V;
		b = hsv->V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (hsv->H == 360)
			hsv->H = 0;
		else
			hsv->H = hsv->H / 60;
		d = hsv->H;
		d *= 10.;
		int j = d;
		d = (double) j / 10.;

		i = (int)trunc(d);
		f = hsv->H - i;

		p = hsv->V * (1.0 - hsv->S);
		q = hsv->V * (1.0 - (hsv->S * f));
		t = hsv->V * (1.0 - (hsv->S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = hsv->V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = hsv->V;
			b = p;
			break;

		case 2:
			r = p;
			g = hsv->V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = hsv->V;
			break;

		case 4:
			r = t;
			g = p;
			b = hsv->V;
			break;

		default:
			r = hsv->V;
			g = p;
			b = q;
			break;
		}

	}

	struct RGB rgb;
	rgb.R = r * 255;
	rgb.G = g * 255;
	rgb.B = b * 255;

	return rgb;
}



void Pin_on_figurOutColors(int x, int y, int z)
{
	int color_r = TM_ADC_Read(ADC1, ADC_Channel_0);
	int color_g = TM_ADC_Read(ADC1, ADC_Channel_1);
	int color_b = TM_ADC_Read(ADC1, ADC_Channel_2);

	switch(z){
	case 0:
		data_lvl1[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl1[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl1[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 1:
		data_lvl2[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl2[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl2[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 2:
		data_lvl3[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl3[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl3[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 3:
		data_lvl4[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl4[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl4[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;
	};

}

/*
 * @brif: positions 0 -> 3, color -> 0xFF12AB
 *
 * */
void Pin_on(int x, int y, int z, int color)
{

	int i,a;
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;

	switch(z){
	case 0:
		data_lvl1[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl1[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl1[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 1:
		data_lvl2[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl2[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl2[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 2:
		data_lvl3[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl3[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl3[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 3:
		data_lvl4[LedArrayOneLvl2D[x][y] + 0] = color_r;
		data_lvl4[LedArrayOneLvl2D[x][y] + 16] = color_g;
		data_lvl4[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;
	};
}

void Pin_on_crusor(int x, int y, int z, int on_off)
{
	int color_b;
	if(on_off == 1)color_b = 4094;
	if(on_off == 0)color_b = 0;

	switch(z){
	case 0:
		data_lvl1[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 1:
		data_lvl2[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 2:
		data_lvl3[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;

	case 3:
		data_lvl4[LedArrayOneLvl2D[x][y] + 32] = color_b;
	break;
	};
}

void Anim_Quatro_2Squars_Infinity(){

	int delay_time = 200;
	int arr_one[4] = {0,1,4,5};
	int arr_two[4] = {2,3,6,7};
	int arr_seq[20] = {0,8,4,4,8,0,9,-1,10,-2,6,2,2,6,1,7,0,8,0,8};

	int a = 0;
	int i = 0;

	for(i = 0; i < 3; i=i+2){
		for(a = 0; a < 17; a=a+2){
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 1 + i, ROSE, arr_seq[a]);
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2 + i, ROSE, arr_seq[a]);
			Update_array_leds(arr_two, sizeof(arr_two)/sizeof(int), 4 - i, LGREEN, arr_seq[a+1]);
			Update_array_leds(arr_two, sizeof(arr_two)/sizeof(int), 3 - i, LGREEN, arr_seq[a+1]);

			Delayms(delay_time);
			DeleteAllLeds();
		}
		Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2,  ROSE, 0);
		Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3,  ROSE, 0);
		Update_array_leds(arr_two, sizeof(arr_two)/sizeof(int), 2,  LGREEN, 8);
		Update_array_leds(arr_two, sizeof(arr_two)/sizeof(int), 3,  LGREEN, 8);

		Delayms(delay_time);
		DeleteAllLeds();
	}
}

void Anim_Quatro_4Squars_Infinity(){

	int delay_time = 240;
	int arr_one[4] = {0,1,4,5};
	int arr_seq1[10] = {0,4,8, 9,10,6,2,1,0};
	int arr_seq2[10] = {8, 9,10,6,2,1,0,4,8};
	int arr_seq3[10] = {2,1,0,4,8, 9,10,6,2};
	int arr_seq4[10] = {10,6,2,1,0,4,8, 9,10};
	int i = 0;
	int a = 0;

		for(i = 0; i < 3; i=i+2){
			for(a = 0; a < 9; a++){
				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 1+i, ROSE, arr_seq1[a]);
				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2+i, ROSE, arr_seq1[a]);

				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3-i, SVETLOMODRA, arr_seq2[a]);
				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 4-i, SVETLOMODRA, arr_seq2[a]);

				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3-i, YELLOW, arr_seq3[a]);
				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 4-i, YELLOW, arr_seq3[a]);

				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 1+i, LGREEN, arr_seq4[a]);
				Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2+i, LGREEN, arr_seq4[a]);


				Delayms(delay_time);
				DeleteAllLeds();
			}
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2, ROSE, 0);
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3, ROSE, 0);

			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3, SVETLOMODRA, 8);
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2, SVETLOMODRA, 8);

			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3, YELLOW, 2);
			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2, YELLOW, 2);

			Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 3, LGREEN, 10);
	        Update_array_leds(arr_one, sizeof(arr_one)/sizeof(int), 2, LGREEN, 10);

	    	Delayms(delay_time);
	    	DeleteAllLeds();
		}

}

void Anim_TrikotDriveBy(Direction dir, int a, int lvl_sp, int color)
{
	int delay_time = 100;
	int shf;
	int shf_lvl;
	//Comment
	if(a == NAPREJ) shf = 1;
	if(a == NAZAJ)  shf = -1;

	//Nastima shift za lvl
	if(lvl_sp == 4) shf_lvl = -1;
	if(lvl_sp == 1)  shf_lvl = 1;


	Update_enaVrstica(a, 			dir, color, lvl_sp);
	Delayms(delay_time);

	Update_enaVrstica(a, 			dir, color, lvl_sp + shf_lvl);
	Update_enaVrstica(a + shf, 		dir, color, lvl_sp);
	Delayms(delay_time);

	Update_enaVrstica(a, 			dir, color, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a+shf, 		dir, color, lvl_sp + shf_lvl);
	Update_enaVrstica(a+(shf*2), 	dir, color, lvl_sp);
	Delayms(delay_time);

	Update_enaVrstica(a, 			dir, color, lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + shf, 		dir, color, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a + (shf*2), 	dir, color, lvl_sp + shf_lvl);
	Update_enaVrstica(a + (shf*3), 	dir, color, lvl_sp);
	Delayms(delay_time);

	//moving through the midle
	Update_enaVrstica(a , 			dir, 0,     lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + shf, 		dir, color, lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + (2*shf), 	dir, color, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a + (3*shf), 	dir, color, lvl_sp + shf_lvl);
	Delayms(delay_time);

	Update_enaVrstica(a,			dir, 0, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a + shf,		dir, 0, lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + (2*shf),	dir, color, lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + (3*shf), 	dir, color, lvl_sp + (2*shf_lvl));
	Delayms(delay_time);

	Update_enaVrstica(a, 			dir, 0, lvl_sp + shf_lvl);
	Update_enaVrstica(a + shf,	 	dir, 0, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a + (2*shf),  dir, 0, lvl_sp + (3*shf_lvl));
	Update_enaVrstica(a + (3*shf),  dir, color, lvl_sp + (3*shf_lvl));
	Delayms(delay_time);

	//down

	Update_enaVrstica(a, 			dir, 0, lvl_sp);
	Update_enaVrstica(a + shf, 		dir, 0, lvl_sp + shf_lvl);
	Update_enaVrstica(a + (2*shf), 	dir, 0, lvl_sp + (2*shf_lvl));
	Update_enaVrstica(a + (3*shf),	dir, 0, lvl_sp + (3*shf_lvl));
	Delayms(delay_time);


	Update_enaVrstica(a + shf, 		dir, 0, lvl_sp);
	Update_enaVrstica(a + (2*shf),	dir, 0, lvl_sp + shf_lvl);
	Update_enaVrstica(a + (3*shf), 	dir, 0, lvl_sp + (2*shf_lvl));
	Delayms(delay_time);


	Update_enaVrstica(a + (2*shf),	 dir, 0,     lvl_sp);
	Update_enaVrstica(a + (3*shf),	dir, 0,  lvl_sp + shf_lvl);
	Delayms(delay_time);


	Update_enaVrstica(a + (3*shf),	 dir, 0, lvl_sp);
	Delayms(delay_time);
}


void Anim_matrix(){

	//TM_RNG_Init();
	//int randMove;
	//int i;
	//int array_RGB[3];


	struct HSV data = { 154, 0.43, 0.60 };
	struct RGB value =HSVToRGB(&data);

	Update_All_Layers(0, value.R, value.G, value.B);


	//TM_RNG_DeInit();
}

void Update_enaVrstica(int draw_st_vr, Direction dir, int color, int lvl)
{
	int i=0;
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;


	//DeleteAllLeds();
	if(dir  == STOLPEC){
		for(i = 0; i < 4; i++){
			Update_me(LedArrayOneLvl2D[i][draw_st_vr], color_r, color_g, color_b, lvl);
		}
	} else {
		for(i = 0; i < 4; i++){
			Update_me(LedArrayOneLvl2D[draw_st_vr][i], color_r, color_g, color_b, lvl);
		}
	}

}

void Wall(int draw_st_vr, Direction dir, int color)
{
	int i=0;
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;


	DeleteAllLeds();
	if(dir  == VRSTICA){
		for(i = 0; i < 4; i++){
			Update_All_Layers(LedArrayOneLvl2D[i][draw_st_vr], color_r, color_g, color_b);
		}
	} else {
		for(i = 0; i < 4; i++){
			Update_All_Layers(LedArrayOneLvl2D[draw_st_vr][i], color_r, color_g, color_b);
		}
	}
}

void Anime_Wall( count_dir count_func, Direction dir)
{

	int i = -1;

	while(i >= -1){
		i = count_func(i, 3);
		if(i == -2) break;
		Wall(i, dir, LGREEN);
		Delayms(120);

	}
}

void count(int start, int end, int step) {

	int i;
	for(i=start; i<end; i+=step) {
		// execute code here
		//Wall(i, dir, ROZI);

	}
}

void Anim_Loytra(int x, int y, int z, int color)
{
	int i,a;
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;

	for(a = 0; a < 4; a ++){
		for(i = 0; i < 4; i++){
			data_lvl1[LedArrayOneLvl2D[a][i] + 0] = color_r;
			data_lvl1[LedArrayOneLvl2D[a][i] + 16] = color_g;
			data_lvl1[LedArrayOneLvl2D[a][i] + 32] = color_b;

			data_lvl2[LedArrayOneLvl2D[a][i] + 0] = color_r;
			data_lvl2[LedArrayOneLvl2D[a][i] + 16] = color_g;
			data_lvl2[LedArrayOneLvl2D[a][i] + 32] = color_b;

			data_lvl3[LedArrayOneLvl2D[a][i] + 0] = color_r;
			data_lvl3[LedArrayOneLvl2D[a][i] + 16] = color_g;
			data_lvl3[LedArrayOneLvl2D[a][i] + 32] = color_b;

			data_lvl4[LedArrayOneLvl2D[a][i] + 0] = color_r;
			data_lvl4[LedArrayOneLvl2D[a][i] + 16] = color_g;
			data_lvl4[LedArrayOneLvl2D[a][i] + 32] = color_b;
		}
	}

}

void Update_me(int pin,int color_r,int color_g,int color_b, int lvl)
{
	switch(lvl){
		case 1:
			data_lvl1[pin + 0] = color_r;
			data_lvl1[pin + 16] = color_g;
			data_lvl1[pin + 32] = color_b;
		break;

		case 2:
			data_lvl2[pin + 0] = color_r;
			data_lvl2[pin + 16] = color_g;
			data_lvl2[pin + 32] = color_b;
		break;

		case 3:
			data_lvl3[pin + 0] = color_r;
			data_lvl3[pin + 16] = color_g;
			data_lvl3[pin + 32] = color_b;
		break;

		case 4:
			data_lvl4[pin + 0] = color_r;
			data_lvl4[pin + 16] = color_g;
			data_lvl4[pin + 32] = color_b;
		break;
		};
}


void Update_array_leds(int *arr, int length, int layer, int color, int shift)
{
	int i;

	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;

	for(i = 0; i < length; i ++){
		switch(layer){
		case 1:
			data_lvl1[LedArrayOneLvl[arr[i] + shift] + 0] = color_r;
			data_lvl1[LedArrayOneLvl[arr[i] + shift] + 16] = color_g;
			data_lvl1[LedArrayOneLvl[arr[i] + shift] + 32] = color_b;
		break;

		case 2:
			data_lvl2[LedArrayOneLvl[arr[i] + shift] + 0] = color_r;
			data_lvl2[LedArrayOneLvl[arr[i] + shift] + 16] = color_g;
			data_lvl2[LedArrayOneLvl[arr[i] + shift] + 32] = color_b;
		break;

		case 3:
			data_lvl3[LedArrayOneLvl[arr[i] + shift] + 0] = color_r;
			data_lvl3[LedArrayOneLvl[arr[i] + shift] + 16] = color_g;
			data_lvl3[LedArrayOneLvl[arr[i] + shift] + 32] = color_b;
		break;

		case 4:
			data_lvl4[LedArrayOneLvl[arr[i] + shift] + 0] = color_r;
			data_lvl4[LedArrayOneLvl[arr[i] + shift] + 16] = color_g;
			data_lvl4[LedArrayOneLvl[arr[i] + shift] + 32] = color_b;
		break;
		}
	}

}
void Update_All_Layers(int pin, int color_r, int color_g, int color_b){

	data_lvl1[pin + 0] = color_r;
	data_lvl1[pin + 16] = color_g;
	data_lvl1[pin + 32] = color_b;

	data_lvl2[pin + 0] = color_r;
	data_lvl2[pin + 16] = color_g;
	data_lvl2[pin + 32] = color_b;

	data_lvl3[pin + 0] = color_r;
	data_lvl3[pin + 16] = color_g;
	data_lvl3[pin + 32] = color_b;

	data_lvl4[pin + 0] = color_r;
	data_lvl4[pin + 16] = color_g;
	data_lvl4[pin + 32] = color_b;
}

void BasicAnim_Colors()
{

	int n = 0;
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 4095;
		data_lvl2[n] = 4095;
		data_lvl3[n] = 4095;
		data_lvl4[n] = 4095;
	}

	Delayms(3000);

	for(n = 0; n < 48;n++){
		data_lvl1[n] = 0;
		data_lvl2[n] = 0;
		data_lvl3[n] = 0;
		data_lvl4[n] = 0;
	}

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

void DeleteAllLeds()
{
	int n;
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 0;
		data_lvl2[n] = 0;
		data_lvl3[n] = 0;
		data_lvl4[n] = 0;
	}
}
