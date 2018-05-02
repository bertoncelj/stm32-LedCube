/*
 * tlc_animations.c
 *
 *  Created on: May 1, 2018
 *      Author: tine
 */


#include "tlc_animations.h"
void BasicAnim_Colors()
{
	int n = 0;
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 4095;
		data_lvl2[n] = 4095;
		data_lvl3[n] = 4095;
		data_lvl4[n] = 4095;
	}
	/*Delayms(3000);
	for(n = 0; n < 48;n++){
		data_lvl1[n] = 0;
		data_lvl2[n] = 0;
		data_lvl3[n] = 0;
		data_lvl4[n] = 0;
	}
	*/

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
