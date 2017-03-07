/*
 * globalvars.h
 *
 *  Created on: 20 мая 2015 г.
 *      Author: pluzhnik
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USART		                     USART2
#define USART_CLK_ENABLE()         		 __USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern UART_HandleTypeDef UartHandle;
extern TIM_HandleTypeDef htimer;

extern const unsigned char ProgramVersion[];

#endif /* GLOBALVARS_H_ */
