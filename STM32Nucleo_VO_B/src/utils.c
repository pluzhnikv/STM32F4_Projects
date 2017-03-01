/*
 * utils.c
 *
 *  Created on: 20 мая 2015 г.
 *      Author: victor
 */

#include "utils.h"

void TM_PWM_Init(int OpenClose) {
	static int PrevValue = 0;
	int cnt1, cnt2;
    TIM_OC_InitTypeDef TIM_OCStruct;

    /* Common settings */
    //HAL_TIM_PWM_Init(&PWMTtimer);

    if (OpenClose)
    	OpenClose = 1;
    if (PrevValue == OpenClose)
        return;

    cnt1 = DoorsPulses[0];
    cnt2 = DoorsPulses[1];
    while (cnt1 < DoorsPulses[1])
    {
		TIM_OCStruct.OCMode = TIM_OCMODE_PWM1;
		TIM_OCStruct.OCIdleState = TIM_OCIDLESTATE_SET;
		TIM_OCStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
		TIM_OCStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		TIM_OCStruct.OCFastMode = TIM_OCFAST_DISABLE;
		TIM_OCStruct.OCNIdleState = TIM_OCNIDLESTATE_SET;

		if (OpenClose)
			TIM_OCStruct.Pulse = cnt1;
		else
			TIM_OCStruct.Pulse = cnt2;

		 HAL_TIM_PWM_ConfigChannel(&PWMTtimer, &TIM_OCStruct, TIM_CHANNEL_2);
		 HAL_TIM_PWM_Start(&PWMTtimer, TIM_CHANNEL_2);

		 if (OpenClose)
		 	TIM_OCStruct.Pulse = cnt2;
		 else
		 	TIM_OCStruct.Pulse = cnt1;
		 HAL_TIM_PWM_ConfigChannel(&PWMTtimer, &TIM_OCStruct, TIM_CHANNEL_1);
		 HAL_TIM_PWM_Start(&PWMTtimer, TIM_CHANNEL_1);
		 cnt1++;
		 cnt2--;
		 vTaskDelay(5);
    }
    PrevValue = OpenClose;
}

void TM_PWM_Set(int pulse) {
	TIM_OC_InitTypeDef TIM_OCStruct;

	/* Common settings */
	//HAL_TIM_PWM_Init(&PWMTtimer);

	TIM_OCStruct.OCMode = TIM_OCMODE_PWM1;
	TIM_OCStruct.OCIdleState = TIM_OCIDLESTATE_SET;

	TIM_OCStruct.Pulse = pulse;				// 1/10 импульса Значение - процент импульса*10 (250 - 25%)
	TIM_OCStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OCStruct.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCStruct.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&PWMTtimer, &TIM_OCStruct, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&PWMTtimer, TIM_CHANNEL_1);
}

void TM_PWM_SetTime(int pulse, int Time) {
	TIM_OC_InitTypeDef TIM_OCStruct;

	/* Common settings */
	//HAL_TIM_PWM_Init(&PWMTtimer);

	TIM_OCStruct.OCMode = TIM_OCMODE_PWM1;
	TIM_OCStruct.OCIdleState = TIM_OCIDLESTATE_SET;

	TIM_OCStruct.Pulse = pulse;				// 1/10 импульса Значение - процент импульса*10 (250 - 25%)
	TIM_OCStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCStruct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	TIM_OCStruct.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCStruct.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&PWMTtimer, &TIM_OCStruct, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&PWMTtimer, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&PWMTtimer, &TIM_OCStruct, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&PWMTtimer, TIM_CHANNEL_2);
	vTaskDelay(Time);
	HAL_TIM_PWM_Stop(&PWMTtimer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&PWMTtimer, TIM_CHANNEL_2);
}

/* ----------------------------------------------------------------------------
 * Initialize hardware and program variables
 ---------------------------------------------------------------------------- */
extern void Init(void)
{
	int i;
	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	USARTx_CLK_ENABLE();
	__TIM1_CLK_ENABLE();
	__TIM3_CLK_ENABLE();
	__TIM2_CLK_ENABLE();

	/* -2- Configure PA05 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// configure output pins
	for (i=0; i<OutputPinsCount; i++)
	{
		GPIO_InitStruct.Pin = AlarmPins[i].Pin;
		HAL_GPIO_Init(AlarmPins[i].GPIOx, &GPIO_InitStruct);
	}

	// configure input pins
	for (i=0; i<InputPinsCount; i++)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Pin = InEvents[i].Pin;
		HAL_GPIO_Init(InEvents[i].GPIOx, &GPIO_InitStruct);
	}

	/**USART2 GPIO Configuration
	 PA2     ------> USART2_TX
	 PA3     ------> USART2_RX
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* PA9, PA8 - PWM output */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	UartHandle.Instance = USARTx;
	UartHandle.Init.BaudRate = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UartHandle);

	PWMTtimer.Init.Prescaler = 84;						// Прескалер - 1 микросекунда (такт таймера)
	PWMTtimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	PWMTtimer.Init.Period = 10000;							// Период - 10 миллисекунд (100 Hz)
	PWMTtimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	PWMTtimer.Instance = TIM1;

	htimer.Init.Prescaler = 84;							// Прескалер - 1 микросекунда (такт таймера)
	htimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer.Init.Period = 1000;							// Период - 1 миллисекунда (прерывание таймера)
	htimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimer.Instance = TIM3;

	HAL_TIM_Base_Init(&PWMTtimer);         				// Инициализация таймера
	HAL_TIM_Base_Init(&htimer);         				// Инициализация таймера
	HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	/* Init PWM */
	//TM_PWM_Init(2000);
}

void FlashPins(uint32_t Pins, int Time)
{
	uint32_t i, k;
	uint32_t TPins;
	uint8_t BeepParam;

	BeepParam = 1;
	xQueueSend(xQueueBeep, &BeepParam, portMAX_DELAY);

	for (i=0; i<FLASH_TIME; i++)
	{
		TPins = Pins;
		for (k=0; k<OutputPinsCount; k++)
		{
			if (TPins & 0x00000001)
			{
				HAL_GPIO_TogglePin(AlarmPins[k].GPIOx, AlarmPins[k].Pin);
			}
			TPins = TPins >> 1;
		}
		vTaskDelay(Time);
	}
}

void SetPins(uint32_t Pins)
{
	int i;

	for (i=0; i<OutputPinsCount; i++)
	{
		if (Pins & 0x00000001)
		{
			HAL_GPIO_WritePin(AlarmPins[i].GPIOx, AlarmPins[i].Pin, GPIO_PIN_SET);
		}
		Pins = Pins >> 1;
	}
}

void ResetPins(uint32_t Pins)
{
	int i;

	for (i=0; i<OutputPinsCount; i++)
	{
		if (Pins & 0x00000001)
		{
			HAL_GPIO_WritePin(AlarmPins[i].GPIOx, AlarmPins[i].Pin, GPIO_PIN_RESET);
		}
		Pins = Pins >> 1;
	}
}

void KPPMoveProcess(void)
{
	int i;
	uint8_t BeepParam;

	BeepParam = 2;
	ResetPins(Light_Red);
	SetPins(Light_Green);
	vTaskDelay(2000);
	xQueueSend(xQueueBeep, &BeepParam, portMAX_DELAY);
	TM_PWM_SetTime(1450, 3200);
	vTaskDelay(10000);
	SetPins(Light_Red);
	ResetPins(Light_Green);
	vTaskDelay(2000);
	xQueueSend(xQueueBeep, &BeepParam, portMAX_DELAY);
	TM_PWM_SetTime(1560, 3650);
}
