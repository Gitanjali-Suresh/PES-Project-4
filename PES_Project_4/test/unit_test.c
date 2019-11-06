/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 3 - Memory Functions and Tests                    *
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
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/********* User defined Header Files *********/
#include "logger.h"
#include "uCUnit.h"

/********* Error code definitions *********/
typedef enum
{
    Temp_Read,
    Temp_Alert,
    Temp_Avg_Wait,
    Disconnected,
    State_Change,
    Last_State
}eTempState;

typedef enum
{
    Start_Event,
    Timeout_1,
    Timeout_2,
    Timeout_3,
    Timeout_4,
    Complete,
    Alert,
    Disconnect,
    Last_Event
}eTempEvent;

typedef eTempState (*Event_Handler)(void);

typedef struct
{
    eTempState state;
    eTempEvent event;
    Event_Handler handler;
}sState_Machine_2;

volatile uint32_t temp, temp_avg = 25;
volatile uint16_t count = 0, flag = 1, structvar = 0;
volatile eTempState Next_State;
volatile eTempEvent New_Event;

void delay(int milliseconds);
eTempState input_Temp(void);
eTempState alert(void);
eTempState avg_Temp(void);
eTempState error_Temp(void);
eTempState change_State(void);

sState_Machine_2 SM[] = {
                            {Temp_Read,Start_Event,input_Temp},
                            {Temp_Read,Timeout_1,input_Temp},
                            {Temp_Read,Timeout_2,input_Temp},
                            {Temp_Read,Timeout_3,input_Temp},
                            {Temp_Alert,Alert,alert},
                            {Temp_Avg_Wait,Complete,avg_Temp},
                            {Disconnected,Disconnect,error_Temp},
                            {State_Change,Timeout_4,change_State},
                        };

/********* Unit Test function definition *********/
int main()
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

	Next_State = Temp_Read;
	UCUNIT_Init(); /* initialize framework */
	log_Str(Test, main_fun, "State driven mode");
	log_Str(Test, main_fun, "***************************************");
    while(1)
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
                             }
                             else
                             {
                             	UCUNIT_TestcaseBegin("Temp_Alert Verify");
                             	UCUNIT_CheckIsEqual(structvar, 4);
                             	UCUNIT_TestcaseEnd();
                                Next_State = (*SM[structvar].handler)();
                             }
                             break;

            case Temp_Avg_Wait: log_Str(Test, main_fun, "Temperature Average/Wait State");
                                if(flag == 1)
                                {
                                    Next_State = avg_Temp();
                                }
                                else
                                {
                                 	UCUNIT_TestcaseBegin("Temp_Avg_Wait Verify");
                                 	UCUNIT_CheckIsEqual(structvar, 5);
                                 	UCUNIT_TestcaseEnd();
                                    Next_State = (*SM[structvar].handler)();
                                }
                                break;

            case Disconnected:  log_Str(Test, main_fun, "Disconnected State");
                               if(flag == 1)
                               {
                                  Next_State = error_Temp();
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
                               }
                               else
                               {
                                	UCUNIT_TestcaseBegin("State_Change Verify");
                                	UCUNIT_CheckIsEqual(structvar, 1);
                                	UCUNIT_TestcaseEnd();
                                    Next_State = (*SM[structvar].handler)();
                               }
                               break;

            default: log_Str(Test, main_fun, "Default case");
            		 break;
        }
    }
	/* Finish all the tests */
	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
	return 0;
}

eTempState input_Temp(void)
{
	temp = rand();
	log_Str(Test, read_Temp, "The current temperature:");
	log_Int(temp);
	log_Char('C');
    if(temp <= 0)
    {
        New_Event = Alert;
        Next_State = Temp_Alert;
        structvar = 4;
    }
    else if(temp > 0)
    {
        New_Event = Complete;
        Next_State = Temp_Avg_Wait;
        structvar = 5;
    }
    else
    {
    	log_Str(Test, read_Temp, "Unable to read from sensor.");
        New_Event = Disconnect;
        Next_State = Disconnected;
        structvar = 6;
    }
    return Next_State;
}

eTempState alert(void)
{
	log_Str(Test, alert_Temp, "Temperature below or equal to 0!!");
    New_Event = Complete;
    Next_State = Temp_Avg_Wait;
    structvar = 5;
    return Next_State;
}

eTempState avg_Temp(void)
{
	log_Str(Test, wait_fun, "The last average temperature value is");
	log_Int(temp_avg);
	log_Char('C');
    temp_avg = (temp_avg + temp)/2;
    log_Str(Test, wait_fun, "The current average temperature value is");
	log_Int(temp_avg);
	log_Char('C');
    count++;
    //delay(2000);
    if(count == 1)
    {
    	log_Str(Test, wait_fun, "Timeout 1...");
        New_Event = Timeout_1;
        Next_State = Temp_Read;
        structvar = 1;
    }
    else if(count == 2)
    {
    	log_Str(Test, wait_fun, "Timeout 2...");
        New_Event = Timeout_2;
        Next_State = Temp_Read;
        structvar = 2;
    }
    else if(count == 3)
    {
    	log_Str(Test, wait_fun, "Timeout 3...");
        New_Event = Timeout_3;
        Next_State = Temp_Read;
        structvar = 3;
    }
    else
    {
    	log_Str(Test, wait_fun, "Timeout 4...");
        count = 0;
        New_Event = Timeout_4;
        Next_State = State_Change;
        structvar = 7;
    }
    //PRINTF("\n\r");
    return Next_State;
}

eTempState error_Temp(void)
{
	log_Str(Test, wrong_Temp, "The temperature couldn't be read.");
    New_Event = Start_Event;
    Next_State = Temp_Read;
    structvar = 0;
    return Next_State;
}

eTempState change_State(void)
{
    if(flag == 1)
    {
        flag = 2;
        log_Str(Test, change_fun, "State changed to");
    	log_Int(flag);
    	log_Str(Test, change_fun, "Table driven mode");
    	log_Str(Test, change_fun, "***************************************");
        structvar = 0;
    }
    else
    {
        flag = 1;
        log_Str(Test, change_fun, "State changed to");
    	log_Int(flag);
    	log_Str(Test, change_fun, "State driven mode");
    	log_Str(Test, change_fun, "***************************************");
    }
    New_Event = Start_Event;
    Next_State = Temp_Read;
    return Next_State;
}


/***************************************** References **************************************************
[1] https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/

********************************************************************************************************/
