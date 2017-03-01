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
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __USART2_CLK_ENABLE();
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

#define Dezh_TVS_PIN				GPIOC, GPIO_PIN_15
#define KPP_TVS_PIN					GPIOC, GPIO_PIN_2
#define KTP_TVS_PIN					GPIOC, GPIO_PIN_14
#define Karaul_TVS_PIN				GPIOH, GPIO_PIN_0
#define Kazarma_TVS_PIN				GPIOC, GPIO_PIN_3
#define ScladAV_TVS_PIN				GPIOH, GPIO_PIN_1
#define KPP_Move_PIN				GPIOB, GPIO_PIN_10

#define KHO_Ochr_PIN				GPIOA, GPIO_PIN_0
#define KHSS_Ochr_PIN				GPIOA, GPIO_PIN_1
#define Box_Ochr_PIN				GPIOA, GPIO_PIN_4
#define ScladAV_Ochr_PIN			GPIOB, GPIO_PIN_0
#define ScladAVPerimetr_Ochr_PIN	GPIOC, GPIO_PIN_1
#define Sclad_Ochr_PIN				GPIOC, GPIO_PIN_0

#define Dezh_Alarm_PIN				GPIOC, GPIO_PIN_12
#define KPP_Alarm_PIN				GPIOC, GPIO_PIN_10
#define KHO_Alarm_PIN				GPIOC, GPIO_PIN_8
#define KHSS_Alarm_PIN				GPIOC, GPIO_PIN_6
#define Kazarma_Alarm_PIN			GPIOC, GPIO_PIN_5
#define Sclad_Alarm_PIN				GPIOA, GPIO_PIN_12
#define ScladAVPerimetr_Alarm_PIN	GPIOA, GPIO_PIN_11
#define ScladAV_Alarm_PIN			GPIOB, GPIO_PIN_12
#define Karaul_Alarm_PIN			GPIOB, GPIO_PIN_2
#define KTP_Alarm_PIN				GPIOB, GPIO_PIN_1
#define Box_Alarm_PIN				GPIOB, GPIO_PIN_15
#define Light_Red_PIN				GPIOB, GPIO_PIN_5
#define Light_Green_PIN				GPIOB, GPIO_PIN_4
#define Zummer_PIN					GPIOB, GPIO_PIN_14

#define Dezh_Alarm					((uint32_t)0x00000001)
#define KPP_Alarm					((uint32_t)0x00000002)
#define KHO_Alarm					((uint32_t)0x00000004)
#define KHSS_Alarm					((uint32_t)0x00000008)
#define Kazarma_Alarm				((uint32_t)0x00000010)
#define Sclad_Alarm					((uint32_t)0x00000020)
#define ScladAVPerimetr_Alarm		((uint32_t)0x00000040)
#define ScladAV_Alarm				((uint32_t)0x00000080)
#define Karaul_Alarm				((uint32_t)0x00000100)
#define KTP_Alarm					((uint32_t)0x00000200)
#define Box_Alarm					((uint32_t)0x00000400)
#define Light_Red					((uint32_t)0x00000800)
#define Light_Green					((uint32_t)0x00001000)
#define Zummer						((uint32_t)0x00002000)

#define InputPinsCount 13
#define OutputPinsCount 14

#define FLASH_TIME 40

typedef struct {
	GPIO_TypeDef  *GPIOx;
	uint32_t Pin;
	uint8_t  Alg;
}SPinIn;

typedef struct {
	GPIO_TypeDef  *GPIOx;
	uint32_t Pin;
}SPinOut;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern UART_HandleTypeDef UartHandle;
extern TIM_HandleTypeDef htimer;
extern TIM_HandleTypeDef PWMTtimer;

extern const unsigned char ProgramVersion[];

extern xQueueHandle xQueueLedsAlgorithm;
extern xQueueHandle WaitIn;
extern xQueueHandle xQueueBeep;

extern int InProcess;

extern const int DoorsPulses[];

extern const SPinIn InEvents[];
extern const SPinOut AlarmPins[];

#endif /* GLOBALVARS_H_ */
