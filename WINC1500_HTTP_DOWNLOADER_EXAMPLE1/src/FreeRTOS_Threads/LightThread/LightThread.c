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
//		VEML_Power_On();
		// After starting sensor, wait 200ms to collect data
// 		vTaskDelay();
//		taskENTER_CRITICAL();
		VEML_ReadALSData(&lightdata);
//		taskEXIT_CRITICAL();
		uint8_t R = lightdata/10;
		uint8_t G = lightdata/20;
		uint8_t B = 255 - lightdata/10;
		UIChangeColors(R,G,B);
 		//SerialConsoleWriteString("Light!\r\n");
// 		SerialConsoleWriteString(sensormsg);
// 		vTaskDelay(500);
// 		VEML_Power_Saving();
// 		vTaskDelay(500);
// 		VEML_Read_Power_Saving(&powersaving);
// 		snprintf(sensormsg, 15, "Power saving:%d\r\n", powersaving);
// 		SerialConsoleWriteString(sensormsg);
//		VEML_Power_Off();

		vTaskDelay(4000);
	}
}