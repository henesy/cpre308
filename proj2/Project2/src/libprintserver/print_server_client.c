#include "print_server_client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/* sends print job to the print server daemon ;; return 0 on success */
int
printer_print(int* handle, char* driver, char* job_name, char* description, char* data)
{
	// hardcode socket stuff for now
	int socket_desc;
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(SRVADDR);
    server.sin_family = AF_INET;
    server.sin_port = htons( SRVPORT );
    
	// Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if(socket_desc == -1){
        printf("ERROR: Could not create socket.\n");
        return -1;
    }
 
    // Connect to remote server
    if(connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
        printf("ERROR: Connecting to server socket.\n");
        return -2;
    }
     
    printf("LIB: Connected to server socket.\n");
	
	// buffer of 1024 bytes
	char buffer[SRVSIZE];
	int i, j;
	for(i = 0; i < SRVSIZE; i++)
		buffer[i] = '\0';
	
	/* add everything into the buffer for serialization */
	// handle
	
	char handlestr[MAXHANDLE];
	if(handle != nil){
		snprintf(handlestr, MAXHANDLE-1, "%d", *handle);
		for(i = 0, j = 0; j < strlen(handlestr) && i < SRVSIZE; i++, j++)
			buffer[i] = handlestr[j];
	}else{
		printf("LIB: No handle being serialized.\n");
		i = 0;
	}

	// driver
	buffer[i] = '~';
	i++;
	
	for(j = 0; j < strlen(driver) && i < SRVSIZE; i++, j++)
		buffer[i] = driver[j];
	
	// job name (splitting on ~'s)
	buffer[i] = '~';
	i++;
	
	for(j = 0; j < strlen(job_name) && i < SRVSIZE; i++, j++)
		buffer[i] = job_name[j];
	
	// job desc
	buffer[i] = '~';
	i++;
	
	for(j = 0; j < strlen(description) && i < SRVSIZE; i++, j++)
		buffer[i] = description[j];
	
	// job data
	/*
		!! TODO !!: Serializing this data is broken, need to fix.
	*/
	buffer[i] = '%';
	i++;
	
	/*
	for(j = 0; j < (sizeof(data) / sizeof(char)) && i < SRVSIZE; i++, j++)
		buffer[i] = data[j];
	*/
	
	int datasize = sizeof(data);
	memcpy(buffer+i, data, datasize);
	
	i += datasize;
	
	buffer[i] = '%';
	buffer[i+1] = 'E';
	buffer[i+2] = 'O';
	buffer[i+3] = 'F';
	i += 3;
	
	if(i > SRVSIZE){
		printf("ERROR: Send buffer overflow.\n");
		return -3;
	}else
		printf("LIB: i was: %d\n", i);
	
	printf("LIB: Buffer follows…\n");
	int l;
	for(l = 0; l <= i; l++)
		putchar(buffer[l]);
	printf("\nLIB: Buffer ends…\n");
	
	// Send to server
	printf("LIB: Begin writing to server…\n");
	
	if(send(socket_desc, buffer, SRVSIZE , 0) < 0){
        printf("ERROR: Send to server failed.\n");
        return -4;
    }
    printf("LIB: Buffer sent to server successfully.\n");
	
	return 0;
}

/* list installed drivers on server ;; return NULL-terminated arr of printer_driver_t's */
printer_driver_t**
printer_list_drivers(int *number)
{


	return nil;
}
