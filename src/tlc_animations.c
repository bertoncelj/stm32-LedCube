/*
 * tlc_animations.c
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */


#include "tlc_animations.h"
//#include "tic_tac_toe.h"

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb_doub;

typedef struct {
    uint8_t r;       // a fraction between 0 and 1
    uint8_t g;       // a fraction between 0 and 1
    uint8_t b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb_doub in);
//static rgb   hsv2rgb(hsv in);
void hsv2rgb(hsv *in, rgb_doub*out);

hsv rgb2hsv(rgb_doub in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


void hsv2rgb(hsv *in, rgb_doub*out)
{
    double hh, p, q, t, ff;
    long  i;


    if(in->s <= 0.0) {       // < is bogus, just shuts up warnings
        out->r = in->v;
        out->g = in->v;
        out->b = in->v;
    }
    hh = in->h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in->v * (1.0 - in->s);
    q = in->v * (1.0 - (in->s * ff));
    t = in->v * (1.0 - (in->s * (1.0 - ff)));

    switch(i) {
    case 0:
        out->r = in->v;
        out->g = t;
        out->b = p;
        break;
    case 1:
        out->r = q;
        out->g = in->v;
        out->b = p;
        break;
    case 2:
        out->r = p;
        out->g = in->v;
        out->b = t;
        break;

    case 3:
        out->r = p;
        out->g = q;
        out->b = in->v;
        break;
    case 4:
        out->r = t;
        out->g = p;
        out->b = in->v;
        break;
    case 5:
    default:
        out->r = in->v;
        out->g = p;
        out->b = q;
        break;
    }
}

int HSV_color_shift(int color_HSV_H)
{
	rgb_doub test_rgb;
	hsv test_hsv;

	test_hsv.h = color_HSV_H;
	test_hsv.s = 1;
	test_hsv.v = 1;
	hsv2rgb(&test_hsv, &test_rgb);
	uint8_t r_num = test_rgb.r*255;
	uint8_t g_num = test_rgb.g*255;
	uint8_t b_num = test_rgb.b*255;

	int color = 0;
	color = r_num << 16;
	color += g_num << 8;
	color += b_num;

	return color;
}

int get_random_color()
{
	int H_deg = TM_RNG_Get()%360;
	if(H_deg > 360) H_deg = H_deg % 360;
	rgb_doub test_rgb;
	hsv test_hsv;

	test_hsv.h = H_deg;
	test_hsv.s = 1;
	test_hsv.v = 1;
	hsv2rgb(&test_hsv, &test_rgb);
	uint8_t r_num = test_rgb.r*255;
	uint8_t g_num = test_rgb.g*255;
	uint8_t b_num = test_rgb.b*255;

	int color = 0;
	color = r_num << 16;
	color += g_num << 8;
	color += b_num;

	return color;
}

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
/*lvl 1-4
 * start_color 0 - 360
 * */
void rainbow_one_lvl(int lvl, int start_color)
{
	int x,y,i;
	int color_r = start_color;
	for(i = 0; i < 1000; i++){
		for(y = 0; y < 4; y++){
			for(x = 0; x < 4; x++){
				 Pin_on(x, y, lvl, HSV_color_shift(color_r));
				 color_r +=1;
				 if(color_r > 360) color_r = 0;
			}
		}
		color_r -=15;
	}

}

void rainbow_all_cube_same()
{
	int x,y,i;
	int color_r = 0;
	for(i = 0; i < 1000; i++){ 	//tle zmansi na 360 za en krog
		for(y = 0; y < 4; y++){
			for(x = 0; x < 4; x++){
				 Pin_on(x, y, 0, HSV_color_shift(color_r));
				 Pin_on(x, y, 1, HSV_color_shift(color_r));
				 Pin_on(x, y, 2, HSV_color_shift(color_r));
				 Pin_on(x, y, 3, HSV_color_shift(color_r));
				 color_r +=1;
				 if(color_r > 360) color_r = 0;
				 Delayms(4);
			}
		}
		color_r -=15;
	}
}

void rainbow_all_cube_diff()
{
	int x,y,z,i;
	int color_r = 0;
	for(i = 0; i < 1000; i++){	//tle zmansi na 360 za en krog
		for(y = 0; y < 4; y++){
			for(x = 0; x < 4; x++){
				for(z = 0; z < 4; z++){
					 Pin_on(x, y, z, HSV_color_shift(color_r));
					 Pin_on(x, y, z, HSV_color_shift(color_r));
					 Pin_on(x, y, z, HSV_color_shift(color_r));
					 Pin_on(x, y, z, HSV_color_shift(color_r));
					 color_r +=1;
					 if(color_r > 360) color_r = 0;
					 Delayms(1);
				}
			}
		}
		color_r -=16*4 - 1;
	}
}

void Anim_snake_rainbow(int lvl){

	int i;
	int r_color = TM_RNG_Get()%360;
	for(i = 0; i < 32; i++){
		Pin_on_one(i,lvl,HSV_color_shift(r_color));
		r_color += 5;
		Delayms(100);
		if(i >= 16){
			Pin_on_one(i-16, lvl, 0);
		}
		if(r_color > 360) r_color == 0;
	}
}

void Update_cube_pin(int pin, int color)
{
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;

	switch(DetermineLayer(pin)){

		case 1:
			data_lvl1[LedArrayOneLvl[pin] + 0] =  color_r;
			data_lvl1[LedArrayOneLvl[pin] + 16] = color_g;
			data_lvl1[LedArrayOneLvl[pin] + 32] = color_b;
			break;

		case 2:
			data_lvl2[LedArrayOneLvl[pin-16] + 0] =  color_r;
			data_lvl2[LedArrayOneLvl[pin-16] + 16] = color_g;
			data_lvl2[LedArrayOneLvl[pin-16] + 32] = color_b;
		break;

		case 3:
			data_lvl3[LedArrayOneLvl[pin-32] + 0] =  color_r;
			data_lvl3[LedArrayOneLvl[pin-32] + 16] = color_g;
			data_lvl3[LedArrayOneLvl[pin-32] + 32] = color_b;
		break;

		case 4:
			data_lvl4[LedArrayOneLvl[pin-48] + 0] =  color_r;
			data_lvl4[LedArrayOneLvl[pin-48] + 16] = color_g;
			data_lvl4[LedArrayOneLvl[pin-48] + 32] = color_b;
		break;
	}
}

/*
 *	0, 1, 2, 3,
 * 	4, 5, 6, 7
 * 	8, 9,10,11
 * 	12,13,14,15
 *
 * 	16,17,18,19
 * 	20,21,22,23
 * 	24,25,26,27
 * 	28,29,30,31
 *
 * 	32,33,34,35
 * 	36,37,38,39
 * 	40,41,42,43
 * 	44,45,46,47
 *
 * 	48,49,50,51
 * 	52,53,54,55
 * 	56,57,58,59
 * 	60,61,62,63
 */

void Anim_infinity_snail_rnd()
{
	uint16_t r_color = TM_RNG_Get() % 360;
	if(r_color > 360) r_color = r_color % 360;
	int lane = (r_color/3) % 4;
	int str = r_color % 2;
	int arr[2][16] ={
		{0,16,32,48,52,56,60,44,28,12,8,4,20,36,40,24},
		{0,1,2,3,7,11,15,14,13,12,8,4,5,6,10,9}
	};
	int i;

	for(i = 0; i < 16; i++){
		if(str == 0)
		Update_cube_pin(arr[str][i] + lane, HSV_color_shift(r_color));
		else Update_cube_pin(arr[str][i]+ lane*16, HSV_color_shift(r_color));
		Delayms(40);
	}
	//ERASE
	/*
	for(i = 0; i < 16; i++){
		if(str == 0)
		Update_cube_pin(arr[str][i] + lane, 0);
		else Update_cube_pin(arr[str][i] + lane*16, 0);
		Delayms(20);
	}
	*/
}

void Anim_zastave(){

	/*
	 *	0, 1, 2, 3,
	 * 	4, 5, 6, 7
	 * 	8, 9,10,11
	 * 	12,13,14,15
	 *
	 * 	16,17,18,19
	 * 	20,21,22,23
	 * 	24,25,26,27
	 * 	28,29,30,31
	 *
	 * 	32,33,34,35
	 * 	36,37,38,39
	 * 	40,41,42,43
	 * 	44,45,46,47
	 *
	 * 	48,49,50,51
	 * 	52,53,54,55
	 * 	56,57,58,59
	 * 	60,61,62,63
	 */
	//japan
	int arr1[16] = {WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE};
	int arr2[16] = {WHITE,WHITE,WHITE,WHITE,WHITE,RED,RED,WHITE,WHITE,RED,RED, WHITE,WHITE,WHITE,WHITE,WHITE};
	int arr3[16] = {WHITE,WHITE,WHITE,WHITE,WHITE,RED,RED,WHITE,WHITE,RED,RED, WHITE,WHITE,WHITE,WHITE,WHITE};
	int arr4[16] = {WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE};

	Update_array_leds_colors(arr1, 16, 1);
	Update_array_leds_colors(arr2, 16, 2);
	Update_array_leds_colors(arr3, 16, 3);
	Update_array_leds_colors(arr4, 16, 4);

	Delayms(2000);
	//Slovenia
	//arr1[16] = {WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE};
	//arr2[16] = {BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	//arr3[16] = {BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	//arr4[16] = {RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED};

	Update_array_leds_colors(arr1, 16, 1);
	Update_array_leds_colors(arr2, 16, 2);
	Update_array_leds_colors(arr3, 16, 3);
	Update_array_leds_colors(arr4, 16, 4);


	//Poland

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
//lvl 1 - 4
int Pin_on_one(int pin, int lvl,  int color)
{
	if(pin < 0 || pin > 15) return 0;
	int color_r = color >> 16;
	int color_g = color >> 8 & 0xFF;
	int color_b = color & 0xFF;

	color_r = (color_r*4095)/255;
	color_g = (color_g*4095)/255;
	color_b = (color_b*4095)/255;

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


void Update_array_leds_colors(int *arr, int length, int layer)
{
	int i;

	for(i = 0; i < length; i ++){
		int color = arr[i];

		int color_r = color >> 16;
		int color_g = color >> 8 & 0xFF;
		int color_b = color & 0xFF;

		color_r = (color_r*4095)/255;
		color_g = (color_g*4095)/255;
		color_b = (color_b*4095)/255;

		switch(layer){
		case 1:
			data_lvl1[LedArrayOneLvl[i] + 0 ]= color_r;
			data_lvl1[LedArrayOneLvl[i] + 16]= color_g;
			data_lvl1[LedArrayOneLvl[i] + 32]= color_b;
		break;

		case 2:
			data_lvl2[LedArrayOneLvl[i] + 0 ]= color_r;
			data_lvl2[LedArrayOneLvl[i] + 16]= color_g;
			data_lvl2[LedArrayOneLvl[i] + 32]= color_b;
		break;

		case 3:
			data_lvl3[LedArrayOneLvl[i] + 0 ]= color_r;
			data_lvl3[LedArrayOneLvl[i] + 16]= color_g;
			data_lvl3[LedArrayOneLvl[i] + 32]= color_b;
		break;

		case 4:
			data_lvl4[LedArrayOneLvl[i] + 0	]  = color_r;
			data_lvl4[LedArrayOneLvl[i] + 16] = color_g;
			data_lvl4[LedArrayOneLvl[i] + 32] = color_b;
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
