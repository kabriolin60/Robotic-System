/*
 * Init_Carte_IA.c
 *
 *  Created on: 28 avr. 2020
 *      Author: kabri
 */

#include <Init.h>

#include "0_Communication.h"
#include "0_Event_Group.h"
#include "0_RS485.h"
#include "0_Infos.h"
#include "0_ADC.h"

#include "1_Trame_Communication.h"
#include "2_Echange_Datas.h"
#include "2_Echange_Datas_Reception.h"


#include "0_Deplacements.h"

#include "Strategie.h"

#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
traceString MyChannel_Debug_pins;
traceString MyChannel_Debug_texte;
#endif


void Debug_Trace_Texte(const char * texte)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	vTracePrint(MyChannel_Debug_texte, texte);
#endif
}


void Set_Debug_Pin_0_High(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	vTracePrint(MyChannel_Debug_pins, "Debug_Pin 0 High");
#endif
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, true);
}


void Set_Debug_Pin_0_Low(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	vTracePrint(MyChannel_Debug_pins, "Debug_Pin 0 Low");
#endif
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, false);
}


void Set_Debug_Pin_1_High(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	vTracePrint(MyChannel_Debug_pins, "Debug_Pin 1 High");
#endif
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, true);
}


void Set_Debug_Pin_1_Low(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	vTracePrint(MyChannel_Debug_pins, "Debug_Pin 1 Low");
#endif
	Chip_GPIO_WritePortBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, false);
}



void Init_Carte_IA(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	MyChannel_Debug_pins = xTraceRegisterString("Debug_Pins");
	MyChannel_Debug_texte = xTraceRegisterString("Debug_Texte");
#endif

	/* Init Debug Output pins */
	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, true);

	/* Init External LEDs output pin*/
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_EXTERNE_RED_PORT, LED_EXTERNE_RED_BIT, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_EXTERNE_YELLOW_PORT, LED_EXTERNE_YELLOW_BIT, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED_EXTERNE_GREEN_PORT, LED_EXTERNE_GREEN_BIT, true);


	Set_Debug_Pin_0_Low();
	Set_Debug_Pin_1_Low();

	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();

	/* Init ADC pour la lecture de la strategie choisie */
	_0_ADC_Init();

	/* Init des infos des cartes de niveau 0 */
	_0_Infos_Init();

	/* Init des deplacement niveau 0 */
	_0_Deplacement_Init();

	/* Init de la communication Niveau 1 */
	_1_Communication_Init();

	/* Init de la communication Niveau 2 */
	_2_Communication_Init();

	/* Init de la communication de Reception Niveau 2 */
	_2_Communication_RX_Init();

	/* Envoie l'ID du Robot vers les cartes sur le réseau */
	Init_Send_Robot_ID();

	/* Init de la communication partie Strategie */
	_2_Comm_Strategie_Init();

	/* Creation de la Task de la strategie */
	xTaskCreate(_Strategie_Init_Strategie_2021, "Init_Strategie", 600, NULL, 1, NULL);
}


void Init_Send_Robot_ID()
{
	_2_Comm_Robot_ID(_Strategie_Get_Robot_ID(), RS485_port);
}
