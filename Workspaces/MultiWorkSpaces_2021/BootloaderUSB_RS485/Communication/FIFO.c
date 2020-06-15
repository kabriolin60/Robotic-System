/*
 * FIFO.c
 *
 *  Created on: 12 juin 2013
 *      Author: Robotic System
 */

#include "FIFO.h"


/**************************************************
Fct Ajout d'un element au tableau
Input:  Pointeur vers la pile
        Pointeur vers la donnée à ajouter
Output: 1: OK
        0: Erreur
 **************************************************/

__attribute__((optimize("O3"))) unsigned char FIFO_ADD_ELEMENT(struct st_FIFO *Fifo, unsigned char data)
{
	taskENTER_CRITICAL();

	//Si le FIFO n'est pas pleine
	if(!FIFO_IS_FULL(Fifo))
	{
		Fifo->Index_Ecriture = (Fifo->Index_Ecriture + 1) % MAX_FIFO_ELEMENTS;
		Fifo->Element[Fifo->Index_Ecriture] = data;
		Fifo->Nb_elements_Ajoutes++;
		if(FIFO_NB_DATA(Fifo) > Fifo->maximumelementever)
		{
			Fifo->maximumelementever = FIFO_NB_DATA(Fifo);
		}

		taskEXIT_CRITICAL();
		return FIFO_PASS;
	}

	taskEXIT_CRITICAL();
	return FIFO_ERREUR;
}

/*************************************************/


/**************************************************
Fct Retrait d'un element au tableau
Input:  Pointeur vers la pile
        Pointeur vers la donnée à lire
Output: 1: OK
        0: Erreur
 **************************************************/

__attribute__((optimize("O3"))) unsigned char FIFO_READ_ELEMENT(struct st_FIFO *Fifo, unsigned char *data)
{
	taskENTER_CRITICAL();

	//Si le FIFO n'est pas vide
	if(!FIFO_IS_EMPTY(Fifo))
	{
		Fifo->Index_Lecture = (Fifo->Index_Lecture + 1) % MAX_FIFO_ELEMENTS;
		*data = Fifo->Element[Fifo->Index_Lecture];
		Fifo->Nb_elements_Retires++;

		taskEXIT_CRITICAL();
		return FIFO_PASS;
	}

	taskEXIT_CRITICAL();
	return FIFO_ERREUR;
}

/*************************************************/


/**************************************************
Fct de test si une FIFO est pleine
Input:  Pointeur vers la pile
Output: 1: Pleine
        0: Non Pleine
 **************************************************/

__attribute__((optimize("O3"))) unsigned char FIFO_IS_FULL(struct st_FIFO *Fifo)
{
	unsigned int Nb_elements;
	Nb_elements = Fifo->Nb_elements_Ajoutes - Fifo->Nb_elements_Retires;
	if(Nb_elements == MAX_FIFO_ELEMENTS)
	{
		//pile pleine
		return FIFO_PASS;
	}

	return FIFO_ERREUR;
}

/*************************************************/


/**************************************************
Fct de test si une FIFO est vide
Input:  Pointeur vers la pile
Output: 1: Vide
        0: Non Vide
 **************************************************/

__attribute__((optimize("O3"))) unsigned char FIFO_IS_EMPTY(struct st_FIFO *Fifo)
{
	unsigned int Nb_elements;
	Nb_elements = Fifo->Nb_elements_Ajoutes - Fifo->Nb_elements_Retires;
	if(Nb_elements == 0)
	{
		//pile vide
		return FIFO_PASS;
	}

	return FIFO_ERREUR;
}

/*************************************************/


/**************************************************
Fct renvoyant le nombre de données dans la FIFO
Input:  Pointeur vers la pile
Output: Nombre de données dans la Pile
 **************************************************/

__attribute__((optimize("O3"))) unsigned int FIFO_NB_DATA(struct st_FIFO *Fifo)
{
	taskENTER_CRITICAL();

	unsigned int Nb_elements;
	Nb_elements = Fifo->Nb_elements_Ajoutes - Fifo->Nb_elements_Retires;

	taskEXIT_CRITICAL();
	return Nb_elements;
}

/*************************************************/


/**************************************************
Fct Effacement de la FIFO
Input:  Pointeur vers la pile
Output: 1: OK
        0: Erreur
 **************************************************/

unsigned char FIFO_FLUSH(struct st_FIFO *Fifo)
{
	taskENTER_CRITICAL();
	Fifo->Index_Ecriture = 0;
	Fifo->Index_Lecture = 0;
	Fifo->Nb_elements_Ajoutes = 0;
	Fifo->Nb_elements_Retires = 0;
	Fifo->maximumelementever = 0;
	taskEXIT_CRITICAL();
	return FIFO_PASS;
}

/*************************************************/
