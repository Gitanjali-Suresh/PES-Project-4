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
 * I2C PROTOCOL TRANSACTIONS
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/********* Board specific Header Files *********/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"

/********* User defined Header Files *********/
#include "i2c.h"
#include "tmp102.h"

uint8_t data[10], dis_count, alert_val = 0x00;

/********* I2C initialization function definition *********/
void i2c_init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;	//Clock to i2C1 module is enabled
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Clock enabled to Port E

	PORTE->PCR[0] |= PORT_PCR_MUX(6);	//PE0 and 1 configured with alternative functionality 6 (I2C1)
	PORTE->PCR[1] |= PORT_PCR_MUX(6);

	//Setting the baud rate to 400kHz
	I2C1->F = I2C_F_ICR(0x11) | I2C_F_MULT(0);

	//Enabling I2C and setting master mode
	I2C1->C1 |= (I2C_C1_IICEN_MASK);

	//Selection of high drive mode
	I2C1->C2 |= I2C_C2_HDRS_MASK;
	I2C1->SLTH |= I2C_SLTH_SSLT(0x01);	//Configuration of I2C timeout registers
}

#define I2C_M_START   I2C1->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP    I2C1->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART  I2C1->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN      I2C1->C1 |= I2C_C1_TX_MASK
#define I2C_REC       I2C1->C1 &= ~I2C_C1_TX_MASK

#define NACK          I2C1->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C1->C1 &= ~I2C_C1_TXAK_MASK

/********* Reading bytes function definition *********/
int32_t i2c_read_bytes(uint8_t dev_addr, uint8_t reg_addr)
{
    int temperature, temperatureC, temperatureF;

    I2C_TRAN;
    I2C_M_START;

    I2C1->D = dev_addr;	//Writing device address
    I2C_WAIT();
    I2C1->D = reg_addr; //Writing register address
    I2C_WAIT();

    I2C_M_RSTART;		//Repeated Start

    I2C1->D = (dev_addr|0x1);		
    I2C_WAIT();			//Wait for completion

    I2C_REC;  			//Setting it in Tx Mode
    ACK; 				//Make sure TX ACK

    data[0] = I2C1->D;  //Dummy read for internal store
    I2C_WAIT();			//Wait for completion

    data[0] = I2C1->D;  //Reading first data byte
    I2C_WAIT();			//Wait for completion

    NACK;

    data[1] = I2C1->D;  //Reading second databyte
    I2C_WAIT();			//Wait for completion

    I2C_M_STOP;

    if((data[1] == 0 && data[0] == 0) || dis_count < 0 || data[0] == 144)
    {
    	return -1;		//Disconnected state
    }
    else
    {
        temperature = ((data[0] << 4) + (data[1] >> 4));
        temperatureC = temperature * 0.0625;		//Calculation of temperature in Celsius
        temperatureF = (temperatureC * 1.8) + 32;	//Calculation of temperature in Fahrenheit
       
        return temperatureF;
    }
}

/********* I2C wait function definition *********/
void I2C_WAIT(void)
{
	  dis_count = 255;
	  while((I2C1->S & I2C_S_IICIF_MASK)==0)
	  {
		if((I2C1->C1 & I2C_C1_TXAK_MASK) == 0)		//Wait for Acknowledgement
		{
				dis_count--;
				if(dis_count < 0)
				{
					I2C1->S |= I2C_S_IICIF_MASK;
					return;
				}
		}
	  }
	  I2C1->S |= I2C_S_IICIF_MASK;
	  return;
}

/********* Writing bytes function definition *********/
void i2c_write_bytes(uint8_t dev, uint8_t reg, uint8_t data[], uint8_t bytes)
{
	volatile int i;

	I2C_TRAN;
	I2C_M_START;

	I2C1->D = dev;		//Writing device address
	I2C_WAIT();			//Wait for completion

	I2C1->D = reg;  	//Writing register address
	I2C_WAIT();			//Wait for completion

	for(i = 0 ;i <  bytes; i++) //Writing MSB first
	{
		I2C1->D = data[i];
		I2C_WAIT();				//Wait for completion
		
	}

	I2C_M_STOP;
}

/********* Reading a single byte function definition *********/
uint8_t i2c_read_byte(uint8_t dev, uint8_t reg)
{
	uint32_t data;

	I2C_TRAN;		//Setting I2C in transmit mode
	I2C_M_START;

	I2C1->D = dev;	//Writing device address
	I2C_WAIT();		//Wait for completion

	I2C1->D = reg;	//Writing register address
	I2C_WAIT();		//Wait for completion

	I2C_M_RSTART;  //Repeated start
	I2C1->D = (dev|0x1);

	I2C_WAIT();		//Wait for completion

	I2C_REC;
	NACK;

	data = I2C1->D;		//Reading of data

	I2C_WAIT();			//Wait for completion

	I2C_M_STOP;
    data = I2C1->D;

	return data;
}

/********* TLow initialization function definition *********/
void Init_Alert_TLO(uint8_t dev, uint8_t reg)
{
    uint8_t tlo[2];

    tlo[0] = alert_val;
    tlo[1] = 0x00;
    i2c_write_bytes(dev, reg, tlo, 2);

    return;
}

/********* THigh initialization function definition *********/
void Init_Alert_THI(uint8_t dev, uint8_t reg)
{
    uint8_t thi[2];

    thi[0] = alert_val;
    thi[1] = 0x00;
    i2c_write_bytes(dev, reg, thi, 2);

    return;
}
