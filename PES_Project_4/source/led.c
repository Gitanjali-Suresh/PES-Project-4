/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 4 - Memory Functions and Tests                    *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*                                                                             *
*******************************************************************************/
/*****************************************************************************
 * LED
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdio.h>

/********* User defined Header Files *********/
#include "led.h"
#include "board.h"

/********* LED initialization function definition *********/
void led_init(void)
{
	LED_RED_INIT(1);
	LED_GREEN_INIT(1);
	LED_BLUE_INIT(1);
}
/********* Function to turn GREEN LED on *********/
void green_on(void)
{
	LED_GREEN_ON();
}
/********* Function to turn GREEN LED off *********/
void green_off(void)
{
	LED_GREEN_OFF();
}
/********* Function to turn RED LED on *********/
void red_on(void)
{
	LED_RED_ON();
}
/********* Function to turn RED LED off *********/
void red_off(void)
{
	LED_RED_OFF();
}
/********* Function to turn BLUE LED on *********/
void blue_on(void)
{
	LED_BLUE_ON();
}
/********* Function to turn BLUE LED off *********/
void blue_off(void)
{
	LED_BLUE_OFF();
}

