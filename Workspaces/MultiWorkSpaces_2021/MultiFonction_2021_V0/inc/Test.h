/*
 * Test.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef TEST_H_
#define TEST_H_

#include "stdio.h"


#include "0_Codeurs.h"

#include "1_PID.h"
#include "1_Odometrie.h"
#include "1_Asservissement.h"
#include "2_Echange_Datas.h"
#include "2_Asservissement.h"

#include "3_Asservissement.h"

#include "Type_Declaration.h"

#include "2_Spline.h"
#include "0_AX12.h"
#include "1_AX12.h"

#include "1_Update_Values.h"


void TEST_init_parametres(void);


void Test_Spline(void *pvparameter);


void TEST_Send_Board_Infos(void *pvParameters);


void Do_Odometrie_Calibration(void);

void Do_Rotation_Calibration(int nb_tours);

void Test_Calibration_Odometrie(void * pvParameter);


void TEST_Test_Deplacement(void *pvParameters);

void TEST_AX12(void *pvParameters);

void vAFunction( void );

void Test_Task_Graphique(void* pvParameter);

void TEST_PID_Tunning(void* pvParameter);

#endif /* TEST_H_ */
