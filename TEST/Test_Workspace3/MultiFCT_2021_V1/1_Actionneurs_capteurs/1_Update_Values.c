/*
 * 1_Update_Values.c
 *
 *  Created on: 4 mai 2020
 *      Author: kabri
 */

#include "1_Update_Values.h"
#include "Configuration.h"

#include "0_Servos.h"
#include "0_AX12.h"
#include "0_ADC.h"
#include "0_Motors.h"

//#include "1_Odometrie.h"
#include "1_Servos.h"
#include "1_AX12.h"

#include "_2_Comm_Echange_Datas.h"
#include "_2_Comm_Receive_Data.h"

static TO_AHBS_RAM3 struct Com_Reponse_Info Updated_Infos;
static float tension_Batterie;

/**************************************************
Fonction d'init de la tache de mise à jour permanante des infos sur la carte
Input: None
Output: None
 **************************************************/
void _1_Update_Init(void)
{
	(void)memset(&Updated_Infos, 0, sizeof(struct Com_Reponse_Info));

	//Tache de mise à jour des infos de la carte
	xTaskCreate(_1_Update_Task, (char *) "_1_Update_Task", 150, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
}


/**************************************************
Tache de mise à jour permanante des infos sur la carte
Input: pvParameter
Output: None
 **************************************************/
struct st_POSITION_ROBOT pos_robot;
struct st_Tableau_Servo pos_servos;

void _1_Update_Task(void * pvParameter)
{
	Task_Delay(1);

	Init_Timing_Tache;

	while(1)
	{
		Task_Delay_Until(PERIODE_UPDATE);


		//Id du Robot
		Updated_Infos.Numero_Robot = _2_Comm_Get_Robot_ID(); //0 = PR, 1 = GR

		//Numero de la carte
		if(static_IS_LPCXPRESSO_BOARD == 1)
		{
			Updated_Infos.Numero_Carte = 1;
		}else{
			Updated_Infos.Numero_Carte = ADRESSE_CARTE;
		}


		//Info sur l'alim
		uint8_t Etat_Alim; //0= motor power; 1 = Simulation; 2 = servos power; 3 = ax12 power; 4 = AUX 1 power; 5 = Aux 2 power					 //1 octet

		Etat_Alim = _0_Get_Motor_Power(); 			//Motor power
		Etat_Alim |= _1_Omodetrie_Get_Simulation() << 1; 	//Simulation
		Etat_Alim |= Chip_GPIO_ReadPortBit(LPC_GPIO, SERVO_ON_OFF_PORT, SERVO_ON_OFF_BIT) << 2;	//Servo Power
		Etat_Alim |= Chip_GPIO_ReadPortBit(LPC_GPIO, AX_12_ON_OFF_PORT, AX_12_ON_OFF_BIT) << 3;	//AX12 power
		Etat_Alim |= 0 << 4;	//Aux 1
		Etat_Alim |= 0 << 5;	//Aux 2

		Updated_Infos.Etat_Alim = Etat_Alim;


		//Info sur les contacteurs et FDC
		uint8_t Etat_Contacteurs; //0= FDC 0; 1 = FDC 1; 2 = CTC 0; 3 = CTC 1; 4 = CTC 2; 5 = CTC 3					 //1 octet

		Etat_Contacteurs = Chip_GPIO_ReadPortBit(LPC_GPIO, FDC_0_PORT, FDC_0_BIT);
		Etat_Contacteurs |= Chip_GPIO_ReadPortBit(LPC_GPIO, FDC_1_PORT, FDC_1_BIT) << 1;
		Etat_Contacteurs |= Chip_GPIO_ReadPortBit(LPC_GPIO, CTC_0_PORT, CTC_0_BIT) << 2;
		Etat_Contacteurs |= Chip_GPIO_ReadPortBit(LPC_GPIO, CTC_1_PORT, CTC_1_BIT) << 3;
		Etat_Contacteurs |= Chip_GPIO_ReadPortBit(LPC_GPIO, CTC_2_PORT, CTC_2_BIT) << 4;
		Etat_Contacteurs |= Chip_GPIO_ReadPortBit(LPC_GPIO, CTC_3_PORT, CTC_3_BIT) << 5;

		Updated_Infos.Etat_Contacteurs = Etat_Contacteurs;

		//Info sur les IO
		uint8_t Etat_IO;          //0= IO 0; 1 = IO 1; 2 = IO 2; 3 = IO 3
		Etat_IO = Chip_GPIO_ReadPortBit(LPC_GPIO, IO_0_PORT, IO_0_BIT);
		Etat_IO |= Chip_GPIO_ReadPortBit(LPC_GPIO, IO_1_PORT, IO_1_BIT) << 1;
		Etat_IO |= Chip_GPIO_ReadPortBit(LPC_GPIO, IO_2_PORT, IO_2_BIT) << 2;
		Etat_IO |= Chip_GPIO_ReadPortBit(LPC_GPIO, IO_3_PORT, IO_3_BIT) << 3;

		Updated_Infos.Etat_IO = Etat_IO;

		//Mise à jour de la position du Robot
		//pos_robot = _1_Odometrie_GetRobot_Position();

		Updated_Infos.PositionRobot.Position_X = (short)(pos_robot.Position_X * 10);
		Updated_Infos.PositionRobot.Position_Y = (short)(pos_robot.Position_Y * 10);
		Updated_Infos.PositionRobot.Angle = (short)(pos_robot.Angle_Deg * 100);


		//Mise à jour de la position des servos
		pos_servos = _1_Servo_GetServos_Position();
		for(int i = 0; i < NB_SERVO; i++)
			Updated_Infos.Position_Servos.Position[i] = pos_servos.Servo[i].Position;


		//Mise à jour de la position des AX12
		struct st_AX_12 Loc_AX_12;
		for(int i = 0; i < NB_AX_12; i++)
		{
			if(AX_12_READ_VALUES(i, &Loc_AX_12))
			{
				Updated_Infos.Position_AX12.Position[i] = Loc_AX_12.Position;
				Updated_Infos.Position_AX12.Torque[i] = Loc_AX_12.Torque;
			}
		}

		//Lecture et MaJ des mesures analogiques
		for(int i = 0; i < NB_MES_ANA; i++)
		{
			Updated_Infos.Mesures_Analogiques.Mesure[i] = _0_ADC_Read_Channel(i);
		}

		tension_Batterie = _0_ADC_Read_Channel(6);
		tension_Batterie *= 0.00609F;
		Updated_Infos.Tension_Batterie = tension_Batterie * 100;
	}
}


/**************************************************
Lecture de la tension de batterie d'alimentation de la carte
Input: None
Output: Tension de batterie
 **************************************************/
float _1_Update_GetTensionBatterie(void)
{
	return tension_Batterie;
}


/**************************************************
Get le pointeur vers la structure des infos à renvoyer
Input: None
Output: None
 **************************************************/
struct Com_Reponse_Info *Get_ptr_Reponse_info(void)
{
	return &Updated_Infos;
}
