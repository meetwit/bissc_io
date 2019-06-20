#include "bissc.h"
#include "xmc_gpio.h"

uint8_t read[40]={0},bissc_read_lock=1,bissc_state=0;
struct bissc bissc_test={0};

void bissc_init(void){
	
	XMC_GPIO_CONFIG_t clk_config={
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
		.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE,
	};
	XMC_GPIO_Init(bissc_clk,&clk_config);
	XMC_GPIO_SetOutputHigh(bissc_clk);
	
	XMC_GPIO_CONFIG_t miso_config={
		.mode = XMC_GPIO_MODE_INPUT_PULL_DOWN,
	};
	XMC_GPIO_Init(bissc_miso,&miso_config);
	XMC_GPIO_EnableDigitalInput (bissc_miso);
	
}

/*
just 
*/
uint8_t bissc_one_bit(void){
	//XMC_GPIO_SetOutputLow
	//XMC_GPIO_SetOutputHigh
	//XMC_GPIO_ToggleOutput
	static uint8_t state_t = 0,state_bit_t=0,done_flag=0,find_start=0;
	state_t++;
	if(bissc_read_lock){
		done_flag=0;
		find_start=0;
		state_t=0;
		return 0;		//locking
	}
	if(done_flag){
		XMC_GPIO_SetOutputLow(bissc_clk);
		if(XMC_GPIO_GetInput(bissc_miso)){
			XMC_GPIO_SetOutputHigh(bissc_clk);
			bissc_read_lock=1;
			//PcTx_String((unsigned char *) "temp read end\r\n");
			return 3;//frame done
		}else{
			//PcTx_String((unsigned char *) "lock again\r\n");
			return 2;//frame waiting time out
		}
	}
	if(state_t%2){
		//change 
		XMC_GPIO_ToggleOutput(bissc_clk);
	}
	if(state_t%4){
	}else{
		//get data
		read[state_bit_t] = XMC_GPIO_GetInput(bissc_miso);
		if(find_start==0){
			//no low
			if(read[0]==0){
				find_start=1;
				return 5;
			}else{
				return 6;
			}
		}else if(find_start==1){
			//after low find start
			if(read[0]==1){
				state_bit_t++;
				bissc_test.start=read[0];
				find_start=2;
				return 7;
			}else{
				return 8;
			}
		}
		if(state_bit_t>=2&&state_bit_t<=21){
			bissc_test.data = (bissc_test.data)<<1 ;
			bissc_test.data |= XMC_GPIO_GetInput(bissc_miso);
		}
		if(state_bit_t>=22&&state_bit_t<=27){
			bissc_test.zero = (bissc_test.crc)<<1 ;
			bissc_test.zero |= XMC_GPIO_GetInput(bissc_miso);
		}
		if(state_bit_t>=30&&state_bit_t<=35){
			bissc_test.crc = (bissc_test.crc)<<1 ;
			bissc_test.crc |= XMC_GPIO_GetInput(bissc_miso);
		}
		switch (state_bit_t){
				case 1:
								bissc_test.csd = read[1];
								bissc_test.data = 0;
								bissc_test.crc = 0;
								break;
				case 28:
								bissc_test.err = XMC_GPIO_GetInput(bissc_miso);
								break;
				case 29:
								bissc_test.war = XMC_GPIO_GetInput(bissc_miso);
								break;
				}
		state_bit_t++;

		if(state_bit_t==36){
			state_bit_t = 0;
			state_t=0;
			done_flag=1;
		}
	}
	
	return 1;		//frame working...
}
