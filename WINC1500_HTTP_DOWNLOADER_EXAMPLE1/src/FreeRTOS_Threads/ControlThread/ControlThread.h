/**************************************************************************//**
* @file      ControlThread.h
* @brief     Thread code for the ESE516 Online game control thread
* @author    You!
* @date      2020-04-015

******************************************************************************/



 #pragma once
 #ifdef __cplusplus
 extern "C" {
	 #endif
 
/******************************************************************************
* Includes
******************************************************************************/
#include "FreeRTOS_Threads/WifiHandlerThread/WifiHandler.h"
/******************************************************************************
* Defines
******************************************************************************/
#define CONTROL_TASK_SIZE			256//<Size of stack to assign to the UI thread. In words
#define CONTROL_TASK_PRIORITY		(configMAX_PRIORITIES - 1)
typedef enum controlStateMachine_state
{
	CONTROL_WAIT_FOR_STATUS = 0,
	CONTROL_WAIT_FOR_GAME, ///<State used to WAIT FOR A GAME COMMAND
	CONTROL_PLAYING_MOVE, ///<State used to wait for user to play a game move
	CONTROL_END_GAME, ///<State to show game end
	CONTROL_STATE_MAX_STATES	///<Max number of states

}controlStateMachine_state;

typedef enum GAME_STATUS
{
	Initialize = 0, 
	P2_turn,
	P1_turn, 
	P1_Lose,
	P2_Lose,
	GAME_STATUS_MAX_STATES	///<Max number of states

}GAME_STATUS;
/******************************************************************************
* Structures and Enumerations
******************************************************************************/



/******************************************************************************
* Global Function Declaration
******************************************************************************/
void vControlHandlerTask( void *pvParameters );
int ControlAddGameData(struct GameDataPacket *gameIn);
int ControlAddStatusDataToQueue(uint8_t *statusdada);
	 #ifdef __cplusplus
 }
 #endif