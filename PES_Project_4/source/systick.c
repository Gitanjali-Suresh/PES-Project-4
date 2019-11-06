/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 4 - Sensor and State Machines                    *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*           Code Leverage : Bruce Montgomery, PES class coding exercises      *
*******************************************************************************/
/*****************************************************************************
 * SysTICK for delay
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdint.h>

/********* Board specific Header Files *********/
#include "MKL25Z4.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

/********* User defined Header Files *********/
#include "systick.h"

int time = 0;
int32_t systick_flag;

/************** Function to initialize SysTick **************/
void Init_Systick(void)
{
	SysTick->LOAD = (48000000L/16);
	NVIC_SetPriority(SysTick_IRQn, 3);	//setting the priority of SysTick interrupt
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/************** SysTick Interrupt Handler **************/
void SysTick_Handler(void)
{
	time++;
	if(time == 15 && systick_flag == 0)	//the delay is generated every 15 seconds
	{
		time = 0;
		systick_flag = 1;
	}
}
