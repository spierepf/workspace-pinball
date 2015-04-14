#include "u8g.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#include <H.rle>
#include <A.rle>
#include <L.rle>
#include <C.rle>
#include <O.rle>
#include <N.rle>
#include <superman.rle>
#include <batman.rle>
#include <wonder_woman.rle>
#include <green_lantern.rle>
#include <aquaman.rle>
#include <the_flash.rle>
#include <captain_america.rle>
#include <iron_man.rle>
#include <the_hulk.rle>
#include <thor.rle>
#include <black_widow.rle>
#include <hawkeye.rle>

#include <RleDecoder.h>

/*
 * Connection List
 *
 * Display	Arduino
 * GND		GND
 * VCC		5V
 * D0		13
 * D1		11
 * RES		8
 * DC		9
 * CS		10
 */

u8g_t u8g;

const unsigned char* bitmaps[] = {
		H_rle_data,
		A_rle_data,
		L_rle_data,
		C_rle_data,
		O_rle_data,
		N_rle_data,
		superman_rle_data,
		batman_rle_data,
		wonder_woman_rle_data,
		green_lantern_rle_data,
		aquaman_rle_data,
		the_flash_rle_data,
		captain_america_rle_data,
		iron_man_rle_data,
		the_hulk_rle_data,
		thor_rle_data,
		black_widow_rle_data,
		hawkeye_rle_data
};

uint16_t lengths[] = {
		H_rle_length,
		A_rle_length,
		L_rle_length,
		C_rle_length,
		O_rle_length,
		N_rle_length,
		superman_rle_length,
		batman_rle_length,
		wonder_woman_rle_length,
		green_lantern_rle_length,
		aquaman_rle_length,
		the_flash_rle_length,
		captain_america_rle_length,
		iron_man_rle_length,
		the_hulk_rle_length,
		thor_rle_length,
		black_widow_rle_length,
		hawkeye_rle_length
};

uint8_t image_buffer[1024];

void draw(void)
{
	u8g_DrawXBM(&u8g, 0, 0, 128, 64, image_buffer);
}

int main(void)
{
	int i = 0;

	/*
	CS: PORTB, Bit 2 --> PN(1,2)
	A0: PORTB, Bit 1 --> PN(1,1)
	SCK: PORTB, Bit 5 --> PN(1,5)
	MOSI: PORTB, Bit 3 --> PN(1,3)
	*/
	u8g_InitSPI(&u8g, &u8g_dev_ssd1306_128x64_2x_hw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), PN(1, 0));

	for(;;) {
		RleDecoder decoder(image_buffer);
		for(size_t j = 0; j < lengths[i]; j++) {
			decoder.write(pgm_read_byte(bitmaps[i] + j));
		}

		u8g_FirstPage(&u8g);
		do
		{
			draw();
		} while ( u8g_NextPage(&u8g) );
		u8g_Delay(1000);

		i = (i + 1) % 18;
	}
}
