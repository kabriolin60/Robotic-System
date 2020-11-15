/*
 * Test.c
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#include "Test.h"



__attribute__((optimize("O0"))) void TEST_init_parametres(void)
{
	struct st_ROBOT_PARAMETRES newparameters;
	//Odométrie du petit Robot 2015
	newparameters.COEF_D = 29.05F;	//29.2F
	newparameters.COEF_ROT = 5489.5F;	//5493.0F
	newparameters.COEF_CORRECTION_DIAMETRES = -0.0027F;	//-0.0018
	newparameters.Coef_Multiplicateur_Periode_asserv = 9;	//(9+1)*1 = 10ms
	newparameters.SIMULATION = 0;
	_1_Odometrie_Set_Parameters(&newparameters);

	//Position initiale du Robot
	struct st_POSITION_ROBOT newposition;
	newposition.Position_X = 250;
	newposition.Position_Y = 1000;
	newposition.Angle_rad = 0;
	newposition.orient_init = 0;
	newposition.Angle_Deg = 0;
	_1_Odometrie_SetRobot_Position(&newposition);


	if(newparameters.SIMULATION == 1)
	{
		//Init PID en vitesse
		struct st_pid_filter* ptr_PID_Vitesse_Position = _1_Asserv_GetPtr_PID_Vit_Pos();
		struct st_pid_filter* ptr_PID_Vitesse_Rotation = _1_Asserv_GetPtr_PID_Vit_Rot();

		ptr_PID_Vitesse_Position->Enable = 1;
		ptr_PID_Vitesse_Position->gain_P = 0.84F;	//1.0F
		ptr_PID_Vitesse_Position->gain_I = 0.005F;	//0.011F
		ptr_PID_Vitesse_Position->gain_D = 0.00125F;	//0.001F
		ptr_PID_Vitesse_Position->Max_Erreur_Cumul = 0;	//1
		ptr_PID_Vitesse_Position->commande_max = 50;
		ptr_PID_Vitesse_Position->commande_min = -50;
		ptr_PID_Vitesse_Position->Sommation_Sortie = 1; //1

		ptr_PID_Vitesse_Rotation->Enable = 1;
		ptr_PID_Vitesse_Rotation->gain_P = 2.0F;	//0.9F
		ptr_PID_Vitesse_Rotation->gain_I = 0.0F;	//0.005F
		ptr_PID_Vitesse_Rotation->gain_D = 0.0F;	//0.00125F
		ptr_PID_Vitesse_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_Rotation->commande_max = 50;
		ptr_PID_Vitesse_Rotation->commande_min = -50;
		ptr_PID_Vitesse_Rotation->Sommation_Sortie = 1;



		//Init des PID en position rotation
		struct st_pid_filter* ptr_PID_Position = _2_Asserv_GetPtr_PID_Pos();
		struct st_pid_filter* ptr_PID_Rotation = _2_Asserv_GetPtr_PID_Rot();

		ptr_PID_Position->Enable = 1;
		ptr_PID_Position->gain_P = 1.9F;	//0.5F
		ptr_PID_Position->gain_I = 0.0F;	//0.0F
		ptr_PID_Position->gain_D = 1.0F;	//0.5F
		ptr_PID_Position->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Position->commande_max = 10;
		ptr_PID_Position->commande_min = -10;
		ptr_PID_Position->Sommation_Sortie = 0;

		ptr_PID_Rotation->Enable = 1;
		ptr_PID_Rotation->gain_P = 2.0F;	//1.0F
		ptr_PID_Rotation->gain_I = 0.0F;	//0.0F
		ptr_PID_Rotation->gain_D = 0.5F;	//0.5F
		ptr_PID_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Rotation->commande_max = 10;
		ptr_PID_Rotation->commande_min = -10;
		ptr_PID_Rotation->Sommation_Sortie = 0;
	}else
	{
		//Réglages PID pour deplacements reels

		//Init PID en vitesse
		struct st_pid_filter* ptr_PID_Vitesse_Position = _1_Asserv_GetPtr_PID_Vit_Pos();
		struct st_pid_filter* ptr_PID_Vitesse_Rotation = _1_Asserv_GetPtr_PID_Vit_Rot();

		ptr_PID_Vitesse_Position->Enable = 1;
		ptr_PID_Vitesse_Position->gain_P = 1.0F;	//0.05F
		ptr_PID_Vitesse_Position->gain_I = 0.05F;	//0.0F
		ptr_PID_Vitesse_Position->gain_D = 0.0125F;	//0.4F
		ptr_PID_Vitesse_Position->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_Position->commande_max = 30;
		ptr_PID_Vitesse_Position->commande_min = -30;
		ptr_PID_Vitesse_Position->Sommation_Sortie = 0; //1

		ptr_PID_Vitesse_Rotation->Enable = 1;
		ptr_PID_Vitesse_Rotation->gain_P = 0.6F;	//0.05F
		ptr_PID_Vitesse_Rotation->gain_I = 0.05F;	//0.0F
		ptr_PID_Vitesse_Rotation->gain_D = 0.025F;	//0.4F
		ptr_PID_Vitesse_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_Rotation->commande_max = 30;
		ptr_PID_Vitesse_Rotation->commande_min = -30;
		ptr_PID_Vitesse_Rotation->Sommation_Sortie = 1;


		//Init des PID en position rotation
		struct st_pid_filter* ptr_PID_Position = _2_Asserv_GetPtr_PID_Pos();
		struct st_pid_filter* ptr_PID_Rotation = _2_Asserv_GetPtr_PID_Rot();

		ptr_PID_Position->Enable = 1;
		ptr_PID_Position->gain_P = 0.03F;	//0.04F
		ptr_PID_Position->gain_I = 0.0F;	//0.0F
		ptr_PID_Position->gain_D = 0.9F;	//0.9F
		ptr_PID_Position->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Position->commande_max = 10;
		ptr_PID_Position->commande_min = -10;
		ptr_PID_Position->Sommation_Sortie = 0;

		ptr_PID_Rotation->Enable = 1;
		ptr_PID_Rotation->gain_P = 0.01F;	//0.01F
		ptr_PID_Rotation->gain_I = 0.0F;	//0.0F
		ptr_PID_Rotation->gain_D = 0.9F;	//0.0F
		ptr_PID_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Rotation->commande_max = 10;
		ptr_PID_Rotation->commande_min = -10;
		ptr_PID_Rotation->Sommation_Sortie = 0;
	}


	/*
	//Réglages des vitesses et acceleration
	_2_Asservissement_Set_Distance_Speed_Accel(20, 1, 1);
	_2_Asservissement_Set_Rotation_Speed_Accel(20, 2, 2);

	//Réglages des parametres d'asservissement
	struct st_Parametre_Deplacement parameters;
	parameters.Angle_Avant_Debut_Avance = 0.1F;
	parameters.Distance_Detection_Fin_Trajectoire = 3.0F;
	parameters.Angle_Detection_Fin_Trajectoire = 0.2F;
	_2_Asservissement_Set_Parameters(&parameters);


	//Ajoute des points de destination
	struct st_COORDONNEES destination;

	destination.Type_Arret = depla_AVEC_freinage;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 500;
	destination.Y = 1000;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 500;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 1000;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	/*
	destination.Type_Arret = depla_AVEC_freinage;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 1800;
	destination.Y = 1000;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 1000;
	destination.Y = 1500;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 2500;
	destination.Y = 1500;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Deplacement = xy_tour_av_arriere;
	destination.X = 250;
	destination.Y = 1000;
	_2_Deplacement_Ajout_Point(&destination);*/

	/*struct CubicSpline spline;
	spline.P0.X = 250;
	spline.P0.Y = 1000;

	spline.P1.X = 1200;
	spline.P1.Y = 1300;

	spline.M0.X = 4000;
	spline.M0.Y = 0;

	spline.M1.X = 4000;
	spline.M1.Y = 0;

	spline.Direction = 0;
	spline.Nombre_Points = 20;
	spline.Taille_Terrain.X = 3000;
	spline.Taille_Terrain.Y = 2000;

	CubicSpline_Process(&spline);/**/

	//Réglages des vitesses et acceleration
	_2_Asservissement_Set_Distance_Speed_Accel(20, 1, 1);
	_2_Asservissement_Set_Rotation_Speed_Accel(20, 2, 2);

	//xTaskCreate(TEST_Send_Board_Infos, (char *) "Infos", 80, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
	//xTaskCreate(TEST_Test_Deplacement, (char *) "Deplacements", 80, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);

	//xTaskCreate(Test_Task_Graphique, (char *) "Graph", 100, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);

	//xTaskCreate(TEST_PID_Tunning, (char *) "PID Tunning", 80, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);

	//xTaskCreate(TEST_AX12, (char *) "TEST AX12", 80, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);
	//xTaskCreate(Test_Calibration_Odometrie, (char *) "Odometrie Calib", 250, NULL, (tskIDLE_PRIORITY + 2UL), (xTaskHandle *) NULL);

}


void Test_Task_Graphique(void* pvParameter)
{
	Task_Delay(500);
	Init_Timing_Tache;

	struct st_Graph_Datas Datas_To_Plot;

	int channel = 0;

	for (;;)
	{
		Task_Delay_Until(5);
		channel = 0;


		/*Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _1_Asserv_GetPtr_PID_Vit_Pos()->Consigne;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _1_Asserv_GetPtr_PID_Vit_Pos()->Current_Value;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _1_Asserv_GetPtr_PID_Vit_Rot()->Consigne;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _1_Asserv_GetPtr_PID_Vit_Rot()->Current_Value;
		channel++;*/



		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _2_Asserv_GetPtr_PID_Pos()->Consigne;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _2_Asserv_GetPtr_PID_Pos()->Current_Value;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _2_Asserv_GetPtr_PID_Rot()->Consigne;
		channel++;

		Datas_To_Plot.Datas[channel].Channel = channel;
		Datas_To_Plot.Datas[channel].Data = _2_Asserv_GetPtr_PID_Rot()->Current_Value;
		channel++;/**/


		Datas_To_Plot.nb_datas_to_send = channel;
		_2_Comm_Send_Graph(&Datas_To_Plot, RS485_port);
	}
}


void TEST_PID_Tunning(void* pvParameter)
{
	Task_Delay(1000);

	//disable asserv level 2
	_2_Disable_Asserv();

/*
	//Init PID en vitesse
	struct st_pid_filter* ptr_PID_Vitesse_Position = _1_Asserv_GetPtr_PID_Vit_Pos();
	struct st_pid_filter* ptr_PID_Vitesse_Rotation = _1_Asserv_GetPtr_PID_Vit_Rot();

	ptr_PID_Vitesse_Position->Enable = 1;
	ptr_PID_Vitesse_Position->gain_P = 0.0F;	//1.0F
	ptr_PID_Vitesse_Position->gain_I = 0.0F;	//0.011F
	ptr_PID_Vitesse_Position->gain_D = 0.0F;	//0.001F
	ptr_PID_Vitesse_Position->Max_Erreur_Cumul = 0;	//1
	ptr_PID_Vitesse_Position->commande_max = 50;
	ptr_PID_Vitesse_Position->commande_min = -50;
	ptr_PID_Vitesse_Position->Sommation_Sortie = 1; //1

	ptr_PID_Vitesse_Rotation->Enable = 1;
	ptr_PID_Vitesse_Rotation->gain_P = 0.0F;	//0.9F
	ptr_PID_Vitesse_Rotation->gain_I = 0.0F;	//0.005F
	ptr_PID_Vitesse_Rotation->gain_D = 0.0F;	//0.00125F
	ptr_PID_Vitesse_Rotation->Max_Erreur_Cumul = 0;	//0
	ptr_PID_Vitesse_Rotation->commande_max = 50;
	ptr_PID_Vitesse_Rotation->commande_min = -50;
	ptr_PID_Vitesse_Rotation->Sommation_Sortie = 1;

	ptr_PID_Vitesse_Rotation->Consigne = 10;


	Task_Delete_Current;*/





	struct st_pid_filter* ptr_PID_Position = _2_Asserv_GetPtr_PID_Pos();
	struct st_pid_filter* ptr_PID_Rotation = _2_Asserv_GetPtr_PID_Rot();

	ptr_PID_Position->Enable = 1;
	ptr_PID_Position->gain_P = 0.0F;	//0.2F
	ptr_PID_Position->gain_I = 0.0F;	//0.0F
	ptr_PID_Position->gain_D = 0.0F;	//1.0F
	ptr_PID_Position->Max_Erreur_Cumul = 0;	//0
	ptr_PID_Position->commande_max = 10;
	ptr_PID_Position->commande_min = -10;
	ptr_PID_Position->Sommation_Sortie = 0;

	ptr_PID_Rotation->Enable = 1;
	ptr_PID_Rotation->gain_P = 0.0F;	//0.1F
	ptr_PID_Rotation->gain_I = 0.0F;	//0.0F
	ptr_PID_Rotation->gain_D = 0.0F;	//0.3F
	ptr_PID_Rotation->Max_Erreur_Cumul = 0;	//0
	ptr_PID_Rotation->commande_max = 10;
	ptr_PID_Rotation->commande_min = -10;
	ptr_PID_Rotation->Sommation_Sortie = 0;

	struct Codeurs_Values Valeurs_Codeurs;
	long deplacement_Droit, deplacement_Gauche;

	Init_Timing_Tache
	for(;;)
	{
		Task_Delay_Until(PERIODE_PID_DISTANCE_ANGLE);

		Valeurs_Codeurs = _0_Codeurs_Get_CodeursValues();
		deplacement_Droit = Valeurs_Codeurs.Codeur_Droit;
		deplacement_Gauche = Valeurs_Codeurs.Codeur_Gauche;

		_2_Asserv_GetPtr_PID_Pos()->Current_Value = (deplacement_Droit + deplacement_Gauche) >> 1;
		_2_Asserv_GetPtr_PID_Rot()->Current_Value = (deplacement_Droit - deplacement_Gauche);

		//Effectue le calcul des PID
		pid_do_filter2(_2_Asserv_GetPtr_PID_Pos());
		pid_do_filter2(_2_Asserv_GetPtr_PID_Rot());

		//La sortie de ces PIDs corresponds à des consignes en vitesses (distance et Rotation)

		//Met à jour les consignes des PID en vitesse position et rotation à partir des sorties des PID en position et rotation
		_2_Update_PID_Vitesse_Consignes(_2_Asserv_GetPtr_PID_Pos()->Commande, _2_Asserv_GetPtr_PID_Rot()->Commande);
	}

	Task_Delete_Current;
}
