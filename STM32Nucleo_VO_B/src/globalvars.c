/*
 * globalvars.c
 *
 *  Created on: 20 мая 2015 г.
 *      Author: pluzhnik
 */

#include "globalvars.h"

UART_HandleTypeDef UartHandle;
TIM_HandleTypeDef htimer;
TIM_HandleTypeDef PWMTtimer;

int InProcess = 0;

xQueueHandle xQueueLedsAlgorithm;
xQueueHandle WaitIn;
xQueueHandle xQueueBeep;

const int DoorsPulses[] = {700, 1700};

const SPinIn InEvents[InputPinsCount] = {{Dezh_TVS_PIN, 1},
				   	   	  {KPP_TVS_PIN, 2},
						  {KTP_TVS_PIN, 3},
						  {Karaul_TVS_PIN, 4},
						  {Kazarma_TVS_PIN, 5},
						  {ScladAV_TVS_PIN, 6},
						  {KPP_Move_PIN, 7},
						  {KHO_Ochr_PIN, 8},
						  {KHSS_Ochr_PIN, 9},
						  {Box_Ochr_PIN, 10},
						  {ScladAV_Ochr_PIN, 11},
						  {ScladAVPerimetr_Ochr_PIN, 12},
						  {Sclad_Ochr_PIN, 13}};

const SPinOut AlarmPins[OutputPinsCount] = {{Dezh_Alarm_PIN},	// 0
				   	   	  {KPP_Alarm_PIN},						// 1
						  {KHO_Alarm_PIN},						// 2
						  {KHSS_Alarm_PIN},						// 3
						  {Kazarma_Alarm_PIN},					// 4
						  {Sclad_Alarm_PIN},					// 5
						  {ScladAVPerimetr_Alarm_PIN},			// 6
						  {ScladAV_Alarm_PIN},					// 7
						  {Karaul_Alarm_PIN},					// 8
						  {KTP_Alarm_PIN},						// 9
						  {Box_Alarm_PIN},						// 10
						  {Light_Red_PIN},						// 11
						  {Light_Green_PIN},					// 12
						  {Zummer_PIN}};						// 13

const unsigned char ProgramVersion[3] = {1, 0, 0};

