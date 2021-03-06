/*
 * tlc_animations.h
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */

#ifndef TLC_ANIMATIONS_H_
#define TLC_ANIMATIONS_H_

#include "tlc5940.h"
#include "tm_stm32f4_delay.h"
#include <math.h>
#include <misc.h>


#define RED		0xFF0000
#define GREEN 	0x00FF00
#define BLUE 	0x0000FF

#define WHITE 		0xFFFFFF
#define ROZI 		0xFA2CFF
#define SVETLOMODRA 0x00BFFF
#define ORANGE 		0xFAA460
#define YELLOW 		0xFFFF00
#define ROSE 		0xFF1493
#define LGREEN 		0x98FB98


//Define dir za Trikotnik DriveBy
#define NAPREJ 0
#define NAZAJ 3
#define NAPREJ_P 1
#define NAPREJ_N -1

typedef enum {
	STOLPEC, VRSTICA
} Direction;

typedef int (*count_dir)(int curr_idx, int count_to);

int count_up(int curr_idx, int count_to);
int count_down(int curr_idx, int count_to);

void ANIM_LIST_constat(void);

/*BASIC TEST*/
void BasicAnim_RGB_All();
void BasicAnim_Colors();
void BasicAnim_One_startToEnd();
void Pin_on_figurOutColors(int x, int y, int z);

/*Single Anim*/
void Wall(int draw_st_vr, Direction dir, int color);
void rainbow_one_lvl(int lvl, int start_color);
void Anim_snake_rainbow(int lvl);

/*Animacije*/
	/*Transition*/
void Anim_TrikotDriveBy(Direction dir, int a, int lvl_sp, int color);
void Anime_Wall( count_dir count_func, Direction dir);
void Anim_Loytra(int x, int y, int z, int color);
void Anim_login(void);

	/*Infinity*/
void Anim_Quatro_2Squars_Infinity();
void Anim_Quatro_4Squars_Infinity();
void Anim_zastave(void);
void Anim_infinity_snail_rnd(void);
void rainbow_all_cube_diff(void);
void rainbow_all_cube_same(void);


/*Leds on/off*/
void Pin_on_crusor(int x, int y, int z, int on_off);
void Pin_on(int x, int y, int z, int color);
int  Pin_on_one(int pin,int lvl, int color);
void Update_enaVrstica(int draw_st_vr, Direction dir, int color, int lvl);
void Update_me(int pin,int color_r,int color_g,int color_b, int lvl);
void Update_array_leds(int *arr, int length, int layer, int color, int shift);
void Update_array_leds_colors(int *arr, int length, int layer);
void Update_All_Layers(int pin, int color_r, int color_g, int color_b);
int  HSV_color_shift(int color_HSV_H);

void DeleteAllLeds();

#endif /* TLC_ANIMATIONS_H_ */
