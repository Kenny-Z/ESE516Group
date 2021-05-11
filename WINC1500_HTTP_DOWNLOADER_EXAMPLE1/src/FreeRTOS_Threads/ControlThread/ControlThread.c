/**************************************************************************//**
* @file      ControlThread.c
* @brief     Thread code for the ESE516 Online game control thread
* @author    You!
* @date      2020-04-015

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "FreeRTOS_Threads/ControlThread/ControlThread.h"
#include "FreeRTOS_Threads/WifiHandlerThread/WifiHandler.h"
#include "FreeRTOS_Threads/UiHandlerThread/UiHandlerThread.h"
#include <errno.h>
#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include "SerialConsole.h"
#include "shtc3.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/
QueueHandle_t xQueueGameBufferIn = NULL; ///<Queue to send the next play to the UI
QueueHandle_t xQueueRgbColorBuffer = NULL; ///<Queue to receive an LED Color packet
QueueHandle_t xQueueStatusBuffer = NULL; ///<Queue to send the distance to the cloud

controlStateMachine_state controlState; ///<Holds the current state of the control thread
GAME_STATUS gameStatus;
/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/


/******************************************************************************
* Task Functions
******************************************************************************/


/**************************************************************************//**
* @fn		void vUiHandlerTask( void *pvParameters )
* @brief	STUDENT TO FILL THIS
* @details 	student to fill this
                				
* @param[in]	Parameters passed when task is initialized. In this case we can ignore them!
* @return		Should not return! This is a task defining function.
* @note         
*****************************************************************************/
void vControlHandlerTask( void *pvParameters )
{
	SerialConsoleWriteString("ESE516 - Control Init Code\r\n");

	//Initialize Queues
	xQueueGameBufferIn = xQueueCreate( 2, sizeof( struct GameDataPacket ) );
	xQueueRgbColorBuffer = xQueueCreate( 2, sizeof( struct RgbColorPacket ) );
	xQueueStatusBuffer  = xQueueCreate( 5, sizeof( uint8_t ) );

	if(xQueueGameBufferIn == NULL || xQueueRgbColorBuffer == NULL || xQueueStatusBuffer==NULL){
		SerialConsoleWriteString("ERROR Initializing Control Data queues!\r\n");
	}
	controlState = CONTROL_WAIT_FOR_STATUS; //Initial state

	while(1)
	{
		switch(controlState)
		{
			case (CONTROL_WAIT_FOR_STATUS):
			{	//Should set the UI to ignore button presses and should wait until there is a message from the server with a new play.
				uint8_t gamestatus;
				if(pdPASS == xQueueReceive( xQueueStatusBuffer , &gamestatus, 0 ))
				{
					switch (gamestatus){
// 						case Initialize:{
// 							// OLED PRINT P2 TURN
// 							controlState = CONTROL_WAIT_FOR_STATUS;
// 							break;
// 						}
						case P2_turn:{
							controlState = CONTROL_WAIT_FOR_STATUS;
							break;							
						}
						case P1_turn:{	// OLED PRINT YOUR TURN
							controlState = CONTROL_WAIT_FOR_GAME;	
							break;
						}
						case P1_Lose:{
						// OLED PRINT YOU LOSE // End game
						controlState = CONTROL_WAIT_FOR_STATUS;
						break;
						}
						case P2_Lose:{
						// OLED PRINT YOU WIN // End game
						controlState = CONTROL_WAIT_FOR_STATUS;
						break;
						}
						default:{
						controlState = CONTROL_WAIT_FOR_STATUS;
						break;	
											
						}
					}
				}

			}
			case (CONTROL_WAIT_FOR_GAME):
			{	//Should set the UI to ignore button presses and should wait until there is a message from the server with a new play.
				struct GameDataPacket gamePacketIn;
				if(pdPASS == xQueueReceive( xQueueGameBufferIn , &gamePacketIn, 0 ))
				{
					LogMessage(LOG_DEBUG_LVL, "Control Thread: Consumed game packet!\r\n");
					UiOrderShowMoves(&gamePacketIn);
					controlState = CONTROL_PLAYING_MOVE;
				}
		
				break;
			}

			case (CONTROL_PLAYING_MOVE):
			{	//Should wait until the UI thread has showed the move AND comes back with the play from the user. Should go back to CONTROL_WAIT_FOR_GAME
				//after posting the game to MQTT
				if(UiPlayIsDone() == true)
				{
					//Send back local game packet
					if( pdTRUE != WifiAddGameDataToQueue(UiGetGamePacketOut()))
					{
						LogMessage(LOG_DEBUG_LVL, "Control Thread: Could not send game packet!\r\n");
					}
					controlState = CONTROL_WAIT_FOR_GAME;
				}

				break;
			}

			case (CONTROL_END_GAME):
			{	

				break;
			}


			default:
				controlState = CONTROL_WAIT_FOR_STATUS;
			break;

		}
	vTaskDelay(40);

	}


}



/**************************************************************************//**
int ControlAddGameData(struct GameDataPacket *gameIn);
* @brief	Adds an game data received from the internet to the local control for play
* @param[out] 
                				
* @return		Returns pdTrue if data can be added to queue, 0 if queue is full
* @note         

*****************************************************************************/
int ControlAddGameData(struct GameDataPacket *gameIn)
{
	int error = xQueueSend(xQueueGameBufferIn , gameIn, ( TickType_t ) 10);
	return error;
}

/**************************************************************************//**
void WifiAddImuDataToQueue(struct ImuDataPacket* imuPacket)
* @brief	Adds an IMU struct to the queue to send via MQTT
* @param[out]

* @return		Returns pdTrue if data can be added to queue, pdFalse if queue is full
* @note

*****************************************************************************/
int ControlAddStatusDataToQueue(uint8_t *statusdada)
{
	int error = xQueueSend(xQueueStatusBuffer , statusdada, ( TickType_t ) 10);
	return error;
}