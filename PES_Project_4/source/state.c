/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 4 - Sensor and State Machines                    *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*           Code leverage: Embedded Systems Fundamentals with Arm Cortex-M    *
*                          based Microcontrollers, Dean, 2017, ARM Education  *
*						   Media                                              *
*******************************************************************************/
/*****************************************************************************
 * STATE MACHINES
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdint.h>

/********* Board specific Header Files *********/
#include "MKL25Z4.h"

/********* User defined Header Files *********/
#include "state.h"
#include "logger.h"
#include "led.h"
#include "tmp102.h"
#include "i2c.h"
#include "systick.h"

/***************** Global Variable declarations *****************/
int32_t temp, temp_avg = 77, temp_alert = 32;
int16_t count = 0, flag = 1, structvar = 0, terminate_flag = 0;
eTempState Next_State;
eTempEvent New_Event;

/************* Table-driven state machine definition ************/
sState_Machine_2 SM[10] = {
                            {Temp_Read,Start_Event,input_Temp},
                            {Temp_Read,Timeout_1,input_Temp},
                            {Temp_Read,Timeout_2,input_Temp},
                            {Temp_Read,Timeout_3,input_Temp},
                            {Temp_Alert,Alert,alert},
                            {Temp_Avg_Wait,Complete,avg_Temp},
                            {Disconnected,Disconnect,error_Temp},
                            {State_Change,Timeout_4,change_State},
                        };

/************** Function to read the current temperature **************/
eTempState input_Temp(void)
{
	temp = i2c_read_bytes(DEVICE_ADDR,REG_ADDR);
	red_off();
	blue_off();
	green_on();
	#ifdef DEBUG_LED
		log_Str(Debug, read_Temp, "LED set to GREEN.");
	#endif
    if(temp <= temp_alert && temp != -1)
    {
    	log_Str(Status, read_Temp, "The current temperature:");
    	log_Int(temp);
    	log_Char('F');
    	interrupt_flag = 0;
    	NVIC_EnableIRQ(PORTA_IRQn);
    	while(interrupt_flag == 0)
    	{
    		red_off();
    		green_off();
    		blue_on();
			#ifdef DEBUG_LED
    			log_Str(Debug, read_Temp, "LED set to BLUE.");
			#endif
    	}
        New_Event = Alert;
        Next_State = Temp_Alert;
        structvar = 4;
    }
    else if(temp > temp_alert && temp != 144)
    {
    	log_Str(Status, read_Temp, "The current temperature:");
    	log_Int(temp);
    	log_Char('F');
        New_Event = Complete;
        Next_State = Temp_Avg_Wait;
        structvar = 5;
    }
    else
    {
    	green_off();
    	blue_off();
    	red_on();
		#ifdef DEBUG_LED
    		log_Str(Debug, read_Temp, "LED set to RED.");
		#endif
    	log_Str(Status, read_Temp, "Unable to read from sensor.");
        New_Event = Disconnect;
        Next_State = Disconnected;
        structvar = 6;
    }
    return Next_State;
}

/************** Function to alert **************/
eTempState alert(void)
{
	red_off();
	green_off();
	blue_on();
	#ifdef DEBUG_LED
		log_Str(Debug, alert_Temp, "LED set to BLUE.");
	#endif
	log_Str(Status, alert_Temp, "Temperature below or equal to");
	log_Int(temp_alert);
	log_Char('F');
    New_Event = Complete;
    Next_State = Temp_Avg_Wait;
    structvar = 5;
    return Next_State;
}

/************** Function to calculate the average temperature **************/
eTempState avg_Temp(void)
{
	red_off();
	blue_off();
	green_on();
	#ifdef DEBUG_LED
		log_Str(Debug, wait_fun, "LED set to GREEN.");
	#endif
	log_Str(Status, wait_fun, "The last average temperature value is");
	log_Int(temp_avg);
	log_Char('F');
    temp_avg = (temp_avg + temp)/2;
	#ifdef DEBUG_LED
		log_Str(Debug, wait_fun, "LED set to GREEN.");
	#endif
    log_Str(Status, wait_fun, "The current average temperature value is");
	log_Int(temp_avg);
	log_Char('F');
	systick_flag = 0;
	Init_Systick();
	while(systick_flag == 0)
	{
		red_on();
	}
	red_off();
    count++;
    if(count == 1)
    {
		#ifdef DEBUG_LED
    		log_Str(Debug, wait_fun, "LED set to GREEN.");
		#endif
    	log_Str(Status, wait_fun, "Timeout 1...");
        New_Event = Timeout_1;
        Next_State = Temp_Read;
        structvar = 1;
    }
    else if(count == 2)
    {
		#ifdef DEBUG_LED
    		log_Str(Debug, wait_fun, "LED set to GREEN.");
		#endif
    	log_Str(Status, wait_fun, "Timeout 2...");
        New_Event = Timeout_2;
        Next_State = Temp_Read;
        structvar = 2;
    }
    else if(count == 3)
    {
		#ifdef DEBUG_LED
    		log_Str(Debug, wait_fun, "LED set to GREEN.");
		#endif
    	log_Str(Status, wait_fun, "Timeout 3...");
        New_Event = Timeout_3;
        Next_State = Temp_Read;
        structvar = 3;
    }
    else
    {
		#ifdef DEBUG_LED
    		log_Str(Debug, wait_fun, "LED set to GREEN.");
		#endif
    	log_Str(Status, wait_fun, "Timeout 4...");
        count = 0;
        New_Event = Timeout_4;
        Next_State = State_Change;
        structvar = 7;
    }
    return Next_State;
}

/************** Function to give error **************/
eTempState error_Temp(void)
{
	green_off();
	blue_off();
	red_on();
	#ifdef DEBUG_LED
		log_Str(Debug, wrong_Temp, "LED set to RED.");
	#endif
	log_Str(Status, wrong_Temp, "The temperature couldn't be read.");
    New_Event = End_Event;
    Next_State = End_State;
    structvar = 0;
    return Next_State;
}

/************** Function to change into another state **************/
eTempState change_State(void)
{
    if(flag == 1)
    {
        flag = 2;
        log_Str(Status, change_fun, "State changed to");
    	log_Int(flag);
    	log_Str(Status, change_fun, "Table driven mode");
    	log_Str(Status, change_fun, "***************************************");
        structvar = 0;
    }
    else
    {
        flag = 1;
        log_Str(Status, change_fun, "State changed to");
    	log_Int(flag);
    	log_Str(Status, change_fun, "State driven mode");
    	log_Str(Status, change_fun, "***************************************");
    }
    New_Event = Start_Event;
    Next_State = Temp_Read;
    return Next_State;
}

/************************** References *******************************
 https://aticleworld.com/state-machine-using-c/

**********************************************************************/
