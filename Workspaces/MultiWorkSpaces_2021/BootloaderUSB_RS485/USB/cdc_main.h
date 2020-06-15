/*
 * cdc_main.h
 *
 *  Created on: 6 août 2014
 *      Author: Fixe
 */

#ifndef CDC_MAIN_H_
#define CDC_MAIN_H_

void USBINIT(void);

void Task_USBReceived(void *pv_parameters);

#endif /* CDC_MAIN_H_ */
