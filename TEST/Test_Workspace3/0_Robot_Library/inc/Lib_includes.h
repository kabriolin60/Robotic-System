/*
 * Lib_includes.h
 *
 *  Created on: 2 déc. 2020
 *      Author: kabri
 */

#ifndef LIB_INCLUDES_H_
#define LIB_INCLUDES_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

/* Librairies */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cr_section_macros.h>

/* Includes FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "ring_buffer.h"

/* Includes déclarations communes */
#include "Common_Init.h"
#include "Commun_Configuration.h"
#include "Communication_Structures.h"

/* Includes sources communes */
#include "0_Communication.h"
#include "0_Event_Group.h"

#include "0_RS485.h"

#endif /* LIB_INCLUDES_H_ */
