#include "print_server_client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* sends print job to the print server daemon ;; return 0 on success */
int
printer_print(int* handle, char* driver, char* job_name, char* description, char* data)
{
	/* socket routines */
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
	
	// Write initial command to server

	// buffer of 1024 bytes
	char buffer[SRVSIZE];
	int i, j;
	for(i = 0; i < SRVSIZE; i++)
		buffer[i] = '\0';

	char* cmd = "MKJOB";
	strcpy(buffer, cmd);
	if(send(socket_desc, buffer, SRVSIZE , 0) < 0){
		printf("ERROR: Send cmd to server failed.\n");
		return -5;
	}
	//printf("LIB: Cmd sent to server successfully.\n");
	
	/* add everything into the buffer for serialization */
	for(i = 0; i < SRVSIZE; i++)
		buffer[i] = '\0';
	
	// handle
	
	char handlestr[MAXHANDLE];
	if(handle != nil){
		snprintf(handlestr, MAXHANDLE-1, "%d", *handle);
		for(i = 0, j = 0; j < strlen(handlestr) && i < SRVSIZE; i++, j++)
			buffer[i] = handlestr[j];
	}else{
		//printf("LIB: No handle being serialized.\n");
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
	
	// job data (filename)
	buffer[i] = '~';
	i++;

	for(j = 0; j < strlen(data) && i < SRVSIZE; i++, j++)
		buffer[i] = data[j];
	
	buffer[i] = '~';
	i++;

	
	/* NOTE: Pushing sending file (if implemented) onto another message write. ← ← */

	
	if(i > SRVSIZE){
		printf("ERROR: Send buffer overflow.\n");
		return -3;
	}else{
		//printf("LIB: i was: %d\n", i);
	}
	
	/*printf("LIB: Buffer follows…\n");
	int l;
	for(l = 0; l <= i; l++)
		putchar(buffer[l]);
	printf("\nLIB: Buffer ends…\n");
	*/
	
	// Send to server
	//printf("LIB: Begin writing to server…\n");
	
	if(send(socket_desc, buffer, SRVSIZE , 0) < 0){
		printf("ERROR: Send to server failed.\n");
		return -4;
	}
	//printf("LIB: Buffer sent to server successfully.\n");
	printf("LIB: Jobs successfully transmitted to server.\n");

	return 0;
}

/* list installed drivers on server ;; return NULL-terminated arr of printer_driver_t's */
printer_driver_t**
printer_list_drivers(int *number)
{
	/* socket routines */
	int socket_desc;
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(SRVADDR);
	server.sin_family = AF_INET;
	server.sin_port = htons( SRVPORT );
	    
	// Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if(socket_desc == -1){
		printf("ERROR: Could not create socket.\n");
		return nil;
	}
	
	// Connect to remote server
	if(connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		printf("ERROR: Connecting to server socket.\n");
		return nil;
	}
	     
	printf("LIB: Connected to server socket.\n");
	
	// Write initial command to server

	// buffer of 1024 bytes
	char buffer[SRVSIZE];
	int i;
	for(i = 0; i < SRVSIZE; i++)
		buffer[i] = '\0';

	char* cmd = "GETDRIVERS";
	strcpy(buffer, cmd);
	if(send(socket_desc, buffer, SRVSIZE , 0) < 0){
		printf("ERROR: Send cmd to server failed.\n");
		return nil;
	}
	//printf("LIB: Cmd sent to server successfully.\n");
	
	/* get info from server */
	for(i = 0; i < SRVSIZE; i++)
		buffer[i] = '\0';

	//Receive a reply from the server
	printf("LIB: Waiting on server to write list…\n");
	if(recv(socket_desc, buffer, SRVSIZE, 0) < 0){
		printf("LIB: recv failed.\n");
		return nil;
	}
	printf("LIB: Reply received.\n");
	
	//printf("LIB: Reply is: %s\n", buffer);
	
	// format is: N~pname~dname~version~pname2~dname2~version2…~
	// mode is: 0=pname, 1=dname, 2=version
	int mode = 0;
	int driven = 0;
	
	// read out leading count of driver no's
	char countstr[MAXNAME];
	for(i = 0; i < SRVSIZE && i < MAXNAME; i++){
		char c = buffer[i];
		if(c == '~'){
			countstr[i] = '\0';
			i++;
			break;
		}
		countstr[i] = c;
	}

	int count = atoi(countstr);
	*number = count;
	
	//printf("LIB: Count is: %d\n", count);
	
	printer_driver_t** drivers = calloc(count, sizeof(printer_driver_t*));
	int z;
	for(z = 0; z < count; z++)
		drivers[z] = calloc(1, sizeof(printer_driver_t));
	
	char str[MAXNAME];
	
	int j;
	for(j = 0; i < SRVSIZE && driven < count && j < MAXNAME; i++){
		// populate array of drivers
		char c = buffer[i];
		if(c == '~'){
			str[j] = '\0';
			
			if(mode == 0){
				// printer_name
				drivers[driven]->printer_name = calloc(MAXNAME, sizeof(char));
				strcpy(drivers[driven]->printer_name, str);
				mode++;
			}else if(mode == 1){
				// driver_name
				drivers[driven]->driver_name = calloc(MAXNAME, sizeof(char));
				strcpy(drivers[driven]->driver_name, str);
				mode++;
			}else if(mode == 2){
				// driver_version
				drivers[driven]->driver_version = calloc(MAXNAME, sizeof(char));
				strcpy(drivers[driven]->driver_version, str);
				
				//printf("LIB: adding printer named: %s\n", drivers[driven]->printer_name);
				
				driven++;
				mode = 0;
			}
			
			j = 0;
		}else{
			str[j] = c;
			j++;
		}
	}

	return drivers;
}
