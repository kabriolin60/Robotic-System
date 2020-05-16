/*
 * Test.h
 *
 *  Created on: 15 avr. 2020
 *      Author: kabri
 */

#ifndef TEST_H_
#define TEST_H_

void TEST_init_parametres(void);

void TEST_init_parametres_task2(void *pvParameters);


void Do_Odometrie_Calibration(void);

void Do_Rotation_Calibration(int nb_tours);

void Test_Calibration_Odometrie(void * pvParameter);


void TEST_init_parametres_task(void *pvParameters);

void TEST_AX12(void *pvParameters);

void vAFunction( void );

#endif /* TEST_H_ */
