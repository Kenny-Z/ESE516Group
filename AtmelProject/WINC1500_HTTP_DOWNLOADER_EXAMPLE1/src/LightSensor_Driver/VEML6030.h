/**************************************************************************//**
* @file      VEML6030.h
* @brief     Driver for light sensor VEML6030
* @author    Kenny Zhang
* @date      2021-04-14

******************************************************************************/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include <stdint.h>
/******************************************************************************
* Defines
******************************************************************************/
#define VEML6030_I2C_ADDR_H 0x48												 // ADDR pin plugged to Vdd
#define VEML6030_I2C_ADDR_L 0x10												// ADDR pin plugged to Ground

#define  ALS_CONF 0X00															//ALS gain, integration time, interrupt, and shut down
#define  ALS_WH 0X01															//ALS high threshold window setting
#define  ALS_WL 0X02															//ALS low threshold window setting
#define  POWER_SAVING 0X03														//Set (15 : 3) 0000 0000 0000 0b
#define  ALS 0X04																//ALS DATA
#define  WHITE 0X05																//data of whole WHITE
#define  ALS_INT 0X06															//ALS INT trigger event

#define VemlReadByteLen 2																//
#define VemlWriteByteLen 2	

/******************************************************************************
* Structures and Enumerations
******************************************************************************/



/******************************************************************************
* Global Function Declaration
******************************************************************************/
static int32_t platform_read(uint8_t reg, uint8_t *bufp, uint16_t len);
static int32_t platform_write(uint8_t reg, uint8_t *bufp,uint16_t len);
int32_t VEML_Reset(void);
int32_t VEML_Filter_L_Threshold(void);
int32_t VEML_Filter_H_Threshold(void);
int32_t VEML_Power_Saving(void);
int32_t VEML_Read_Power_Saving(uint32_t *readdata);
int32_t VEML_ReadALSData(uint32_t * readdata);
int32_t VEML_Power_Off(void);
int32_t VEML_Power_On(void);
int32_t VEML_Read_Config(uint32_t *readdata);
#ifdef __cplusplus
}
#endif