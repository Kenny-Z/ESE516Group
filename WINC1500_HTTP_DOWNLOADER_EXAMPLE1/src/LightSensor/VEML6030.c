/**************************************************************************//**
* @file      VEML6030.c
* @brief     Ambient Light Senor Driver: Doxygen-style comments
* @author    Kenny Zhang
* @date      2021-04-14

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "I2CDriver/I2cDriver.h"
#include "VEML6030.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/
I2C_Data light_data;
uint8_t msgOutlight[10];					//used as the final output to I2C interface
uint8_t msgOutlightbuffer[2];			//used as input for platform_write
uint8_t msgInlightbuffer[2];			//used as input for platform_write	    	      
/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/
/**************************************************************************//**
* @fn		int32_t VEML_Configure(void)
* @brief	Configure the VEML
* @details 
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Reset(void)
{
	int32_t error = ERROR_NONE;
	//ALS Gain:1
	//Integration time: 100ms
	//Interrupt disable
	//ALS power on
	msgOutlightbuffer[0] = 0x00;
	msgOutlightbuffer[1] = 0x00;
	error = platform_write(ALS_CONF, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(ALS_WH, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(ALS_WL, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(POWER_SAVING, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(ALS, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(WHITE, msgOutlightbuffer,VemlWriteByteLen);
	error = platform_write(ALS_INT, msgOutlightbuffer,VemlWriteByteLen);
	return error;
}
/**************************************************************************//**
* @fn		int32_t VEML_Configure(void)
* @brief	Configure the VEML
* @details 
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Power_On(void)
{
	int32_t error = ERROR_NONE;
	//ALS Gain:1
	//Integration time: 100ms
	//Interrupt disable
	//ALS power on
	msgOutlightbuffer[0] = 0x00;
	msgOutlightbuffer[1] = 0x08;
	error = platform_write(ALS_CONF, msgOutlightbuffer,VemlWriteByteLen);
	return error;
}

/**************************************************************************//**
* @fn		int32_t VEML_Power_Saving(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Read_Config(uint32_t *readdata)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	error = platform_read(ALS_CONF, msgInlightbuffer,VemlReadByteLen);
	*readdata = (uint32_t)msgInlightbuffer[0] | ((uint32_t)msgInlightbuffer[1] <<8);
	return error;
}

/**************************************************************************//**
* @fn		int32_t VEML_Configure(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Power_Off(void)
{
	int32_t error = ERROR_NONE;
	//ALS Gain:1
	//Integration time: 100ms
	//Interrupt disable
	//ALS power on
	msgOutlightbuffer[0] = 0x01;
	msgOutlightbuffer[1] = 0x00;
	error = platform_write(ALS_CONF, msgOutlightbuffer,VemlWriteByteLen);
	return error;
}
/**************************************************************************//**
* @fn		int32_t VEML_Power_Saving(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Power_Saving(void)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	msgOutlightbuffer[0] = 0x00;
	msgOutlightbuffer[1] = 0x00;
	error = platform_write(POWER_SAVING, msgOutlightbuffer,VemlReadByteLen);
	return error;
}
/**************************************************************************//**
* @fn		int32_t VEML_Power_Saving(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Read_Power_Saving(uint32_t *readdata)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	error = platform_read(POWER_SAVING, msgInlightbuffer,VemlReadByteLen);
	*readdata = (uint32_t)msgInlightbuffer[0] | ((uint32_t)msgInlightbuffer[1] <<8);
	return error;
}
/**************************************************************************//**
* @fn		int32_t VEML_Filter_H_Threshold(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Filter_H_Threshold(void)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	msgOutlightbuffer[0] = 0x00;
	msgOutlightbuffer[1] = 0x00;
	error = platform_write(ALS_WH, msgOutlightbuffer,VemlReadByteLen);
	return error;
}

/**************************************************************************//**
* @fn		int32_t VEML_Filter_H_Threshold(void)
* @brief	Configure the VEML
* @details
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_Filter_L_Threshold(void)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	msgOutlightbuffer[0] = 0x00;
	msgOutlightbuffer[1] = 0x00;
	error = platform_write(ALS_WL, msgOutlightbuffer,VemlReadByteLen);
	return error;
}

/**************************************************************************//**
* @fn		int32_t VEML_ReadALSData(void)
* @brief	read
* @details
* @param[in]	uint32 * readdata: the data is 16 bits, but stored in a 32bit register
* @return		STATUS_OK if ok
* @note
*****************************************************************************/

int32_t VEML_ReadALSData(uint32_t * readdata)
{
	int32_t error = ERROR_NONE;
	// Disable power_saving;
	error = platform_read(ALS, msgInlightbuffer,VemlReadByteLen);
	*readdata = (uint32_t)msgInlightbuffer[0] | ((uint32_t)msgInlightbuffer[1] <<8);
	return error;
}

/**************************************************************************//**
* @fn		static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
* @brief	The platform function for I2C write
* @details 	This function takes in a data to write, and sends out through I2C interface. 
*			Remember in I2C interface, we should transmit the address of register(to write to) first.
* @param[in]	uint8_t reg: the register address
* @param[in]	uint8_t *bufp: the buffer that stores the data to send
* @param[in]	uint16_t len: number of bytes to send (the register address is not counted)
* @return		STATUS_OK if ok
* @note         
*****************************************************************************/

static int32_t platform_write(uint8_t reg, uint8_t *bufp,uint16_t len)
{
	int32_t error = ERROR_NONE;
	light_data.address = VEML6030_I2C_ADDR_H;
	msgOutlight[0] = reg;
	memcpy (msgOutlight+1, bufp, len);
	light_data.lenOut = len +1;
	light_data.msgOut = &msgOutlight;
	error = I2cWriteDataWait(&light_data,100);
	return error;
}

/**************************************************************************//**
* @fn		static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
* @brief	The platform function for I2C read
* @details 	This function takes in a data to write, and sends out through I2C interface.
*			Remember in I2C interface, we should transmit the address of register(to write to) first.
* @param[in]	uint8_t reg: the register address
* @param[in]	uint8_t *bufp: the buffer that stores the data to send
* @param[in]	uint16_t len: number of bytes to send (the register address is not counted)
* @return		STATUS_OK if ok
* @note
*****************************************************************************/
static  int32_t platform_read(uint8_t reg, uint8_t *bufp, uint16_t len)
{
	int32_t error = ERROR_NONE;
	light_data.address = VEML6030_I2C_ADDR_H;
	msgOutlight[0] = reg;
	msgOutlight[1] = 0;
	light_data.lenOut = 1;
	light_data.msgOut = &msgOutlight;
	light_data.lenIn = len;
	light_data.msgIn = bufp;
	error = I2cReadDataWait(&light_data, 5, 100);
	return error;
}