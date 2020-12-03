/*
 * Old_TEST.c
 *
 *  Created on: 7 juin 2020
 *      Author: kabri
 */

#include "Test.h"



void Do_Odometrie_Calibration(void)
{

	//Réglages des vitesses et acceleration
	_2_Asservissement_Set_Distance_Speed_Accel(20, 1, 1);
	_2_Asservissement_Set_Rotation_Speed_Accel(20, 2, 2);

	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 30);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 30);

	//Recule sans asserv dist/angle
	_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv = Vitesse_Droite_Vitesse_Gauche_Indep;
	_1_Update_PID_Consigne_Vitesse_Roue_Droite(-5);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(-5);
	//Pendant 3 secondes
	Task_Delay(2000);

	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 0);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 0);

	_1_Update_PID_Consigne_Vitesse_Roue_Droite(0);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(0);

	Task_Delay(200);
	//Reset les codeurs
	_0_Codeurs_Reset();
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Pos(), 0);
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Rot(), 0);
	Task_Delay(200);


	struct st_POSITION_ROBOT newposition;
	//Position initiale du Robot contre la bordure
	newposition.Position_X = 75;
	newposition.Position_Y = 1000;
	newposition.Angle_rad = 0;
	newposition.orient_init = 0;
	newposition.Angle_Deg = 0;
	_1_Odometrie_SetRobot_Position(&newposition);

	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 30);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 30);

	_3_Deplacement_Distance_Brute(1000, true);
	_3_Deplacement_Rotation_Brute(PI, true);
	_3_Deplacement_Distance_Brute(920, true);
	_3_Deplacement_Rotation_Brute(-PI, true);


	//Recule sans asserv dist/angle
	_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv = Vitesse_Droite_Vitesse_Gauche_Indep;
	_1_Update_PID_Consigne_Vitesse_Roue_Droite(-5);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(-5);
	//Pendant 3 secondes
	Task_Delay(4000);

	_1_Update_PID_Consigne_Vitesse_Roue_Droite(0);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(0);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 0);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 0);

	Task_Delay(100);

	static char str[70];
	sprintf(str, "Mesure decalage Codeuses, difference = %ld sur: %ld pas\n",
			(long)(_1_Odometrie_GetRobot_Position().Angle_rad *_1_Odometrie_Get_Parameters()->COEF_ROT),
			(_0_Codeurs_Get_CodeursValues().Codeur_Droit + _0_Codeurs_Get_CodeursValues().Codeur_Gauche) >> 1);
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Test, RS485_port);

	sprintf(str, "Old coef = %f\n", _1_Odometrie_Get_Parameters()->COEF_CORRECTION_DIAMETRES);
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Test, RS485_port);

	float factor = (float)(_1_Odometrie_GetRobot_Position().Angle_rad *_1_Odometrie_Get_Parameters()->COEF_ROT) / (float)((_0_Codeurs_Get_CodeursValues().Codeur_Droit + _0_Codeurs_Get_CodeursValues().Codeur_Gauche) >> 1);
	float new_coef = _1_Odometrie_Get_Parameters()->COEF_CORRECTION_DIAMETRES - factor;

	sprintf(str, "New coef = %f\n", new_coef);
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Test, RS485_port);

	struct st_ROBOT_PARAMETRES* newparameters;
	newparameters = _1_Odometrie_Get_Parameters();
	newparameters->COEF_CORRECTION_DIAMETRES = new_coef;
	_1_Odometrie_Set_Parameters(newparameters);

	Task_Delay(500);
}


void Do_Rotation_Calibration(int nb_tours)
{
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 0);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 0);

	_1_Update_PID_Consigne_Vitesse_Roue_Droite(0);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(0);

	Task_Delay(200);
	//Reset les codeurs
	_0_Codeurs_Reset();
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Pos(), 0);
	PID_update_Current_Value(_2_Asserv_GetPtr_PID_Rot(), 0);
	Task_Delay(200);


	struct st_POSITION_ROBOT newposition;
	//Position initiale du Robot contre la bordure
	newposition.Position_X = 75;
	newposition.Position_Y = 1000;
	newposition.Angle_rad = 0;
	newposition.orient_init = 0;
	newposition.Angle_Deg = 0;
	_1_Odometrie_SetRobot_Position(&newposition);

	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 30);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 30);


	_3_Deplacement_Distance_Brute(150, true);
	_3_Deplacement_Rotation_Brute(nb_tours * 2*PI, true);

	//Recule sans asserv dist/angle
	_1_Odometrie_Get_Parameters()->_1_Odometrie_Type_Asserv = Vitesse_Droite_Vitesse_Gauche_Indep;
	_1_Update_PID_Consigne_Vitesse_Roue_Droite(-10);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(-10);
	//Pendant 3 secondes
	Task_Delay(3000);

	_1_Update_PID_Consigne_Vitesse_Roue_Droite(0);
	_1_Update_PID_Consigne_Vitesse_Roue_Gauche(0);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Droite(), 00);
	PID_set_Max_Min(_1_Get_prt_PID_Vit_Gauche(), 0);

	static char str[70];

	sprintf(str, "Old coef Rotation = %f\n", _1_Odometrie_Get_Parameters()->COEF_ROT);
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Test, RS485_port);

	float delta_angle = _1_Odometrie_GetRobot_Position().Angle_rad;
	float new_track = _1_Odometrie_Get_Parameters()->COEF_ROT * (1 + (delta_angle / (2 * PI * nb_tours)));

	sprintf(str, "New coef Rotation = %f\n", new_track);
	_2_Comm_Send_Log_Message(str, Color_Red, Channel_Debug_Test, RS485_port);

	struct st_ROBOT_PARAMETRES* newparameters;
	newparameters = _1_Odometrie_Get_Parameters();
	newparameters->COEF_ROT = new_track;
	_1_Odometrie_Set_Parameters(newparameters);

	Task_Delay(500);
}


void Test_Calibration_Odometrie(void * pvParameter)
{
	Task_Delay(2000);

	struct st_ROBOT_PARAMETRES newparameters;
	//Odométrie du petit Robot 2015
	newparameters.COEF_D = 29.05F;	//29.2F
	newparameters.COEF_ROT = 5489.5F;	//5493.0F
	newparameters.COEF_CORRECTION_DIAMETRES = 0.0F;	//-0.0027
	newparameters.Coef_Multiplicateur_Periode_asserv = 9;	//(9+1)*1 = 10ms
	newparameters.SIMULATION = 0;

	newparameters._1_Odometrie_Type_Asserv = Vitesse_D_G__Distance_Angle;



	/*
	 * TEST ONLY
	 */
	newparameters.COEF_D = 29.05F;	//29.2F
	newparameters.COEF_ROT = 5470.0F;	//5493.0F
	newparameters.COEF_CORRECTION_DIAMETRES = 0.02F;	//-0.0027
	/*
	 * END OF TEST ONLY
	 */


	_1_Odometrie_Set_Parameters(&newparameters);

	//Position initiale du Robot
	struct st_POSITION_ROBOT newposition;
	newposition.Position_X = 250;
	newposition.Position_Y = 1000;
	newposition.Angle_rad = 0;
	newposition.orient_init = 0;
	newposition.Angle_Deg = 0;
	_1_Odometrie_SetRobot_Position(&newposition);


	//Init PID en vitesse
	struct st_pid_filter* ptr_PID_Vitesse_D = _1_Get_prt_PID_Vit_Droite();
	struct st_pid_filter* ptr_PID_Vitesse_G = _1_Get_prt_PID_Vit_Gauche();

	if(newparameters.SIMULATION == 1)
	{
		ptr_PID_Vitesse_D->Enable = 1;
		ptr_PID_Vitesse_D->gain_P = 0.05F;	//0.1F
		ptr_PID_Vitesse_D->gain_I = 0.0F;	//0.0F
		ptr_PID_Vitesse_D->gain_D = 0.4F;	//0.6F
		ptr_PID_Vitesse_D->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_D->commande_max = 30;
		ptr_PID_Vitesse_D->commande_min = -30;
		ptr_PID_Vitesse_D->Sommation_Sortie = 1; //1

		ptr_PID_Vitesse_G->Enable = 1;
		ptr_PID_Vitesse_G->gain_P = 0.05F;	//0.05F
		ptr_PID_Vitesse_G->gain_I = 0.0F;	//0.0F
		ptr_PID_Vitesse_G->gain_D = 0.4F;	//0.4F
		ptr_PID_Vitesse_G->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_G->commande_max = 30;
		ptr_PID_Vitesse_G->commande_min = -30;
		ptr_PID_Vitesse_G->Sommation_Sortie = 1;

		//Init des PID en position rotation
		struct st_pid_filter* ptr_PID_Position = _2_Asserv_GetPtr_PID_Pos();
		struct st_pid_filter* ptr_PID_Rotation = _2_Asserv_GetPtr_PID_Rot();

		ptr_PID_Position->Enable = 1;
		ptr_PID_Position->gain_P = 0.5F;	//0.2F
		ptr_PID_Position->gain_I = 0.0F;	//0.0F
		ptr_PID_Position->gain_D = 0.5F;	//1.0F
		ptr_PID_Position->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Position->commande_max = 10;
		ptr_PID_Position->commande_min = -10;
		ptr_PID_Position->Sommation_Sortie = 0;

		ptr_PID_Rotation->Enable = 1;
		ptr_PID_Rotation->gain_P = 0.9F;	//0.1F
		ptr_PID_Rotation->gain_I = 0.0F;	//0.0F
		ptr_PID_Rotation->gain_D = 1.0F;	//0.3F
		ptr_PID_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Rotation->commande_max = 10;
		ptr_PID_Rotation->commande_min = -10;
		ptr_PID_Rotation->Sommation_Sortie = 0;
	}else
	{
		ptr_PID_Vitesse_D->Enable = 1;
		ptr_PID_Vitesse_D->gain_P = 0.05F;	//0.1F
		ptr_PID_Vitesse_D->gain_I = 0.0F;	//0.0F
		ptr_PID_Vitesse_D->gain_D = 0.4F;	//0.6F
		ptr_PID_Vitesse_D->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_D->commande_max = 30;
		ptr_PID_Vitesse_D->commande_min = -30;
		ptr_PID_Vitesse_D->Sommation_Sortie = 1; //1

		ptr_PID_Vitesse_G->Enable = 1;
		ptr_PID_Vitesse_G->gain_P = 0.05F;	//0.05F
		ptr_PID_Vitesse_G->gain_I = 0.0F;	//0.0F
		ptr_PID_Vitesse_G->gain_D = 0.4F;	//0.4F
		ptr_PID_Vitesse_G->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Vitesse_G->commande_max = 30;
		ptr_PID_Vitesse_G->commande_min = -30;
		ptr_PID_Vitesse_G->Sommation_Sortie = 1;


		//Init des PID en position rotation
		struct st_pid_filter* ptr_PID_Position = _2_Asserv_GetPtr_PID_Pos();
		struct st_pid_filter* ptr_PID_Rotation = _2_Asserv_GetPtr_PID_Rot();

		ptr_PID_Position->Enable = 1;
		ptr_PID_Position->gain_P = 0.03F;	//0.2F
		ptr_PID_Position->gain_I = 0.0F;	//0.0F
		ptr_PID_Position->gain_D = 1.0F;	//1.0F
		ptr_PID_Position->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Position->commande_max = 20;
		ptr_PID_Position->commande_min = -20;
		ptr_PID_Position->Sommation_Sortie = 0;

		ptr_PID_Rotation->Enable = 1;
		ptr_PID_Rotation->gain_P = 0.02F;	//0.1F
		ptr_PID_Rotation->gain_I = 0.0F;	//0.0F
		ptr_PID_Rotation->gain_D = 0.5F;	//0.75F
		ptr_PID_Rotation->Max_Erreur_Cumul = 0;	//0
		ptr_PID_Rotation->commande_max = 20;
		ptr_PID_Rotation->commande_min = -20;
		ptr_PID_Rotation->Sommation_Sortie = 0;
	}


	Do_Odometrie_Calibration();
	Do_Odometrie_Calibration();
	Do_Odometrie_Calibration();


	Do_Rotation_Calibration(1);
	//Do_Rotation_Calibration(2);


	for(;;)
	{
		Task_Delay(1000);
	}
}


struct st_Parametre_Deplacement parameters1;
struct st_Parametre_Deplacement parameters2;

void TEST_Test_Deplacement(void *pvParameters)
{
	Task_Delay(2000);
	Init_Timing_Tache;

	//Réglages des vitesses et acceleration
	_2_Asservissement_Set_Distance_Speed_Accel(10, 1, 1);
	_2_Asservissement_Set_Rotation_Speed_Accel(10, 2, 2);

	//Réglages des parametres d'asservissement

	parameters1.Angle_Avant_Debut_Avance = 10;
	parameters1.Distance_Detection_Fin_Trajectoire = 300;
	parameters1.Angle_Detection_Fin_Trajectoire = 20;

	parameters2.Angle_Avant_Debut_Avance = 10;
	parameters2.Distance_Detection_Fin_Trajectoire = 2000;
	parameters2.Angle_Detection_Fin_Trajectoire = 20;



	//Ajoute des points de destination
	struct st_COORDONNEES destination;

	destination.Type_Arret = depla_SANS_freinage;
	destination.ptrParameters = parameters2;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 375;
	destination.Y = 1000;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Arret = depla_AVEC_freinage;
	destination.ptrParameters = parameters1;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 500;
	destination.Y = 1000;
	_2_Deplacement_Ajout_Point(&destination);



	destination.X = 500;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Arret = depla_SANS_freinage;
	destination.ptrParameters = parameters2;
	destination.X = 600;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 700;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 800;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 900;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 1000;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.X = 1100;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	destination.Type_Arret = depla_AVEC_freinage;
	destination.ptrParameters = parameters1;
	destination.X = 1200;
	destination.Y = 1200;
	_2_Deplacement_Ajout_Point(&destination);

	Task_Delete_Current;

	while (1)
	{
		Task_Delay_Until(50);
	}
}


void TEST_AX12(void *pvParameters)
{
	Task_Delay(1000);
	Init_Timing_Tache;

	_0_AX_12_POWER(true);
	Task_Delay(50);
	AX_12_LED_ON(1);

	/*Task_Delay(1000);
	AX_12_SET_POSITION(1, 150, 0);

	Task_Delay(1000);
	AX_12_SET_POSITION(1, 200, 0);

	Task_Delay(1000);
	AX_12_SET_POSITION(1, 220, 0);

	Task_Delay(1000);
	AX_12_SET_SPEED(1, 100, 0);
	AX_12_SET_POSITION(1, 150, 0);

	Task_Delay(1000);
	AX_12_TORQUE_DISABLE(1, false);*/


	//Réglages des vitesses et acceleration
	_2_Asservissement_Set_Distance_Speed_Accel(20, 1, 1);
	_2_Asservissement_Set_Rotation_Speed_Accel(20, 2, 2);

	//Réglages des parametres d'asservissement

	parameters1.Angle_Avant_Debut_Avance = 10;
	parameters1.Distance_Detection_Fin_Trajectoire = 300;
	parameters1.Angle_Detection_Fin_Trajectoire = 20;

	parameters2.Angle_Avant_Debut_Avance = 10;
	parameters2.Distance_Detection_Fin_Trajectoire = 2000;
	parameters2.Angle_Detection_Fin_Trajectoire = 20;

	Task_Delay(1000);
	//Leve la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	//Ajoute des points de destination
	struct st_COORDONNEES destination;

	destination.Type_Arret = depla_AVEC_freinage;
	destination.ptrParameters = parameters1;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 415;
	destination.Y = 970;
	_2_Deplacement_Ajout_Point(&destination);

	Task_Delay(2000);
	//baisse la trappe
	AX_12_SET_POSITION(1, 110, 0);
	Task_Delay(1000);

	destination.X = 385; //365
	destination.Y = 950;
	_2_Deplacement_Ajout_Point(&destination);

	Task_Delay(4000);
	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.X = 300;
	destination.Y = 872;
	_2_Deplacement_Ajout_Point(&destination);

	Task_Delay(2000);
	//baisse la trappe
	AX_12_SET_POSITION(1, 110, 0);
	Task_Delay(1000);

	destination.X = 250;
	destination.Y = 1040;
	_2_Deplacement_Ajout_Point(&destination);

	Task_Delay(5000);
	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_arriere;
	destination.X = 380;
	destination.Y = 800;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(4000);



	destination.Type_Arret = depla_SANS_freinage;
	destination.ptrParameters = parameters2;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 470;
	destination.Y = 1100;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(2000);

	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.Type_Arret = depla_AVEC_freinage;
	destination.ptrParameters = parameters1;

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 430;
	destination.Y = 1410;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(2000);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 380;
	destination.Y = 1550;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(3000);

	//baisse la trappe
	AX_12_SET_POSITION(1, 110, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 260;
	destination.Y = 1400;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(3500);

	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_arriere;
	destination.X = 400;
	destination.Y = 1600;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(2000);




	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 647;
	destination.Y = 1820;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(6500);

	//baisse la trappe
	AX_12_SET_POSITION(1, 110, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 840;
	destination.Y = 1720;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(3500);

	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 920;
	destination.Y = 1675;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(1500);

	//baisse la trappe
	AX_12_SET_POSITION(1, 110, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_avant;
	destination.X = 380;
	destination.Y = 1360;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(7500);

	//lève la trappe
	AX_12_SET_POSITION(1, 500, 0);
	Task_Delay(1000);

	destination.Type_Deplacement = xy_tour_av_arriere;
	destination.X = 500;
	destination.Y = 1430;
	_2_Deplacement_Ajout_Point(&destination);
	Task_Delay(4000);


	Task_Delete_Current;
}


