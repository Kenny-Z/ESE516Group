 /**************************************************************************//**
* @file      UiHandlerThread.c
* @brief     File that contains the task code and supporting code for the UI Thread for ESE516 Spring (Online) Edition
* @author    Kenny Zhang and Chen Chen
* @date      2021-05-13

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include <errno.h>
#include "asf.h"
#include "FreeRTOS_Threads/UiHandlerThread/UiHandlerThread.h"
#include "SeesawDriver/Seesaw.h"
#include "FreeRTOS_Threads/WifiHandlerThread/WifiHandler.h"
#include "SerialConsole.h"
#include "main.h"
#include "gfx_mono.h"

/******************************************************************************
* Defines
******************************************************************************/
#define		BUTTON_PRESSES_MAX	16	///<Number of maximum button presses to analyze in one go

/******************************************************************************
* Variables
******************************************************************************/
uiStateMachine_state uiState; ///<Holds the current state of the UI
struct GameDataPacket gamePacketIn; ///<Holds the game packet to show
struct GameDataPacket gamePacketOut;///<Holds the game packet to send back
volatile uint8_t red = 0; ///<Holds the color of the red LEDs. Can be set by MQTT
volatile uint8_t green = 100; ///<Holds the color of the green LEDs. Can be set by MQTT
volatile uint8_t blue = 50; ///<Holds the color of the blue LEDs. Can be set by MQTT

uint8_t pressedKeys = 0; ///<Variable to count how many presses the player has done
uint8_t keysToPress = 0; ///<Variable that holds the number of new key presses the user should do
bool playIsDone = false; ///<Boolean flag to indicate if the player has finished moving. Useful for COntrol to determine when to send back a play.
uint8_t buttons[BUTTON_PRESSES_MAX]; ///<Array to hold button presses
/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/


/******************************************************************************
* Task Function
******************************************************************************/

/**************************************************************************//**
* @fn		void vUiHandlerTask( void *pvParameters )
* @brief	Free RTOS task to handle user inputs
* @details 	display packet received on keypad or 
			register keypad presses and send out as packets or do nothing
* @param[in]	Parameters passed when task is initialized. In this case we can ignore them!
* @return		Should not return! This is a task defining function.
* @note         
*****************************************************************************/
void vUiHandlerTask( void *pvParameters )
{
//Do initialization code here
SerialConsoleWriteString("UI Task Started!");
uiState = UI_STATE_IGNORE_PRESSES; //Initial state

//Here we start the loop for the UI State Machine
while(1)
{
	switch(uiState)
	{
		case(UI_STATE_IGNORE_PRESSES):
		{
			//Ignore any presses until we receive a command from the control thread to go to UI_STATE_SHOW_MOVES
			//Will be changed by control with the function void UiOrderShowMoves(struct GameDataPacket *packetIn) which gets called when a valid
			//MQTT Package comes in!
			break;
		}

		case(UI_STATE_SHOW_MOVES):
		{
			//Set initial state variable that will be used on the UI_STATE_Handle_Buttons and need to be initialized once
			pressedKeys = 0; //Set number of keys pressed by player to 0.
			//keysToPress = 1; //Set as an example to 1. STUDENTS should change this to the number of key presses needed.
			memset(gamePacketOut.game,0xff, sizeof(gamePacketOut.game)); //Erase gamePacketOut to an initial state
			playIsDone = false; //Set play to false
			uint8_t presses = SeesawGetKeypadCount();
			if(presses >= BUTTON_PRESSES_MAX) presses = BUTTON_PRESSES_MAX;
			if(presses != 0) SeesawReadKeypad(buttons,presses); //Empty Seesaw buffer just in case it has latent presses on it!
			memset(buttons, 0, BUTTON_PRESSES_MAX);
			//Make this function show the moves of the gamePacketIn.
			//You can use a static delay to show each move but a quicker delay as the message gets longer might be more fun!
			//After you finish showing the move should go to state UI_STATE_HANDLE_BUTTONS
			
			for (int i =0; i < GAME_SIZE;i++)
			{
				//quit if it reaches the end of the move
				if (gamePacketIn.game[i]==0xff)
				{
					keysToPress = i+1;
					break;
				}
				uint8_t keyToShow = gamePacketIn.game[i];
				SeesawSetLed(keyToShow, red, green, blue); //Turn button 1 on
				SeesawOrderLedUpdate();
				//delay_ms(1000);
				vTaskDelay(2000);			
				SeesawSetLed(keyToShow, 0, 0, 0); //Turn button 1 on
				SeesawOrderLedUpdate();
			}
			
			uiState = UI_STATE_HANDLE_BUTTONS;

			break;
		}



		case(UI_STATE_HANDLE_BUTTONS):
		{
		//This state should accept (gamePacketIn length + 1) moves from the player (capped to maximum 19 + new move)
		//The moves by the player should be stored on "gamePacketOut". The keypresses that should count are when the player RELEASES the button.
		
		//In this example, we return after only one button press!
		
		uint8_t numPresses = SeesawGetKeypadCount();
		memset(buttons, 0, BUTTON_PRESSES_MAX);

		if(numPresses >= BUTTON_PRESSES_MAX )
		{
			numPresses = BUTTON_PRESSES_MAX;
		}
		if(numPresses != 0 && ERROR_NONE == SeesawReadKeypad(buttons, numPresses))
		{
			//Process Buttons
			for (int iter = 0; iter < numPresses; iter++)
			{
				uint8_t keynum = NEO_TRELLIS_SEESAW_KEY((buttons[iter] & 0xFD) >> 2);
				uint8_t actionButton = buttons[iter] & 0x03;
				if(actionButton == 0x03) 
				{
					SeesawSetLed(keynum, red, green, blue);
				}
				else
				{
					SeesawSetLed(keynum, 0, 0, 0);
					//Button released! Count this into the buttons pressed by user.
					gamePacketOut.game[pressedKeys] = keynum;
					pressedKeys++;
				}
			}
			SeesawOrderLedUpdate();
		}

		//Check if we are done!
		if(pressedKeys >= keysToPress || pressedKeys >= GAME_SIZE)
		{
			//Tell control gamePacketOut is ready to be send out AND go back to UI_STATE_IGNORE_PRESSES
			playIsDone = true;
			uiState = UI_STATE_IGNORE_PRESSES;
		}


		break;
		}



		default: //In case of unforseen error, it is always good to sent state machine to an initial state.
			uiState = UI_STATE_HANDLE_BUTTONS;
		break;
	}

	//After execution, you can put a thread to sleep for some time.
	vTaskDelay(50);
}



}




/******************************************************************************
* Functions
******************************************************************************/

/**************************************************************************//**
* @fn		void UiOrderShowMoves(struct GameDataPacket *packetIn)
* @brief	Read packet and light up leds
* @details 	display packet information received to keypad
* @param[in]	Parameters passed when task is initialized. In this case we can ignore them!
* @note         
*****************************************************************************/
void UiOrderShowMoves(struct GameDataPacket *packetIn){
	memcpy(&gamePacketIn, packetIn, sizeof(gamePacketIn));
	uiState = UI_STATE_SHOW_MOVES;
	playIsDone = false; //Set play to false
}

/**************************************************************************//**
* @fn		bool UiPlayIsDone(void)
* @brief	note other function Uiplay is done
* @details 	
* @return	playIsDone status
* @note         
*****************************************************************************/
bool UiPlayIsDone(void)
{
	return playIsDone;
}
/**************************************************************************//**
* @fn		struct GameDataPacket *UiGetGamePacketOut(void)
* @brief	Wrapper function to pass game data packet
* @details 	
* @return	Game data packet
* @note         
*****************************************************************************/
struct GameDataPacket *UiGetGamePacketOut(void)
{
	return &gamePacketOut;
}


/**************************************************************************//**
void UIChangeColors(uint8_t r, uint8_t g, uint8_t b);
* @brief	Changes the LED colors
* @param[out]    				
* @return		
* @note         
*****************************************************************************/
void UIChangeColors(uint8_t r, uint8_t g, uint8_t b)
{
	red = r;
	green = g;
	blue = b;
}