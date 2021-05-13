/**************************************************************************//**
* @file      LightThread.c
* @brief     File for Light Sensor Reading and Keyboard color changing
* @author    Kenny Zhang
* @date      2021-05-11

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "LightThread.h"
#include "LightSensor_Driver/VEML6030.h"
#include "SerialConsole/SerialConsole.h"
#include "stdio.h"
#include "FreeRTOS_Threads/UiHandlerThread/UiHandlerThread.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/

/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/


/**************************************************************************//**
void vLightReadTask( void *pvParameters )
* @brief	Free RTOS task to change RGB color depending on ALS reading
* @param[out] 
                				
* @return		
* @note         

*****************************************************************************/

void vLightReadTask( void *pvParameters )
{

/* The input and output buffers are declared static to keep them off the stack. */
	uint32_t lightdata = 0;
	VEML_Reset();
	delay_ms(100);
    /* Send a welcome message to the user knows they are connected. */
	VEML_Power_On();
    for( ;; )
    {
		VEML_ReadALSData(&lightdata);
		uint8_t R = lightdata/10;
		uint8_t G = lightdata/20;
		uint8_t B = 255 - lightdata/10;
		UIChangeColors(R,G,B);


		vTaskDelay(4000);
	}
}