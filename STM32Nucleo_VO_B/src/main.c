/*******************************************************************************
* STM32F401RE NUCLEO board start project
*
* Author: 		Pluzhnik Victor
* Create date: 	07.06.15
*
* Info: Console parser work at UART2 port at speed 115200 (see Init for settings)
*
*******************************************************************************/

#include <stdio.h>
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

void vTaskLED1(void *pvParameters) {
	static int count = 0;
	for (;;) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		vTaskDelay(1000);
		count++;
	}
}

void vTaskLEDs_Algorithm(void *pvParameters) {
	static int count = 0;
	int i;
	uint8_t value;
	for (;;) {
		fflush(stdout);
		SetPins(Light_Red | KHO_Alarm | KHSS_Alarm | Box_Alarm | ScladAV_Alarm | ScladAVPerimetr_Alarm | Sclad_Alarm);
		ResetPins(Light_Green | Dezh_Alarm | KTP_Alarm | Karaul_Alarm | KPP_Alarm | Kazarma_Alarm);
		count++;
		if (xQueueReceive(xQueueLedsAlgorithm, &value, portMAX_DELAY))
		{
			switch (value){
			case 1:		//Dezh_TVS_PIN
			{
				FlashPins(Dezh_Alarm | Karaul_Alarm, 250);
			}
			break;
			case 2:		// KPP_TVS_PIN
			{
				FlashPins(Dezh_Alarm | KPP_Alarm, 250);
			}
			break;
			case 3:		// KTP_TVS_PIN
			{
				FlashPins(Dezh_Alarm | KTP_Alarm, 250);
			}
			break;
			case 4:		// Karaul_TVS_PIN
			{
				FlashPins(Dezh_Alarm | Karaul_Alarm, 250);
			}
			break;
			case 5:		// Kazarma_TVS_PIN
			{
				FlashPins(Dezh_Alarm | Kazarma_Alarm, 250);
			}
			break;
			case 6:		// ScladAV_TVS_PIN
			{
				FlashPins(Dezh_Alarm | Karaul_Alarm, 250);
			}
			break;
			case 7:		// KPP_Move_PIN
			{
				KPPMoveProcess();
			}
			break;
			case 8:		// KHO_Ochr_PIN
			{
				FlashPins(Dezh_Alarm | KHO_Alarm, 250);
			}
			break;
			case 9:		// KHSS_Ochr_PIN
			{
				FlashPins(KHSS_Alarm, 250);
			}
			break;
			case 10:	// Box_Ochr_PIN
			{
				FlashPins(Dezh_Alarm | Box_Alarm | KTP_Alarm, 250);
			}
			break;
			case 11:	// ScladAV_Ochr_PIN
			{
				FlashPins(Dezh_Alarm | ScladAV_Alarm | Karaul_Alarm, 250);
			}
			break;
			case 12:	// ScladAVPerimetr_Ochr_PIN
			{
				FlashPins(Dezh_Alarm | ScladAVPerimetr_Alarm | Karaul_Alarm, 250);
			}
			break;
			case 13:	// Sclad_Ochr_PIN
			{
				FlashPins(Dezh_Alarm | Sclad_Alarm, 250);
			}
			break;
			default: vTaskDelay(500);
			}
		}
		InProcess = 0;
	}
}

void vTaskBeep(void *pvParameters) {
	uint8_t value;
	int i;

	for (;;) {
		if (xQueueReceive(xQueueBeep, &value, portMAX_DELAY))
		{
			if (value == 1)
			{
				for (i=0; i<6; i++)
				{
					SetPins(Zummer);
					vTaskDelay(300);
					ResetPins(Zummer);
					vTaskDelay(400);
				}
			}
			else
			if (value == 2)
			{
				for (i=0; i<6; i++)
				{
					SetPins(Zummer);
					vTaskDelay(150);
					ResetPins(Zummer);
					vTaskDelay(150);
				}
			}
		}
	}
}

void vTaskProcess(void *pvParameters) {
	static int count = 0;
	int i;
	uint8_t Alg;
	unsigned char ch;

	printf("\nSTM32F401RE nucleo board demo. Software version %d.%d.%d!\n",
				ProgramVersion[0], ProgramVersion[1], ProgramVersion[2]);

	vTaskDelay(1000);
	for (;;) {
		if (USART2->SR & UART_FLAG_RXNE)
		{
			ch = USART2->DR;
			Parser(ch);
			fflush(stdout);
		};

		if (!InProcess)
		{
			for (i=0; i<InputPinsCount; i++)
			{
				if (HAL_GPIO_ReadPin(InEvents[i].GPIOx, InEvents[i].Pin) == GPIO_PIN_RESET)
				{
					xQueueSend(xQueueLedsAlgorithm, &InEvents[i].Alg, portMAX_DELAY);
					InProcess = 1;
					printf("Algorithm %d\n", InEvents[i].Alg);
					fflush(stdout);
					break;
				}
			}
		}
		taskYIELD();
	}
}

int
main(int argc, char* argv[])
{
	Init();

	HAL_TIM_Base_Start_IT(&htimer); // Старт таймера

	// At this stage the system clock should have already been configured
	// at high speed.

	xQueueLedsAlgorithm = xQueueCreate( 1, sizeof(uint8_t) );
	xQueueBeep 			= xQueueCreate( 1, sizeof(uint8_t) );

	xTaskCreate(vTaskLED1, (signed char * ) "LED1", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskLEDs_Algorithm, (signed char * ) "LED2", (unsigned short)512,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskBeep, (signed char * ) "BEEP", configMINIMAL_STACK_SIZE,
			NULL, 2, ( xTaskHandle * ) NULL);
	xTaskCreate(vTaskProcess, (signed char * ) "Proc", (unsigned short)512,
			NULL, 2, ( xTaskHandle * ) NULL);

	vTaskStartScheduler();

	// Infinite loop
	while (1) {
	};
}

#pragma GCC diagnostic pop
