/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 4 - Sensor and State Machines                    *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*                                                                             *
*******************************************************************************/
/***********************************************************************************
 * UNIT TESTING [1]
 ***********************************************************************************/

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "MKL25Z4.h"

/********* User defined Header Files *********/
#include "logger.h"
#include "uCUnit.h"
#include "i2c.h"
#include "tmp102.h"
#include "systick.h"
#include "state.h"

uint8_t regvalue[2] = {0x60,0xA0};

void delay_test(int32_t n)
{
	int32_t i = n;
	while(i)
	{
		i--;
	}
	return;
}

/********* Unit Test function definition *********/
void test(void)
{
	int8_t i = 10, post_flag = 0;
	Next_State = Temp_Read;
	i2c_init();
	int32_t post_test;
	UCUNIT_Init(); /* initialize framework */
    post_test = i2c_read_byte(DEVICE_ADDR,CONFIG_ADDR);
    log_Str(Test, main_fun, "Power On Self Test");
    log_Str(Test, main_fun, "Configuration register value - ");
    log_Int(post_test);
    if(post_test == 96 || post_test == 98)
    {
    	post_flag = 1;
      	UCUNIT_TestcaseBegin("Power On Self Test");
      	UCUNIT_CheckIsEqual(post_flag,1);
      	UCUNIT_TestcaseEnd();
    	log_Str(Test, main_fun, "POST success!");
    	log_Str(Test, main_fun, "***************************************");
    	if(post_test != 0x60)
    	{
    		log_Str(Test, main_fun, "Rewriting Configuration Register Value");
        	i2c_write_bytes(DEVICE_ADDR,CONFIG_ADDR,regvalue,2);
            post_test = i2c_read_byte(DEVICE_ADDR,CONFIG_ADDR);
            log_Str(Test, main_fun, "Configuration register value is");
            log_Int(post_test);
    	}

        post_test = i2c_read_byte(DEVICE_ADDR,TLO_ADDR);
        log_Str(Test, main_fun, "TLow register value before write is");
        log_Int(post_test);

        post_test = i2c_read_byte(DEVICE_ADDR,THI_ADDR);
        log_Str(Test, main_fun, "THigh register value before write is");
        log_Int(post_test);

        delay_test(10000);
        Init_Alert_TLO(DEVICE_ADDR,TLO_ADDR);
        delay_test(10000);

        post_test = i2c_read_byte(DEVICE_ADDR,TLO_ADDR);
        log_Str(Test, main_fun, "TLow register value after write is");
        log_Int(post_test);

        delay_test(10000);
        Init_Alert_THI(DEVICE_ADDR,THI_ADDR);
        delay_test(10000);

        post_test = i2c_read_byte(DEVICE_ADDR,THI_ADDR);
        log_Str(Test, main_fun, "THigh register value after write is");
        log_Int(post_test);

    	log_Str(Test, main_fun, "State driven mode");
    	log_Str(Test, main_fun, "***************************************");
        while(i)
        {
            switch(Next_State)
            {
                case Temp_Read: log_Str(Test, main_fun, "Temperature Read State");
                                if(flag == 1)
                                {
                                    Next_State = input_Temp();
                                }
                                else
                                {
                                    Next_State = (*SM[structvar].handler)();
                                }
                                break;

                case Temp_Alert: log_Str(Test, main_fun, "Temperature Alert State");
                                 if(flag == 1)
                                 {
                                    Next_State = alert();
                                  	UCUNIT_TestcaseBegin("State Machine 1 - Temp_Alert State Test");
                                  	UCUNIT_CheckIsEqual(Next_State, Temp_Avg_Wait);
                                  	UCUNIT_TestcaseEnd();
                                 }
                                 else
                                 {
                                 	UCUNIT_TestcaseBegin("State Machine 2 - Temp_Alert Test");
                                 	UCUNIT_CheckIsEqual(structvar, 4);
                                 	UCUNIT_TestcaseEnd();
                                    Next_State = (*SM[structvar].handler)();
                                 }
                             	UCUNIT_TestcaseBegin("Temp_Alert Event Test");
                             	UCUNIT_CheckIsEqual(New_Event, Complete);
                             	UCUNIT_TestcaseEnd();
                                break;

                case Temp_Avg_Wait: log_Str(Test, main_fun, "Temperature Average/Wait State");
                                    if(flag == 1)
                                    {
                                        Next_State = avg_Temp();
                                      	UCUNIT_TestcaseBegin("State Machine 1 - Temp_Avg State Test");
                                      	UCUNIT_CheckIsEqual(Next_State, Temp_Avg_Wait);
                                      	UCUNIT_TestcaseEnd();
                                    }
                                    else
                                    {
                                     	UCUNIT_TestcaseBegin("Temp_Avg_Wait Verify");
                                     	UCUNIT_CheckIsEqual(structvar, 5);
                                     	UCUNIT_TestcaseEnd();
                                        Next_State = (*SM[structvar].handler)();
                                    }
                                 	UCUNIT_TestcaseBegin("Temp_Avg Event Timeout_1 Test");
                                 	UCUNIT_CheckIsEqual(New_Event, Timeout_1);
                                 	UCUNIT_TestcaseEnd();

                                 	UCUNIT_TestcaseBegin("Temp_Avg Event Timeout_2 Test");
                                 	UCUNIT_CheckIsEqual(New_Event, Timeout_2);
                                 	UCUNIT_TestcaseEnd();

                                 	UCUNIT_TestcaseBegin("Temp_Avg Event Timeout_3 Test");
                                 	UCUNIT_CheckIsEqual(New_Event, Timeout_3);
                                 	UCUNIT_TestcaseEnd();

                                 	UCUNIT_TestcaseBegin("Temp_Avg Event Timeout_4 Test");
                                 	UCUNIT_CheckIsEqual(New_Event, Timeout_4);
                                 	UCUNIT_TestcaseEnd();
                                    break;

                case Disconnected:  log_Str(Test, main_fun, "Disconnected State");
                                   if(flag == 1)
                                   {
                                	   	Next_State = error_Temp();
                                    	UCUNIT_TestcaseBegin("State Machine 1 - Disconnected State Test");
                                    	UCUNIT_CheckIsEqual(Next_State, End_State);
                                    	UCUNIT_TestcaseEnd();
                                   }
                                   else
                                   {
                                    	UCUNIT_TestcaseBegin("Disconnected Verify");
                                    	UCUNIT_CheckIsEqual(structvar, 3);
                                    	UCUNIT_TestcaseEnd();
                                        Next_State = (*SM[structvar].handler)();
                                   }
                                   break;

                case State_Change: log_Str(Test, main_fun, "State Change");
                                   if(flag == 1)
                                   {
                                       	Next_State = change_State();
                                      	UCUNIT_TestcaseBegin("State Machine 1 - State Change State Test");
                                      	UCUNIT_CheckIsEqual(Next_State, Temp_Read);
                                      	UCUNIT_TestcaseEnd();
                                   }
                                   else
                                   {
                                    	UCUNIT_TestcaseBegin("State_Change Verify");
                                    	UCUNIT_CheckIsEqual(structvar, 1);
                                    	UCUNIT_TestcaseEnd();
                                        Next_State = (*SM[structvar].handler)();
                                   }
                                   break;

                case End_State: log_Str(Test, main_fun, "The program has terminated.");
                				terminate_flag = 1;
                				break;

                default: log_Str(Test, main_fun, "Default case");
                		 break;
            }

            if(terminate_flag)
            	break;
            i--;
        }
    }
    else
    {
      	UCUNIT_TestcaseBegin("Power On Self Test");
      	UCUNIT_CheckIsEqual(post_flag,1);
      	UCUNIT_TestcaseEnd();
    }
	/* Finish all the tests */
	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
	return;
}

/***************************************** References **************************************************
[1] https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/

********************************************************************************************************/
