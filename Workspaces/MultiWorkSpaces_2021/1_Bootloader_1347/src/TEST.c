/*
 * TEST.c
 *
 *  Created on: 28 déc. 2020
 *      Author: kabri
 */
#include "Test.h"


void TEST_Send_Message(void * pvParameters)
{
	Task_Delay(3000);
	Init_Timing_Tache;

	for(;;)
	{
		Task_Delay(3000);

		_2_Comm_Send_PONG(RS485_port);
		_0_Communication_Give_Sending_Clearance();
	}
}
