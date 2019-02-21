/**
===============================================================================
 Name        : hal.c
 Author      : $(author)
 Version     : $Revision: 36 $
 Url         : $URL: https://repo1.mydevil.net/svn/priv/bawolski/stm8/trunk/tig_controller/misc/hal.c $
 Copyright   : $(copyright)
 Description : hardware abstraction layer module
===============================================================================
*/

#include "../stm8s.h"
#include "../include/stm8s_irq.h"

#include "string.h"
#include "hal.h" 
#include "led.h"

volatile uint8_t debounce=0;
volatile uint16_t Timeout;
volatile uint16_t *pTimer=NULL;
volatile uint16_t decrementStep=0;

void TIMER1_isr(void) __interrupt(IRQ_TIM1)
{
	static uint16_t second=0;

	if (Timeout > 0){
		Timeout--;
	}
	second++;

	if (second == 1000){
		second =0;

	if ( pTimer != NULL){
		if ((*pTimer) >0){
			(*pTimer) = (*pTimer) - decrementStep;
			}
		}
	}



	TIM1->SR1 &= ~(1<<0);
}


uint8_t  InitClock(void)
{
  CLK->CKDIVR = (3u<<3) |(1u<<1) ; // Fmaster is 16MHz / 8 and CPU clk is Fmaster / 2
  // which results in Fmaster 2MHz and CPU Clk is 1MHz
  // Fmaster 2MHz  routed to Timer 1, TImer 4, UART and SPI
  CLK->PCKENR1 |= CLK_PCKENR1_TIM1 | \
		  	  	  CLK_PCKENR1_SPI  | \
				  CLK_PCKENR1_TIM4 | \
				  CLK_PCKENR1_UART1;

  return 0;
}

void SetupTimer1(uint16_t period ){

  // Assuming FMaster = 2 MHz and prescaler = 2, have 1us clock in timer
  // Timer Auto-reload preload enable, DownCounting, Timer Enabled
  TIM1->CR1 = (1u<<7) | (1u<<0) | (1u<<4);

  TIM1->ARRH =  (uint8_t)(period>>8);
  TIM1->ARRL =  (uint8_t)(period & 0x00FF);


  TIM1->PSCRH = 0x00;
  TIM1->PSCRL = 0x01;

  TIM1->IER = 1ul<<0;     /*!< interrupt enable register*/
}

void attach_to_timer1(uint16_t *pCounter, uint16_t decrement)
{  disableInterrupts();
	if (pCounter != NULL){
		pTimer = pCounter;
		decrementStep = decrement;
	}
	  enableInterrupts();
}



void setup_timer4(uint8_t period )
{
  // Assuming FMaster = 2 MHz and prescaler = 64, have 1us clock in timer
  // Timer Auto-reload preload enable, UpCounting, Timer Enabled
  TIM4->CR1 = (1u<<7) | (1u<<0);
  TIM4->ARR =  period;
  TIM4->PSCR = 0x06;

  TIM4->IER = 1u<<0;     /*!< interrupt enable register*/
}



inline void clear_timeout(void)
{
   disableInterrupts();
   Timeout=0;
   enableInterrupts();
}


inline void wait_for_timeout(void)
{
	while (!(Timeout==0))
	    ;
}

/**
 *
 * @param time
 */
inline void set_timeout(uint16_t time)
{
	Timeout = time;
}
