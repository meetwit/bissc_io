/*
 * main.c
 *
 *  Created on: 2019 Jun 20 14:07:10
 *  Author: E
 */




#include <DAVE.h>
#include "bissc.h"

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

#define TICKS_PER_SECOND 1000000

uint8_t read_f=0;
uint64_t	read_d=0;

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }
  SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);
  bissc_init();

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	  if(read_f){
	      		read_f=0;
	      		read_d=0;
	      		for(uint8_t i=0;i<40;i++){
	      			read_d |=	(read[i]<<(40-i));
	      		}
	      	}
  }
}


void SysTick_Handler (void)
{
	bissc_state = bissc_one_bit();
}

