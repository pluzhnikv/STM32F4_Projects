/*
 * utils.h
 *
 *  Created on: 20 мая 2015 г.
 *      Author: victor
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "globalvars.h"

extern void Init(void);
extern void TM_PWM_Init(int OpenClose);
extern void TM_PWM_Set(int pulse);
extern void TM_PWM_SetTime(int pulse, int Time);
extern void FlashPins(uint32_t Pins, int Time);
extern void SetPins(uint32_t Pins);
extern void ResetPins(uint32_t Pins);
extern void KPPMoveProcess(void);

#endif /* UTILS_H_ */
