/*
 * FIFO.h
 *
 *  Created on: 12 juin 2013
 *      Author: Robotic System
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdlib.h>
#include "Includes.h"

#define MAX_FIFO_ELEMENTS (300)

#ifndef OK
#define OK 1
#define NOK 0
#endif

#ifndef FIFO_PASS
#define FIFO_PASS 1
#endif

#ifndef FIFO_ERREUR
#define FIFO_ERREUR 0
#endif

struct st_FIFO
  {
    unsigned short Index_Ecriture;
    unsigned short Index_Lecture;
    unsigned int Nb_elements_Ajoutes;
    unsigned int Nb_elements_Retires;
    unsigned char Element[MAX_FIFO_ELEMENTS];
    unsigned short maximumelementever;
  };


/**************************************************
Fct Ajout d'un element au tableau
Input:  Pointeur vers la pile
        Pointeur vers la donnée à ajouter
Output: 1: OK
        0: Erreur
**************************************************/

unsigned char FIFO_ADD_ELEMENT(struct st_FIFO *Fifo, unsigned char data);

/*************************************************/


/**************************************************
Fct Retrait d'un element au tableau
Input:  Pointeur vers la pile
        Pointeur vers la donnée à lire
Output: 1: OK
        0: Erreur
**************************************************/

unsigned char FIFO_READ_ELEMENT(struct st_FIFO *Fifo, unsigned char *data);

/*************************************************/


/**************************************************
Fct de test si une FIFO est pleine
Input:  Pointeur vers la pile
Output: 1: Pleine
        0: Non Pleine
**************************************************/

unsigned char FIFO_IS_FULL(struct st_FIFO *Fifo);

/*************************************************/


/**************************************************
Fct de test si une FIFO est vide
Input:  Pointeur vers la pile
Output: 1: Vide
        0: Non Vide
**************************************************/

unsigned char FIFO_IS_EMPTY(struct st_FIFO *Fifo);

/*************************************************/


/**************************************************
Fct renvoyant le nombre de données dans la FIFO
Input:  Pointeur vers la pile
Output: Nombre de données dans la Pile
**************************************************/

unsigned int FIFO_NB_DATA(struct st_FIFO *Fifo);

/*************************************************/


/**************************************************
Fct Effacement de la FIFO
Input:  Pointeur vers la pile
Output: 1: OK
        0: Erreur
**************************************************/

unsigned char FIFO_FLUSH(struct st_FIFO *Fifo);

/*************************************************/

#endif /* FIFO_H_ */
