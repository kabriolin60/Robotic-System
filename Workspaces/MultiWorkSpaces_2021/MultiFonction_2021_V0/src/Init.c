/*
 * Init_Carte_MultiFonction.c
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#include <Init.h>
#include "cdc_usb_main.h"

#include "0_Codeurs.h"
#include "0_Motors.h"
#include "0_Servos.h"
#include "0_AX12.h"
#include "0_ADC.h"

#include "0_Communication.h"
#include "0_Event_Group.h"

#include "1_Odometrie.h"
#include "1_Trame_Communication.h"

#include "1_Asservissement.h"
#include "2_Asservissement.h"

#include "2_Echange_Datas.h"
#include "2_Echange_Datas_Reception.h"

#include "3_Asservissement.h"

#include "1_Servos.h"
#include "1_AX12.h"

#include "1_Update_Values.h"

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



void Init_Carte_MultiFonctions(void)
{
#if(config_debug_Trace_ISR_AND_Buffer_Level == 1)
	MyChannel_Debug_pins = xTraceRegisterString("Debug_Pins");
	MyChannel_Debug_texte = xTraceRegisterString("Debug_Texte");
#endif

	/* Init Debug Output pins */
	Chip_IOCON_PinMux(LPC_IOCON, DEBUG_0_PORT, DEBUG_0_BIT, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_IOCON_PinMux(LPC_IOCON, DEBUG_1_PORT, DEBUG_1_BIT, IOCON_MODE_INACT, IOCON_FUNC0);

	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_0_PORT, DEBUG_0_BIT, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, DEBUG_1_PORT, DEBUG_1_BIT, true);

	Set_Debug_Pin_0_Low();
	Set_Debug_Pin_1_Low();

	/* Init Codeurs Input / Interrupts */
	_0_Codeurs_Init();

	/* Init Timer des Moteurs */
	_0_Motors_Init();

	/* Init des sorties GPIO de direction des moteurs */
	_0_GPIO_Moteurs_Init();

	/* Init des sorties Servos Niveau 0*/
	_0_Servo_Init();

	/* Init des sorties Servos Niveau 1*/
	_1_Servos_Init();

	/* Init du module ADC niveau 0 */
	_0_ADC_Init();

	/* Init des AX 12 Niveau 0 */
	_0_Init_AX12();

	/* Init des AX 12 Niveau 1 */
	_1_Init_AX12();

	/* Init du Groupe d'event de synchronisation */
	_0_Communication_Init_Event_Group();

	/* Init de la communication Niveau 0 */
	_0_Communication_Init();

	/* Init de la communication Niveau 1 */
	_1_Communication_Init();

	/* Init de la communication Niveau 2 */
	_2_Communication_Init();

	/* Init de la communication de Reception Niveau 2 */
	_2_Communication_RX_Init();

	/* Init de l'Odometrie */
	_1_Odometrie_Init();

	/* Init de l'asserv niveau 1 */
	_1_Asserv_Init_Asserv();

	/* Init de l'asserv de niveau 2 */
	_2_Deplacement_Init();

	/* Init de l'asserv de niveau 3 */
	_3_Deplacement_Init();


	/* Init de la tache de mise à jour des infos */
	_1_Update_Init();

	//Quand l'init est terminée, envoie un message d'identification
	//_2_Comm_Send_PONG(RS485_port);
}


void vConfigureTimerForRunTimeStats( void )
{
	const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* Power up and feed the timer with a clock. */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Reset Timer 0 */
	LPC_TIMER0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIMER0->TCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
     but not too fast so as to overflow all the time. */
	LPC_TIMER0->PR =  ( configCPU_CLOCK_HZ / 50000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIMER0->TCR = TCR_COUNT_ENABLE;
}
