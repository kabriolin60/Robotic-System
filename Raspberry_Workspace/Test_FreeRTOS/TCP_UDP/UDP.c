/*
 * UDP.c
 *
 *  Created on: 11 nov. 2020
 *      Author: pi
 */

#include "UDP.h"

#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"


void error(const char *msg)
{
	//perror(msg);
	//exit(0);
	vTaskDelay(pdMS_TO_TICKS(250));
}

void Init_UDP(void* pvparameter)
{
	int sock, length, n;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buf[1024];
	int port = *(int *)pvparameter;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		error("Opening socket");
	length = sizeof(server);
	bzero(&server, length);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&server, length) < 0)
		error("binding");
	fromlen = sizeof(struct sockaddr_in);

	printf("Serveur UDP en écoute sur le port %d !", port);


	while (1)
	{
		while ((n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen)) == -1 )
		{
			vTaskDelay(pdMS_TO_TICKS(250));
		}
		//n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
		//if (n < 0) error("recvfrom");
		//write(1, buf, n);
		n = sendto(sock, "Etat PiRobot", 12, 0, (struct sockaddr *)&from, fromlen);
		//if (n < 0) error("sendto");
	}
}


