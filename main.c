
/**
===============================================================================
 Name        : main.c
 Author      : $Author: marcin $
 ===============================================================================
*/

// Marcin: interrupt function must be at least declared  in the same file as main() function
// Definition (code itself) can be  the in other location
// void TIMER1_isr(void) __interrupt(IRQ_TIM1);
// void TIMER4_isr(void) __interrupt(IRQ_TIM4);
// above functions are in corresponding headers and MUST be included in main.c

#include "misc/hal.h"
#include "misc/led.h"
#include "include/stm8s_irq.h"
#include "misc/encoder.h"
#include "string.h"

uint8_t *pLed_cnt;
uint8_t * get_display(void);

extern volatile uint16_t  irq_count;

extern uint8_t button=0;

void set_number(uint8_t led[3], uint16_t value);
void set_blank_number(uint8_t led[3]);

uint16_t on_time=0;
uint16_t blank_time=550;

uint16_t led_counter=0;
uint8_t counter=0;
uint8_t update=0;
uint8_t lcd_state=0;
volatile uint8_t blink=0;
const uint8_t time_resolution = 10;

void main(void)
{
  pLed_cnt = get_display();

  InitClock();
  disableInterrupts();

  led_init_lcd();
  SetupTimer1(1000);


  GPIOD->DDR |= (uint8_t)((1u<<5));
  GPIOD->CR1 |= (uint8_t)((1u<<5));
  GPIOD->ODR &= (uint8_t)(~(1u<<5));

  GPIOA->DDR |= (uint8_t)((1u<<3));
  GPIOA->CR1 |= (uint8_t)((1u<<3));
  GPIOA->ODR |= (uint8_t)((1u<<3));

  init_encoder(&irq_count, time_resolution);
  on_time = 1;

  set_number(pLed_cnt, 0);
  enableInterrupts();
    irq_count = 0;
	while (1) 
	{

		led_counter = irq_count;
		init_encoder(&irq_count, time_resolution);

		set_number(pLed_cnt, led_counter);
		GPIOA->ODR |= (uint8_t)((1u<<3));
		do 
		{
			do {
				wfi();	
				if(button)
				{
					break;
				}			
			}while (!update);
			
			//disableInterrupts();
			update = 0;
			led_counter = irq_count;
		//	enableInterrupts();
			set_number(pLed_cnt, led_counter);

		} while (!button);

		init_encoder(NULL, 1);

		//GPIOA->ODR |= (uint8_t)((1u<<3));
		if (button == 1) 
		{
		//	disableInterrupts();
			button = 0;
		//	enableInterrupts();
			on_time = led_counter;
			attach_to_timer1(&on_time, time_resolution);
		    GPIOA->ODR &= (uint8_t)(~(1u<<3));
			do
			{
				set_number(pLed_cnt, on_time);
				wfi();
				button=0;
				// wait untill counter decremented to 0 and terminate flag set
			}while (on_time > 0);
			GPIOA->ODR |= (uint8_t)((1u<<3));


			set_number(pLed_cnt, on_time);
			button=0;
		//	disableInterrupts();
			button = 0;
	//		enableInterrupts();
		}
		set_blank_number(pLed_cnt);
		blank_time = 750 ;
	    attach_to_timer1(&blank_time, 50);
        do{
            wfi();
        }while (blank_time >0);
        button=0;
	}
}


void set_blank_number(uint8_t led[3])
{
	/*led[0] =LED_ALPHABET_CNT-2;
	led[1] =LED_ALPHABET_CNT-2;
	led[2] =LED_ALPHABET_CNT-2;
	*/
	led[0] =12;
	led[1] =14;
	led[2] =12;
}

void set_number(uint8_t led[3], uint16_t value) 
{
	uint16_t hundrets, tenths, singles, tmp_counter;
	if (value > 999){
		value = 999;
	}

	hundrets = value / 100;

	tmp_counter = value - (100 * hundrets);
	tenths = (tmp_counter) / 10;
	singles = tmp_counter - (10 * tenths);

	led[2] = hundrets;
	led[1] = tenths;
	led[0] = singles;
}



