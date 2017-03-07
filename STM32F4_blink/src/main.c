/*******************************************************************************
* STM32F401RE NUCLEO board start project
*
* Author: 		Pluzhnik Victor
* Create date: 	20.02.17
*
* Info: Console parser work at UART2 port at speed 115200 (see Init for settings)
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "globalvars.h"
#include "utils.h"
#include "parser.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void TIM3_IRQHandler ( void ) {
	TIM3->SR = 0x0; // reset the status register
}

void vTaskRead(void *pvParameters) {
	uint8_t value;

	for (;;) {
		if (USART2->SR & USART_FLAG_RXNE) {

			value = USART2->DR;
			Parser((unsigned char)value);
			fflush(stdout);
		}
		taskYIELD();
	}
}

void vTaskLED1(void *pvParameters) {
	for (;;) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
		vTaskDelay(500);
	}
}

void vTaskTact(void *pvParameters) {
	static int count = 0;
	portTickType xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		count++;
		vTaskDelayUntil(&xLastWakeTime,  1000 / portTICK_RATE_MS);
	}
}

void vTaskProcess(void *pvParameters) {
	printf("\nSTM32F401RE nucleo board demo. Software version %d.%d.%d!\n",
				ProgramVersion[0], ProgramVersion[1], ProgramVersion[2]);

	vTaskDelay(1000);
	for (;;) {
		taskYIELD();
	}
}

int main(int argc, char* argv[])
{
	Init();

	HAL_TIM_Base_Start_IT(&htimer); 		// Старт таймера

	// At this stage the system clock should have already been configured
	// at high speed.

	xTaskCreate(vTaskLED1, "LED1", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskProcess, "Proc", (unsigned short)512,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskRead, "Read", (unsigned short)512,
					NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskTact, "Tact", (unsigned short)256,
						NULL, 2, ( xTaskHandle * ) NULL);

	vTaskStartScheduler();

	// Infinite loop
	while (1) {
	};
}

#pragma GCC diagnostic pop
