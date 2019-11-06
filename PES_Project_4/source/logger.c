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
/*****************************************************************************
 * LOGGER
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdint.h>

/********* User defined Header Files *********/
#include "logger.h"

/********* Board specific Header Files *********/
#include "fsl_debug_console.h"

/********** Function to print different modes of the project **********/
void log_Level(eLog_level mode)
{
	if(mode == Status)
	{
		PRINTF("\n\rSTATUS: ");
	}
	else if(mode == Debug)
	{
		PRINTF("\n\rDEBUG: ");
	}
	else
	{
		PRINTF("\n\rTEST: ");
	}
	return;
}

/********** Function to print different function names **********/
void log_Str(eLog_level mode, eFunction_name func, char *string)
{
	log_Level(mode);
	if(func == read_Temp)
	{
		PRINTF("input_Temp: ");
	}
	else if(func == alert_Temp)
	{
		PRINTF("alert: ");
	}
	else if(func == wait_fun)
	{
		PRINTF("avg_Temp: ");
	}
	else if(func == wrong_Temp)
	{
		PRINTF("error_Temp: ");
	}
	else if(func == main_fun)
	{
		PRINTF("main: ");
	}
	else if(func == test_init)
	{
		PRINTF("System_Init: ");
	}
	else if(func == test_shut)
	{
		PRINTF("System_Shutdown: ");
	}
	else if(func == str_write)
	{
		PRINTF("System_WriteString: ");
	}
	else if(func == int_write)
	{
		PRINTF("System_WriteInt: ");
	}
	else if(func == test_recover)
	{
		PRINTF("System_Recover: ");
	}
	else if(func == test_safe)
	{
		PRINTF("System_Safestate: ");
	}
	else
	{
		PRINTF("change_State: ");
	}
	PRINTF("%s ",string);
	return;
}

/********** Function to print a number **********/
void log_Int(int32_t number)
{
	PRINTF("%d ",number);
}

/********** Function to print a character **********/
void log_Char(char ch)
{
	PRINTF("%c ",ch);
}

/********** Function to print a string (used only for uc testing) **********/
void log_string(char *str)
{
	PRINTF("%s",str);
}
