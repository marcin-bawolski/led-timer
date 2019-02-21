/*
 * encoder.h
 *
 *  Created on: Jul 15, 2018
 *      Author: marcin
 */

#ifndef MISC_ENCODER_H_
#define MISC_ENCODER_H_

#include "../stm8s.h"
#include "../include/stm8s_irq.h"

#define ENCODER_PORT GPIOA
#define BUTTON_PORT  GPIOD
#define BUTTON_PIN   (1u<<6)

#define ENCODER_CLK_PIN   (1u<<2)
#define ENCODER_DATA_PIN  (1u<<1)

uint8_t init_encoder(uint16_t *pCounter);

void ENCODER_isr(void) __interrupt(IRQ_EXTI_PORT_A);
void BUTTON_isr(void) __interrupt(IRQ_EXTI_PORT_D);

#endif /* MISC_ENCODER_H_ */

