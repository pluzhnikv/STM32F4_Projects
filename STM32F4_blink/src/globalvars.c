/*
 * globalvars.c
 *
 *  Created on: 20 мая 2015 г.
 *      Author: pluzhnik
 */

#include "globalvars.h"

UART_HandleTypeDef UartHandle;
TIM_HandleTypeDef htimer;

//int (*pf)(unsigned char);  Указатель на функцию

const unsigned char ProgramVersion[3] = {1, 0, 1};

