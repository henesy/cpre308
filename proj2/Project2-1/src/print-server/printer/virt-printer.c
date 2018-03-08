


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>
#include <string.h>

#define PRINT_STREAM_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

int verbose_flag = 0;
FILE* log_stream;
FILE* debug_stream;
FILE* print_stream_in;
FILE* print_stream_out;

//void onExit(int p);

//const struct sigaction on_exit_act = {
//	.sa_handler = onExit,
	//.sa_sigaction = NULL,
	//.sa_mask = 0,
	//.sa_flags = 0,
	//.sa_restorer = NULL,
//};

int main(int argc, char* argv[])
{
	int i;
	int rv;
	int c;	
	int err;
	int status = 0;
	int pipefd[2];
	pid_t child;
	log_stream = stdout;	
	debug_stream = stderr;
	char *printer_name_r = NULL;
	char *printer_name_w = NULL;
	char *line = NULL;
	char *temp = NULL;
	char *arguments[4];
	FILE *write_end;

	while((c = getopt(argc, argv, "f:d:n:v?")) != -1)
	{
		switch(c)
		{
			case 'f': // log stream file
				log_stream = freopen(optarg, "w", stdout);
				if(log_stream == NULL){
					perror("freopen");
					abort();
				}
				break;
			case 'd': // debug stream file
				debug_stream = freopen(optarg, "w", stderr);
				break;
			case 'n': // printer name
				printer_name_r = calloc(13 + strlen(optarg), sizeof(char));
				if(printer_name_r == NULL){
					perror("calloc");
					abort();
				}
				printer_name_w = calloc(13 + strlen(optarg), sizeof(char));
				if(printer_name_w == NULL){
					perror("calloc");
					abort();
				}
				strncat(printer_name_r, "./drivers/", 10);
				strncat(printer_name_r, optarg, strlen(optarg));
				strncat(printer_name_r, "-r", 2);
				
				strncat(printer_name_w, "./drivers/", 10);
				strncat(printer_name_w, optarg, strlen(optarg));
				strncat(printer_name_w, "-w", 2);
				break;
			case 'v': // turn on verbose mode
				verbose_flag = 1;
				break;
			case '?': // print help information
				fprintf(stderr, "%s\n", argv[0]);
				break;
		}
	}
	
	line = calloc(1024, sizeof(char));
	if(line == NULL){
		perror("calloc");
		abort();
	}
	
	// replace the constant string with `printer_name` later	
	rv = mkfifo(printer_name_r, PRINT_STREAM_MODE);
	if(rv)
	{
		perror("mkfifo()");
		abort();
	}

	rv = mkfifo(printer_name_w, PRINT_STREAM_MODE);
	if(rv){
		perror("mkfifo()");
		abort();
	}

	//sigaction(9,&on_exit_act, NULL);

	printf("Switching to background\n");fflush(stdout);
	daemon(1, 1);

	print_stream_in = fopen(printer_name_r, "r");
	if(!print_stream_in)
	{
		perror("fopen()");
		abort();
	}

	print_stream_out = fopen(printer_name_w, "w");
	if(!print_stream_out){
		perror("fopen()");
		abort();
	}

	// 1. watch the fifo/print stream
	// 2. when the print stream is not empty, assume the first line is meta data
	// 3. create an unnamed pipe
	// 4. fork
	// 5. change stdin of the child to be the pipe
	// 6. call `ps2pdf - job_name`
	// 7. copy data from the fifo to the pipe until `##END##` is reached
	// 8. wait for the next job
	while(!feof(print_stream_in)){
		// read the first line
		fgets(line, 1024, print_stream_in);
		// if it returns, that means something was sent to the fifo
		if(verbose_flag) printf("line recieved\n"); fflush(stdout);
		printf("%s", line); fflush(stdout);
		
		// see if the line is "##NAME##"
		if(!strncmp(line, "##NAME##\n", 9)){
			if(verbose_flag){
				printf("found ##NAME##\n"); fflush(stdout);
			}
			fprintf(print_stream_out, "%s\n", printer_name_r); fflush(print_stream_out);
		}else if(!strncmp(line, "##DESCRIPTION##\n", 16)){
			if(verbose_flag){
				printf("found ##DESCRIPTION##\n"); fflush(stdout);
			}
			fprintf(print_stream_out, "a generic printer\n"); fflush(print_stream_out);
		}else if(!strncmp(line, "##LOCATION##\n", 13)){
			if(verbose_flag){
				printf("found ##LOCATION##\n"); fflush(stdout);
			}
			fprintf(print_stream_out, "center of a black hole\n"); fflush(print_stream_out);
		}else if(!strncmp(line, "##END##\n", 8)){
			continue;
		}else{
			
			// parse out the file name from the first line
			temp = strtok(line, ": ");
			if(temp == NULL){
				printf("invalid format\n"); fflush(stdout);
				continue;
			}
			if(verbose_flag) printf("temp: %s\n", temp); fflush(stdout);
			temp = strtok(NULL, ": ");
			if(temp != NULL){
				temp[strlen(temp) - 3] = '\0';
				if(verbose_flag) printf("temp: %s\n", temp); fflush(stdout);
			}else{
				fprintf(stderr, "invalid name for output file\n"); fflush(stderr);
				continue;
			}

			
			// create the argument array
			arguments[0] = "ps2pdf";
			arguments[1] = "-";
			arguments[2] = strdup(temp);
			arguments[3] = NULL;
			if(verbose_flag){
				printf("created the argument array, it's contents:\n");
				for(i = 0; i < 4; i++){
					printf("arguments[%d]: %s\n", i, arguments[i]); fflush(stdout);
				}
			}

			// create an unnamed pipe
			err = pipe(pipefd);
			if(err){
				perror("pipe");
				abort();
			}
			if(verbose_flag) printf("created an unnamed pipe\n"); fflush(stdout);

			// fork a child
			child = fork();

			// if child
			if(child == 0){
				// close the write end of pipe file descriptor for the read side
				close(pipefd[1]);
				// reopen stdin file handle using the read end of the pipe
				dup2(pipefd[0], fileno(stdin));
				if(verbose_flag) printf("duplicated read end of pipe to stdin\n"); fflush(stdout);
				// call `ps2pdf - job_name`
				execvp(arguments[0], arguments);
				// if execvp fails
				perror("execvp");
				close(pipefd[0]);
				close(pipefd[1]);
			// if parent
			}else{
				// close the read end
				close(pipefd[0]);
				// read data from print_stream and write it to the write end of the pipe
				// but only until the line "##END##" is reached
				write_end = fdopen(pipefd[1], "w");
				if(write_end == NULL){
					perror("fdopen");
					abort();
				}
				if(verbose_flag) printf("opened the write end of the pipe as FILE *\n"); fflush(stdout);
				fgets(line, 1024, print_stream_in);
				if(verbose_flag) printf("line: %s", line); fflush(stdout);
				while(strncmp(line, "##END##", 7)){
					fprintf(write_end, "%s", line); fflush(write_end);
					fgets(line, 1024, print_stream_in);
					if(verbose_flag) printf("line: %s", line); fflush(stdout);
				}
				// once "##END##" is reached, read one last time, and close the write end of pipe
				if(verbose_flag) printf("reached the ##END##\n"); fflush(stdout);
				fclose(write_end);
				write_end = 0;
				// wait for child to exit
				wait(&status);
				
				//fclose(write_end);
			}
		}
	}
	
	free(line);
	if(-1 != lseek(fileno(print_stream_in), 1, SEEK_CUR)){
		fclose(print_stream_in);
	}
	fclose(print_stream_out);
	unlink(printer_name_r);
	unlink(printer_name_w);
	free(printer_name_r);
	free(printer_name_w);
	return 0;
}

//void onExit(int p)
//{
	//fclose(print_stream_in);
	//fclose(print_stream_out);
	//remove("./drivers/printer1");
//}



