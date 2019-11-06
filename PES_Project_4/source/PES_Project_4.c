/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 4 - Sensor and State Machines                     *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*                                                                             *
*******************************************************************************/

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/********* Board specific Header Files *********/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/********* User defined Header Files *********/
#include "logger.h"
#include "led.h"
#include "unit_test.h"
#include "i2c.h"
#include "tmp102.h"
#include "systick.h"
#include "state.h"

/***************** Global Variable Declarations ******************/
volatile bool interrupt;
int16_t interrupt_flag = 0;

/************** Delay function definition for handling ACK waits ***************/
void delay(int32_t n)
{
	int32_t i = n;
	while(i)
	{
		i--;
	}
	return;
}

/***************** Function to configure PORT A as interrupt pin **************/
void Init_PORTA(void)
{
	PORTA->PCR[12] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[12] |= PORT_PCR_MUX(1);
	PORTA->PCR[12] |= PORT_PCR_PE_MASK;
	PORTA->PCR[12] |= PORT_PCR_IRQC(12);
	PORTA->PCR[12] &= ~PORT_PCR_PS_MASK;
	PTA->PDDR &= ~GPIO_PDDR_PDD_MASK;
	PTA->PDDR &= ~(1UL << 12);
	PTA->PDDR &= ~GPIO_PDDR_PDD(0x04);
}

/************** PORT A Interrupt Handler *****************/
void PORTA_IRQHandler(void)
{
	DisableIRQ(PORTA_IRQn);
	interrupt = PTA->PDIR & GPIO_PDIR_PDI(0x00001000);
	if(interrupt == 1)
	{
		interrupt_flag = 1;
		#ifndef TEST
			red_off();
			green_off();
			blue_on();
		#endif
		#ifdef DEBUG_LED
			log_Str(Debug, read_Temp, "LED set to BLUE.");
		#endif
	}
}

/**************** Main Function *****************/
int main(void)
{
	/************** Board Initializations *************/
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    led_init();				//LED Initialization
    Init_PORTA();			//Port A Initialization

#ifndef TEST				//Executions for only Debug and Normal modes

    Next_State = Temp_Read;
    i2c_init();
    int32_t post_test;
    uint8_t regvalue[2] = {0x60,0xA0};		//Value to be written in configuration register

    /******************** POST *************************/
    post_test = i2c_read_byte(DEVICE_ADDR,CONFIG_ADDR);
    log_Str(Status, main_fun, "Power On Self Test");
    log_Str(Status, main_fun, "Configuration register value - ");
    log_Int(post_test);
    if(post_test == 96 || post_test == 98)
    {
    	log_Str(Status, main_fun, "POST success!");
    	log_Str(Status, main_fun, "***************************************");
    	if(post_test != 0x60)
    	{
    		log_Str(Status, main_fun, "Rewriting Configuration Register Value");
        	i2c_write_bytes(DEVICE_ADDR,CONFIG_ADDR,regvalue,2);
            post_test = i2c_read_byte(DEVICE_ADDR,CONFIG_ADDR);
            log_Str(Status, main_fun, "Configuration register value is");
            log_Int(post_test);
    	}

        post_test = i2c_read_byte(DEVICE_ADDR,TLO_ADDR);
	#ifdef DEBUG_LED
        log_Str(Debug, main_fun, "TLow register value before write is");
        log_Int(post_test);
	#endif

        post_test = i2c_read_byte(DEVICE_ADDR,THI_ADDR);
	#ifdef DEBUG_LED
        log_Str(Debug, main_fun, "THigh register value before write is");
        log_Int(post_test);
	#endif

        delay(10000);
        Init_Alert_TLO(DEVICE_ADDR,TLO_ADDR);
        delay(10000);

        post_test = i2c_read_byte(DEVICE_ADDR,TLO_ADDR);
	#ifdef DEBUG_LED
        log_Str(Debug, main_fun, "TLow register value after write is");
        log_Int(post_test);
	#endif

        delay(10000);
        Init_Alert_THI(DEVICE_ADDR,THI_ADDR);
        delay(10000);

        post_test = i2c_read_byte(DEVICE_ADDR,THI_ADDR);
	#ifdef DEBUG_LED
        log_Str(Debug, main_fun, "THigh register value after write is");
        log_Int(post_test);
	#endif

    	log_Str(Status, main_fun, "State driven mode");
    	log_Str(Status, main_fun, "***************************************");
        while(1)
        {
            switch(Next_State)
            {
                case Temp_Read: log_Str(Status, main_fun, "Temperature Read State");
                                if(flag == 1)
                                {
                                    Next_State = input_Temp();
                                }
                                else
                                {
                                    Next_State = (*SM[structvar].handler)();
                                }
                                break;

                case Temp_Alert: log_Str(Status, main_fun, "Temperature Alert State");
                                 if(flag == 1)
                                 {
                                     Next_State = alert();
                                 }
                                 else
                                 {
                                    Next_State = (*SM[structvar].handler)();
                                 }
                                 break;

                case Temp_Avg_Wait: log_Str(Status, main_fun, "Temperature Average/Wait State");
                                    if(flag == 1)
                                    {
                                        Next_State = avg_Temp();
                                    }
                                    else
                                    {
                                        Next_State = (*SM[structvar].handler)();
                                    }
                                    break;

                case Disconnected: log_Str(Status, main_fun, "Disconnected State");
                                   if(flag == 1)
                                   {
                                      Next_State = error_Temp();
                                   }
                                   else
                                   {
                                        Next_State = (*SM[structvar].handler)();
                                   }
                                   break;

                case State_Change: log_Str(Status, main_fun, "State Change");
                                   if(flag == 1)
                                   {
                                       Next_State = change_State();
                                   }
                                   else
                                   {
                                        Next_State = (*SM[structvar].handler)();
                                   }
                                   break;

                case End_State: log_Str(Status, main_fun, "The program has terminated.");
                				terminate_flag = 1;
                				break;

                default: log_Str(Status, main_fun, "Default case");
                		 break;
            }

            if(terminate_flag)
            	break;
        }
    }
    else
    {
    	log_Str(Status, main_fun, "POST failed!");
    }

#endif

		#ifdef TEST
        	test();
		#endif

        return 0;
}

