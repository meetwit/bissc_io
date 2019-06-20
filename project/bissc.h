#ifndef __BISSc_H
#define __USART1_H	 

#include "xmc_gpio.h"

#define bissc_clk P1_1
#define bissc_miso P4_7

//#define bissc_clk P0_8
//#define bissc_miso P0_5
//

struct bissc{
	//uint8_t head;		//  3bit
	uint8_t start;			//  1bit
	uint8_t csd;				//  1bit
	uint32_t data;			// 20bit
	uint8_t zero;				//  6bit
	uint8_t err;				//  1bit
	uint8_t war;				//  1bit
	uint8_t crc;				//  6bit
	//uint8_t foot;		//  1bit
  };

extern uint8_t read[40],bissc_read_lock,bissc_state;
void bissc_init(void);
uint8_t bissc_one_bit(void);

#endif

















