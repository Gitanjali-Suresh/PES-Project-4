/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : System.c                                                   *
 *  Description : System dependent functions used by uCUnit.                 *
 *                This file runs with arm-elf-run                            *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */
#include <stdio.h>
#include <stdlib.h>
#include "System.h"

#include "logger.h"

void System_exit(int val) {
	for(;;) {
		/* we don't do a shutdown with exit(), as this is will affect debugging.
		 * Instead, we stay here in an endless loop.
		 */
		__asm("nop"); /* burning some CPU cycles here */
	}
	// exit(val);
}

/* Stub: Initialize your hardware here */
void System_Init(void)
{
	log_Str(Test, test_init, "Initialization of hardware finished.");
}

/* Stub: Shutdown your hardware here */
void System_Shutdown(void)
{
	log_Str(Test, test_shut, "System shutdown!");

	System_exit(0);
}

/* Stub: Recover the system */
void System_Recover(void)
{
	log_Str(Test, test_recover, "System reset.");
	System_exit(0);
}

/* Stub: Put system in a safe state */
void System_Safestate(void)
{
	log_Str(Test, test_safe, "System safe state.");
	System_exit(0);
}

/* Stub: Transmit a string to the host/debugger/simulator */
void System_WriteString(char * msg)
{
	log_Str(Test, str_write, msg);
}

void System_WriteInt(int n)
{
	log_Int(n);
}

/***************************************** References **************************************************
 https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/

********************************************************************************************************/
