/**************************************************************************//**
* @file      CliThread.c
* @brief     File for the CLI Thread handler. Uses FREERTOS + CLI
* @author    Eduardo Garcia
* @date      2021-04-15

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "SensorThread.h"
#include "LightSensor/VEML6030.h"
#include "SerialConsole/SerialConsole.h"
#include "stdio.h"
/******************************************************************************
* Defines
******************************************************************************/
char sensormsg[20] = "";

/******************************************************************************
* Variables
******************************************************************************/
static const int8_t * const pcWelcomeMessage =
"FreeRTOS Sensor Task.\r\n";

/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/




void vSensorReadTask( void *pvParameters )
{

/* The input and output buffers are declared static to keep them off the stack. */
	uint32_t lightdata = 0;
	uint32_t powersaving = 0;
	uint32_t config = 0;

    /* This code assumes the peripheral being used as the console has already
    been opened and configured, and is passed into the task as the task
    parameter.  Cast the task parameter to the correct type. */
	//VEML_Power_Saving();
	//VEML_Filter_L_Threshold();
	delay_ms(100);
	VEML_Reset();
	//VEML_Filter_H_Threshold();
	delay_ms(100);
	//VEML_Power_Saving();
	delay_ms(100);
    /* Send a welcome message to the user knows they are connected. */
    SerialConsoleWriteString( pcWelcomeMessage);
	VEML_Power_On();
    for( ;; )
    {
// 		VEML_Power_Off();
// 		vTaskDelay(500);
// 		VEML_Read_Config(&config);
// 		snprintf(sensormsg, 15, "Config:%d\r\n", config);
// 		SerialConsoleWriteString(sensormsg);
// 		VEML_Power_On();
// 		vTaskDelay(1000);
		VEML_Power_On();
 		vTaskDelay(200);
// 		VEML_Read_Config(&config);
// 		snprintf(sensormsg, 15, "Config:%d\r\n", config);
// 		SerialConsoleWriteString(sensormsg);
/* 		vTaskDelay(500);*/
		VEML_ReadALSData(&lightdata);
// 		snprintf(sensormsg, 15, "Light ALS: %d\r\n", lightdata);
// 		SerialConsoleWriteString(sensormsg);
// 		vTaskDelay(500);
// 		VEML_Power_Saving();
// 		vTaskDelay(500);
// 		VEML_Read_Power_Saving(&powersaving);
// 		snprintf(sensormsg, 15, "Power saving:%d\r\n", powersaving);
//		SerialConsoleWriteString(sensormsg);
		VEML_Power_Off();

		vTaskDelay(1000);
	}
}