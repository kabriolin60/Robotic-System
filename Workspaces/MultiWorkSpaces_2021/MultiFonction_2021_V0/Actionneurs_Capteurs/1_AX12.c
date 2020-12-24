/*
 * 1_AX12.c
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "board.h"
#include "chip.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"

#include "0_Event_Group.h"
#include "0_AX12.h"
#include "1_AX12.h"


struct st_AX_12_Trame AX_12_TRAME;

QueueHandle_t _1_xQueue_AX12_Receive; 				//Queue Recevant les messages des canaux de communication

extern traceString MyChannel_RX_AX12;

extern RINGBUFF_T rxring_AX12;

/**************************************************
Fonction d'init des AX12 de niveau 1
Input: None
Output: None
 **************************************************/
void _1_Init_AX12(void)
{
	//Création de la Queue de reception pour les datas
	_1_xQueue_AX12_Receive = xQueueCreate( 2, sizeof( struct st_AX_12_Trame )); //Queue contenant les messages reçus
	vQueueAddToRegistry( _1_xQueue_AX12_Receive, "_1_xQue_AX12_Receive");


	//Tache de décodage des donnees recues des AX12
	xTaskCreate(_1_AX12_Recomposition_Rx, (char *) "1_AX12_Rx", 100, _1_xQueue_AX12_Receive, (tskIDLE_PRIORITY + 1UL), (xTaskHandle *) NULL);


	//Tache d'interpretation des messages reçus par les AX12
	xTaskCreate(_1_AX12_Interpretation_RX, (char *) "1_Interp_Rx_AX12", 100, _1_xQueue_AX12_Receive, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
}


/*****************************************************************************
 ** Function name:		_1_AX12_Interpretation_RX
 **
 ** Descriptions:		Tache d'interpretation des messages reçus depuis les AX-12
 **
 ** parameters:			Pointeur vers la Queue qui doit recevoir le message termine
 ** Returned value:		None
 **
 *****************************************************************************/
static TO_AHBS_RAM3 struct st_AX_12_Trame received_trame_AX12;
void _1_AX12_Interpretation_RX(void *pvParameters)
{
	Task_Delay(5);

	while (1)
	{
		//Reception d'un message depuis une Queue
		if(xQueueReceive(pvParameters, &received_trame_AX12, portMAX_DELAY))
		{
			if(received_trame_AX12.Length == 2)
			{
				//ACK or !ACK
				if(received_trame_AX12.Data[0] == 0)
				{
					//Pas d'erreur = ACK
					xEventGroupSetBits(_0_Comm_EventGroup,    /* The event group being updated. */
							eGROUP_SYNCH_AX12_ACK__OK);		 /* The bits being set. */
				}else
				{
					//Erreur = NOT ACK
					xEventGroupSetBits(_0_Comm_EventGroup,    /* The event group being updated. */
							eGROUP_SYNCH_AX12_ERROR);		 /* The bits being set. */
				}
			}else
			{
				AX_12_TRAME = received_trame_AX12;
			}

		}
	}
}


/*****************************************************************************
 ** Function name:		_1_AX12_Recomposition_Rx
 **
 ** Descriptions:		Tache de recomposition des messages reçus depuis les AX-12
 **
 ** parameters:			Pointeur vers la Queue qui doit recevoir le message termine
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_AX12_Recomposition_Rx(void *pvParameters)
{
	Task_Delay(5);

	if(pvParameters == NULL)
	{
		Task_Delete_Current;
	}

	while (1)
	{
		//Attente de l'info qu'une data est dispo dans un buffer
		//Sans effacer le bit de Flag
		(void)xEventGroupWaitBits(_0_Comm_EventGroup,   /* The event group being tested. */
				eGROUP_SYNCH_AX12_Rx_Data_Avail, /* The bits within the event group to wait for. */
				pdTRUE,         /* Clear bits before returning. */
				pdFALSE,        /* Wait for ALL bits to be set */
				portMAX_DELAY );/* Wait a maximum of xTicksToWait for either bit to be set. */

		vTracePrintF(MyChannel_RX_AX12, "AX-12_Before Read = %d", RingBuffer_Count(&rxring_AX12));


		//Temps qu'il y a des datas à lire dans le buffer en question
		//Creation de la trame à partir des data reçues
		while(RingBuffer_Count(&rxring_AX12))
		{
			Rx_Ax_12_Trame(pvParameters);
		}
	}
}


/*****************************************************************************
 ** Function name:		_1_AX12_MOVE_WITH_TIME
 **
 ** Descriptions:		Fonction de deplacement d'un AX12 en un temps donné avec mise à jour du torque
 **
 ** parameters:			Id de l'AX12 (local ID)
 						destination
						torque
						temps de deplacement (ms)
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_AX12_MOVE_WITH_TIME(byte ID, unsigned short desti, unsigned short torque, unsigned short time)
{	
	//Convert local ID to AX-12 bus ID
	//LocID @6 = bus @1
	ID -= 6;
	
	//Calcule la nouvelle speed à appliquer a l'ax12
	//Get it's current position
	unsigned short current_pos;
	current_pos = AX_12_READ_Position(ID);
	
	//error in steps
	float error_pos = (float)(desti - current_pos);
	//error in °
	error_pos *= 0.29F;
	
	float new_speed = error_pos / time;
	new_speed /= 1000; //Vitesse de deplacement en °/sec
	new_speed /= 0.666F; //Vitesse de deplacement en pas/sec (issu du 1 step = 0.111 tr/min)
	
	AX_12_SET_POSITION_SPEED_TORQUE(ID, desti, new_speed, torque);
}




/*****************************************************************************
 ** Function name:		_1_AX12_WAIT_FOR_ACK
 **
 ** Descriptions:		Fonction d'attente de reception d'un ACK par les AX-12
 **
 ** parameters:			Pointeur vers la Queue qui doit recevoir le message termine
 ** Returned value:		None
 **
 *****************************************************************************/
void _1_AX12_WAIT_FOR_ACK(struct st_AX_12_Trame *trame, bool wait)
{
	//Avant d'envoyer un message, commence par effacer les potentiels Flags
	(void)xEventGroupClearBits(_0_Comm_EventGroup,   /* The event group being tested. */
			eGROUP_SYNCH_AX12_ACK__OK | eGROUP_SYNCH_AX12_ERROR); /* The bits within the event group to wait for. */

	//Ensuite envoi le message
	Tx_Ax_12_Trame(trame);

	if(wait)
	{
		byte boucle = 0;
		//Si l'attente est demandée

		//Attente de reception de l'ACK
		while(!xEventGroupWaitBits(_0_Comm_EventGroup,   /* The event group being tested. */
				eGROUP_SYNCH_AX12_ACK__OK, /* The bits within the event group to wait for. */
				pdTRUE,         /* Clear bits before returning. */
				pdFALSE,        /* Wait for ALL bits to be set */
				ms_to_tick(10) ))/* Wait a maximum of xTicksToWait for either bit to be set. */
		{
			//Force le ré-envoi le message
			Tx_Ax_12_Trame(trame);
			boucle++;
			if(boucle > 3)
				return;
		}

	}
}


/**************************************************
Fonction de pilotage en position/torque/speed d'un AX-12
Input: ID de l'ax12
		Consigne en position
		Consigne en vitesse
		consigne en torque
Output: None
 **************************************************/
__attribute__((optimize("O0"))) void AX_12_SET_POSITION_SPEED_TORQUE(unsigned char ID, unsigned short position, unsigned short speed, unsigned short torque)
{
	struct st_AX_12_Trame AX_12;
	unsigned int Pos_H = (position >> 8);
	unsigned int Pos_L = (position & 0xFF);
	
	unsigned int Speed_H = (speed >> 8);
	unsigned int Speed_L = (speed & 0xFF);
	
	unsigned int Torque_H = (torque >> 8);
	unsigned int Torque_L = (torque & 0xFF);

	AX_12.ID = ID;
	AX_12.Length = 9;

	AX_12.Instruction = AX_12_WRITE_DATA;

	AX_12.Data[0] = AX_12_Goal_Position_L;
	AX_12.Data[1] = Pos_L;
	AX_12.Data[2] = Pos_H;
	AX_12.Data[3] = Speed_L;
	AX_12.Data[4] = Speed_H;
	AX_12.Data[5] = Torque_L;
	AX_12.Data[6] = Torque_H;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}



/**************************************************
Fonction de pilotage en position d'un AX-12
Input: ID de l'ax12
		Consigne en position
		Attente pour synchro ou pas
Output: None
 **************************************************/
__attribute__((optimize("O0"))) void AX_12_SET_POSITION(unsigned char ID, unsigned int position, unsigned char Attente_Synchro)
{
	struct st_AX_12_Trame AX_12;
	unsigned int Pos_H = (position >> 8);
	unsigned int Pos_L = (position & 0xFF);

	AX_12.ID = ID;
	AX_12.Length = 5;
	if(Attente_Synchro)
	{
		AX_12.Instruction = AX_12_REG_WRITE;
	}else
	{
		AX_12.Instruction = AX_12_WRITE_DATA;
	}
	AX_12.Data[0] = AX_12_Goal_Position_L;
	AX_12.Data[1] = Pos_L;
	AX_12.Data[2] = Pos_H;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


/**************************************************
Fonction de lecture en position d'un AX-12
Input: ID de l'ax12
Output: Position
 **************************************************/
__attribute__((optimize("O0"))) unsigned short AX_12_READ_Position(unsigned char ID)
{
	struct st_AX_12_Trame AX_12;
	unsigned short Position = 0;

	AX_12.ID = ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_READ_DATA;
	AX_12.Data[0] = AX_12_Present_Position_L;
	AX_12.Data[1] = 0x02;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);

	Position = AX_12_TRAME.Data[1];
	Position = Position << 8;
	Position += AX_12_TRAME.Data[0];

	return Position;
}

__attribute__((optimize("O0"))) void AX_12_DO_RESET(unsigned char ID)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 2;
	AX_12.Instruction = AX_12_RESET;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_RETURN_DELAY(unsigned char ID, unsigned char Value)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 2;
	AX_12.Instruction = AX_12_WRITE_DATA;

	AX_12.Data[0] = AX_12_Return_Delay_Time;
	AX_12.Data[1] = Value;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_TORQUE_ENABLE(unsigned char ID, unsigned char Attente_Synchro)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 4;
	if(Attente_Synchro)
	{
		AX_12.Instruction = AX_12_REG_WRITE;
	}else
	{
		AX_12.Instruction = AX_12_WRITE_DATA;
	}
	AX_12.Data[0] = AX_12_Torque_Enable;
	AX_12.Data[1] = 0x01;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}

__attribute__((optimize("O0"))) void AX_12_TORQUE_DISABLE(unsigned char ID, unsigned char Attente_Synchro)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 4;
	if(Attente_Synchro)
	{
		AX_12.Instruction = AX_12_REG_WRITE;
	}else
	{
		AX_12.Instruction = AX_12_WRITE_DATA;
	}
	AX_12.Data[0] = AX_12_Torque_Enable;
	AX_12.Data[1] = 0x00;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}

__attribute__((optimize("O0"))) void AX_12_SET_TORQUE(unsigned char ID, unsigned int torque, unsigned char Attente_Synchro)
{
	struct st_AX_12_Trame AX_12;
	unsigned int Torque_H = (torque >> 8);
	unsigned int Torque_L = (torque & 0xFF);

	AX_12.ID = ID;
	AX_12.Length = 5;
	if(Attente_Synchro)
	{
		AX_12.Instruction = AX_12_REG_WRITE;
	}else
	{
		AX_12.Instruction = AX_12_WRITE_DATA;
	}
	AX_12.Data[0] = AX_12_Torque_Limite_L;
	AX_12.Data[1] = Torque_L;
	AX_12.Data[2] = Torque_H;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_CHANGE_BAUDRATE(unsigned char ID, long Baudrate)
{
	struct st_AX_12_Trame AX_12;
	long value = 0;

	value = (2000000 / Baudrate) - 1;

	AX_12.ID = ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_WRITE_DATA;
	AX_12.Data[0] = AX_12_Baud_Rate;
	AX_12.Data[1] = (unsigned char)value;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_CHANGE_ID(unsigned char Old_ID, unsigned char New_ID)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = Old_ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_WRITE_DATA;
	AX_12.Data[0] = AX_12_ID;
	AX_12.Data[1] = New_ID;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_SYNCHRO_ACTION(void)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = 0xFE;
	AX_12.Length = 2;
	AX_12.Instruction = AX_12_ACTION;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	//Pas de réponse car ID = 0xFE
	_1_AX12_WAIT_FOR_ACK(&AX_12, false);
}

__attribute__((optimize("O0"))) void AX_12_SET_SPEED(unsigned char ID, unsigned int speed, unsigned char Attente_Synchro)
{
	struct st_AX_12_Trame AX_12;
	unsigned int Speed_H = (speed >> 8);
	unsigned int Speed_L = (speed & 0xFF);

	AX_12.ID = ID;
	AX_12.Length = 5;
	if(Attente_Synchro)
	{
		AX_12.Instruction = AX_12_REG_WRITE;
	}else
	{
		AX_12.Instruction = AX_12_WRITE_DATA;
	}
	AX_12.Data[0] = AX_12_Moving_Speed_L;
	AX_12.Data[1] = Speed_L;
	AX_12.Data[2] = Speed_H;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}



/**************************************************
Fonction de lectures des infos (couple/vitesse_position)
Input: None
Output: None
 **************************************************/
__attribute__((optimize("O0"))) int AX_12_TWO_CHAR_TO_INT(unsigned char Input1, unsigned char Input2)
{
	int Temp = Input2;
	Temp <<= 8;
	Temp += Input1;
	return Temp;
}

__attribute__((optimize("O0"))) unsigned char AX_12_READ_VALUES(unsigned char ID, struct st_AX_12 *Loc_AX_12)
{
	struct st_AX_12_Trame AX_12;
	unsigned char Adresse = 0;

	AX_12.ID = ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_READ_DATA;
	AX_12.Data[0] = AX_12_Present_Position_L;
	AX_12.Data[1] = 0x08;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);

	if(AX_12_TRAME.Length == 0)
		return NOK;

	//MAJ de la structure avec les données reçues
	Loc_AX_12->Position = AX_12_TWO_CHAR_TO_INT(AX_12_TRAME.Data[Adresse], AX_12_TRAME.Data[Adresse+1]);

	Loc_AX_12->Speed = AX_12_TWO_CHAR_TO_INT(AX_12_TRAME.Data[Adresse+2], AX_12_TRAME.Data[Adresse+3]);
	if(Loc_AX_12->Speed > 1023)
	{
		Loc_AX_12->Speed -= 1023;
		Loc_AX_12->Speed = -Loc_AX_12->Speed;
	}

	Loc_AX_12->Torque = AX_12_TWO_CHAR_TO_INT(AX_12_TRAME.Data[Adresse+4], AX_12_TRAME.Data[Adresse+5]);
	if(Loc_AX_12->Torque > 1023)
	{
		Loc_AX_12->Torque -= 1023;
		Loc_AX_12->Torque = -Loc_AX_12->Torque;
	}

	Loc_AX_12->Voltage = AX_12_TRAME.Data[Adresse+6];
	Loc_AX_12->Temperature = AX_12_TRAME.Data[Adresse+7];

	return OK;
}


/*************************************************/



/**************************************************
Fonction de pilotage de la LED des AX-12
Input: None
Output: None
 **************************************************/
__attribute__((optimize("O0"))) void AX_12_LED_ON(unsigned char ID)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_WRITE_DATA;
	AX_12.Data[0] = AX_12_LED;
	AX_12.Data[1] = 0x01;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, true);
}


__attribute__((optimize("O0"))) void AX_12_LED_OFF(unsigned char ID)
{
	struct st_AX_12_Trame AX_12;

	AX_12.ID = ID;
	AX_12.Length = 4;
	AX_12.Instruction = AX_12_WRITE_DATA;
	AX_12.Data[0] = AX_12_LED;
	AX_12.Data[1] = 0x00;

	(void)memset(&AX_12_TRAME, 0, sizeof(struct st_AX_12_Trame));

	_1_AX12_WAIT_FOR_ACK(&AX_12, false);
}






