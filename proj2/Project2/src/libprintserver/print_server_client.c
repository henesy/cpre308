#include "print_server_client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

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
    
	//Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        return 1;
    }
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connect error\n");
        return 2;;
    }
     
    printf("Connected\n");
	
	// buffer of 512 bytes
	char* buffer[SRVSIZE];
	
	
	
	return 0;
}

/* list installed drivers on server ;; return NULL-terminated arr of printer_driver_t's */
printer_driver_t**
printer_list_drivers(int *number)
{


	return nil;
}
