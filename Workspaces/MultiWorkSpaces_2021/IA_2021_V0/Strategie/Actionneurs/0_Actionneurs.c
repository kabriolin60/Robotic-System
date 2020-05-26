  
/*
 * 0_Actionneurs.c
 *
 *  Created on: 26 mai 2020
 *      Author: kabri
 */

#include "FreeRTOS.h"
#include "event_groups.h"

#include "0_Actionneurs.h"
#include "0_Infos.h"


/*****************************************************************************
 ** Function name:		_0_Actionneurs_Move_1_Servo
 **
 ** Descriptions:		Fonction de deplacement d'un servo (peu importe le type)
 **
 ** parameters:			servo ID
 **                     Destination
 **                     Torque (used only on AX12)
 **
 **                     Temps de deplacement (ms)
 **                     Attente
 **
 ** Returned value:		true: we arrived
 **                     false: error
 **
 *****************************************************************************/
bool _0_Actionneurs_Move_1_Servo(byte ID, unsigned short desti, short torque, 
                                 unsigned short time, bool attente)
{
    struct st_Destination_Servos servos;
    memset(&servos, 0, sizeof(struct st_Destination_Servos));
    
    //Only one servo
    servos.Nombre_servos_to_move = 1;
    servos.Time_to_move = time;
    
    servos.servo[0].ID = ID;
    servos.servo[0].Destination = desti;
    servos.servo[0].Torque = torque;
    
    _2_Comm_Send_Servos_Destinations(&servos, RS485_port);
    
    if(!attente)
        return true; //assume the servos will reach it's position
    
    
    //wait for the servo to reach it's position
    //first, wait the requested time
    Task_Delay(time);
    
    //the check if the servo is arrived
    struct servo_destination position;
    
    position = _0_Get_Servo_Position(ID);
    if(position.Destination >= desti - 10 && position.Destination <= desti + 10)
    {
        //servo 1 is arrived
        return true;
    }
    
    return false;  
}


/*****************************************************************************
 ** Function name:		_0_Actionneurs_Move_2_Servo
 **
 ** Descriptions:		Fonction de deplacement de 2 servo (peu importe le type)
 **
 ** parameters:			servo ID
 **                     Destination
 **                     Torque (used only on AX12)
 **
 **                     servo ID servo 2
 **                     Destination servo 2
 **                     Torque (used only on AX12) servo 2
 **
 **                     Temps de deplacement (ms)
 **                     Attente
 **
 ** Returned value:		true: we arrived
 **                     false: error
 **
 *****************************************************************************/
bool _0_Actionneurs_Move_2_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 unsigned short time, bool attente)
{
    struct st_Destination_Servos servos;
    memset(&servos, 0, sizeof(struct st_Destination_Servos));
    
    //Only two servo
    servos.Nombre_servos_to_move = 2;
    servos.Time_to_move = time;
    
    servos.servo[0].ID = ID;
    servos.servo[0].Destination = desti;
    servos.servo[0].Torque = torque;
    
    servos.servo[1].ID = ID_2;
    servos.servo[1].Destination = desti_2;
    servos.servo[1].Torque = torque_2;
    
    _2_Comm_Send_Servos_Destinations(&servos, RS485_port);
    
    if(!attente)
        return true; //assume the servos will reach it's position
    
    
    
    //wait for the servo to reach it's position
    //first, wait the requested time
    Task_Delay(time);
    
    //the check if the servo is arrived
    struct servo_destination position;
    
    position = _0_Get_Servo_Position(ID);
    if(position.Destination >= desti - 10 && position.Destination <= desti + 10)
    {
        //servo 1 is arrived
        position = _0_Get_Servo_Position(ID_2);
        if(position.Destination >= desti_2 - 10 && position.Destination <= desti_2 + 10)
        {    
            //servo 2 is arrived
            return true;
        }
    }
    
    return false;       
}


/*****************************************************************************
 ** Function name:		_0_Actionneurs_Move_3_Servo
 **
 ** Descriptions:		Fonction de deplacement de 3 servo (peu importe le type)
 **
 ** parameters:			servo ID
 **                     Destination
 **                     Torque (used only on AX12)
 **
 **                     servo ID servo 2
 **                     Destination servo 2
 **                     Torque (used only on AX12) servo 2
 **
 **                     servo ID servo 3
 **                     Destination servo 3
 **                     Torque (used only on AX12) servo 3
 **
 **                     Temps de deplacement (ms)
 **                     Attente
 **
 ** Returned value:		true: we arrived
 **                     false: error
 **
 *****************************************************************************/
bool _0_Actionneurs_Move_3_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 byte ID_3, unsigned short desti_3, short torque_3, 
                                 unsigned short time, bool attente)
{
    struct st_Destination_Servos servos;
    memset(&servos, 0, sizeof(struct st_Destination_Servos));
    
    //Only 3 servo
    servos.Nombre_servos_to_move = 3;
    servos.Time_to_move = time;
    
    servos.servo[0].ID = ID;
    servos.servo[0].Destination = desti;
    servos.servo[0].Torque = torque;
    
    servos.servo[1].ID = ID_2;
    servos.servo[1].Destination = desti_2;
    servos.servo[1].Torque = torque_2;
    
    servos.servo[2].ID = ID_3;
    servos.servo[2].Destination = desti_3;
    servos.servo[2].Torque = torque_3;
       
    _2_Comm_Send_Servos_Destinations(&servos, RS485_port);
    
    if(!attente)
        return true; //assume the servos will reach it's position
    
    
    //wait for the servo to reach it's position
    //first, wait the requested time
    Task_Delay(time);
    
    //the check if the servo is arrived
    struct servo_destination position;
    
    position = _0_Get_Servo_Position(ID);
    if(position.Destination >= desti - 10 && position.Destination <= desti + 10)
    {
        //servo 1 is arrived
        position = _0_Get_Servo_Position(ID_2);
        if(position.Destination >= desti_2 - 10 && position.Destination <= desti_2 + 10)
        {       
            //servo 2 is arrived
            position = _0_Get_Servo_Position(ID_3);
            if(position.Destination >= desti_3 - 10 && position.Destination <= desti_3 + 10)
            {       
                //servo 3 is arrived
                return true;
            }
        }
    }
    
    return false;  
}


/*****************************************************************************
 ** Function name:		_0_Actionneurs_Move_4_Servo
 **
 ** Descriptions:		Fonction de deplacement de 4 servo (peu importe le type)
 **
 ** parameters:			servo ID
 **                     Destination
 **                     Torque (used only on AX12)
 **
 **                     servo ID servo 2
 **                     Destination servo 2
 **                     Torque (used only on AX12) servo 2
 **
 **                     servo ID servo 3
 **                     Destination servo 3
 **                     Torque (used only on AX12) servo 3
 **
 **                     servo ID servo 4
 **                     Destination servo 4
 **                     Torque (used only on AX12) servo 4
 **
 **                     Temps de deplacement (ms)
 **                     Attente
 **
 ** Returned value:		true: we arrived
 **                     false: error
 **
 *****************************************************************************/
bool _0_Actionneurs_Move_4_Servo(byte ID, unsigned short desti, short torque, 
                                 byte ID_2, unsigned short desti_2, short torque_2, 
                                 byte ID_3, unsigned short desti_3, short torque_3, 
                                 byte ID_4, unsigned short desti_4, short torque_4,
                                 unsigned short time, bool attente)
{
    struct st_Destination_Servos servos;
    memset(&servos, 0, sizeof(struct st_Destination_Servos));
    
    //Only 4 servo
    servos.Nombre_servos_to_move = 4;
    servos.Time_to_move = time;
    
    servos.servo[0].ID = ID;
    servos.servo[0].Destination = desti;
    servos.servo[0].Torque = torque;
    
    servos.servo[1].ID = ID_2;
    servos.servo[1].Destination = desti_2;
    servos.servo[1].Torque = torque_2;
    
    servos.servo[2].ID = ID_3;
    servos.servo[2].Destination = desti_3;
    servos.servo[2].Torque = torque_3;
    
    servos.servo[3].ID = ID_4;
    servos.servo[3].Destination = desti_4;
    servos.servo[3].Torque = torque_4;
       
    _2_Comm_Send_Servos_Destinations(&servos, RS485_port);
    
    if(!attente)
        return true; //assume the servos will reach it's position
    
    
    //wait for the servo to reach it's position
    //first, wait the requested time
    Task_Delay(time);
    
    //the check if the servo is arrived
    struct servo_destination position;
    
    position = _0_Get_Servo_Position(ID);
    if(position.Destination >= desti - 10 && position.Destination <= desti + 10)
    {
        //servo 1 is arrived
        position = _0_Get_Servo_Position(ID_2);
        if(position.Destination >= desti_2 - 10 && position.Destination <= desti_2 + 10)
        {       
            //servo 2 is arrived
            position = _0_Get_Servo_Position(ID_3);
            if(position.Destination >= desti_3 - 10 && position.Destination <= desti_3 + 10)
            {       
                //servo 3 is arrived
                position = _0_Get_Servo_Position(ID_4);
                if(position.Destination >= desti_4 - 10 && position.Destination <= desti_4 + 10)
                {       
                    //servo 4 is arrived
                    return true;
                }
            }
        }
    }
    
    return false;  
}
