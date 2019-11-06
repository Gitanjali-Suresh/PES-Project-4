#include <stdint.h>
#include "logger.h"
#include "fsl_debug_console.h"

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

uint16_t message_count = 0;

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

void log_Int(int number)
{
	PRINTF("%d ",number);
}

void log_Char(char ch)
{
	PRINTF("%c ",ch);
}
