
/**
===============================================================================
 Name        : main.c
 Author      : $Author: marcin $
 Description : main definition
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

extern volatile uint8_t led_cnt[3];
extern volatile uint16_t  irq_count;

extern uint8_t button=0;

void set_number(uint8_t led[3], uint16_t value);
uint16_t terminate=0;

uint16_t led_counter=0;

uint8_t counter=0;
uint8_t update=0;

void main(void)
{


  InitClock();
  disableInterrupts();

  led_init_lcd();
  SetupTimer1(1000);

  init_encoder(&irq_count);
  terminate = 10000;

  set_number(led_cnt, 0);
  enableInterrupts();


  while (1) {

		irq_count = 0;
		led_counter = irq_count;

		set_number(led_cnt, led_counter);
		do {

			while (!update)
				;

			disableInterrupts();
			update = 0;
			led_counter = irq_count;
			enableInterrupts();
			set_number(led_cnt, led_counter);
		} while (button != 1);

		if (button == 1) {
			disableInterrupts();
			button = 0;
			enableInterrupts();
			terminate = led_counter;
			attach_to_timer1(&terminate, 1);

			while (terminate > 0) {
				set_number(led_cnt, terminate);
			}
			set_number(led_cnt, terminate);

			while (button != 1)
				;
			button = 0;
		}
	}
}

void set_number(uint8_t led[3], uint16_t value) {
	uint16_t hundredths, tenths, decimals, tmp_counter;

	if (value > 999){
		value = 999;
	}

	hundredths = value / 100;

	tmp_counter = value - (100 * hundredths);
	tenths = (tmp_counter) / 10;
	decimals = tmp_counter - (10 * tenths);

	led[2] = hundredths;
	led[1] = tenths;
	led[0] = decimals;
}


