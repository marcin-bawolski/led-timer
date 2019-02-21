/*
 * encoder.c
 *
 *  Created on: Jul 15, 2018
 *      Author: marcin
 */


#include "encoder.h"
#include "../stm8s.h"
#include "string.h"

volatile uint16_t  irq_count=0;
extern uint8_t button;
static uint16_t *pEncoderCnt= NULL;
//extern uint8_t counter;
//extern volatile uint8_t led_cnt[3];
extern uint8_t update;
//extern uint16_t led_counter;

uint8_t init_encoder(uint16_t *pCounter){
	// DATA and CLK pin are inputs
	ENCODER_PORT->DDR &= ~(ENCODER_CLK_PIN | ENCODER_DATA_PIN);
	//pull-up configuration
	ENCODER_PORT->CR1  |= (ENCODER_CLK_PIN | ENCODER_DATA_PIN);

	// Activate interrupt only on CLK
	ENCODER_PORT->CR2 |= (ENCODER_CLK_PIN );

	//EXTI->CR1 &= ~(EXTI_CR1_PAIS);
	// Port bit 1 interrupt falling edge
	//EXTI->CR1 |= (0x02u<<4);

	pEncoderCnt = pCounter;

	BUTTON_PORT->DDR &= ~(BUTTON_PIN);
	BUTTON_PORT->CR1 |= BUTTON_PIN;

	// interrupt on BUTTON_PIN
	BUTTON_PORT->CR2 |= BUTTON_PIN;

	EXTI->CR1 = 0xAA;
//	EXTI->CR2 = 0xAA;

	return 0;
}



void ENCODER_isr(void) __interrupt(IRQ_EXTI_PORT_A)
{
	if( !(ENCODER_PORT->IDR & ENCODER_CLK_PIN))
	{
		if (ENCODER_PORT->IDR & ENCODER_DATA_PIN)
		{
			//(*pEncoderCnt)++;
			//if((counter) < 255)
			//counter--;
			//irq_count -= 5;
			if ((*pEncoderCnt) >0)
			(*pEncoderCnt) = (*pEncoderCnt)- 1;
		} else {
			//(*pEncoderCnt)--;
			//if((counter) >0)
			//counter++;
			//irq_count += 5;
			if ((*pEncoderCnt) < (999))
				(*pEncoderCnt) = (*pEncoderCnt) + 1;
		}
		update=1;
	}
}



void BUTTON_isr(void) __interrupt(IRQ_EXTI_PORT_D)
{
	if( !(BUTTON_PORT->IDR & BUTTON_PIN)) {

//		if ((*pEncoderCnt) < (999-50)){
//			(*pEncoderCnt) = (*pEncoderCnt) + 50;
//		}
//		update=1;

		button =1;
	}
}

