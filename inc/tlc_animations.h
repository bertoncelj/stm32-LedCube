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

#define ROZI 0xFA2CFF
#define SVETLOMODRA 0x00BFFF
#define ORANGE 0xFAA460
#define YELLOW 0xFFFF00
#define ROSE 0xFF1493
#define LGREEN 0x98FB98

typedef enum {
	STOLPEC, VRSTICA
} Direction;


typedef int (*count_dir)(int curr_idx, int count_to);

int count_up(int curr_idx, int count_to);
int count_down(int curr_idx, int count_to);



void BasicAnim_RGB_All();
void BasicAnim_Colors();
void BasicAnim_One_startToEnd();
void Pin_on_figurOutColors(int x, int y, int z);

void Wall(int draw_st_vr, Direction dir, int color);



/*Animacije*/
void Anim_Quatro_2Squars_Infinity();
void Anim_Quatro_4Squars_Infinity();
void Anime_Wall( count_dir count_func, Direction dir);
void Anim_Loytra(int x, int y, int z, int color);

/*Leds on/off*/
void Pin_on(int x, int y, int z, int color);
void Update_array_leds(int *arr, int length, int layer, int color, int shift);
void Update_All_Layers(int pin, int color_r, int color_g, int color_b);

void DeleteAllLeds();

#endif /* TLC_ANIMATIONS_H_ */
