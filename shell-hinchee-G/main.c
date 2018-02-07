#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"

void	split(char* line, char** out);

int	findpid(void* Node, void* pid);

/* shell is a novel shell written by seh for CPRE 308 section G */
void
main(int argc, char** argv)
{
	char* prompt = nil;
	int debug = false;
	List jobs = mklist();

	/* arg processing */
	int arg;
	while((arg = getopt(argc, argv, "dp:")) != -1){
		switch(arg){
		case 'p': 
			prompt = optarg;
			break;
		case 'd':
			debug = true;
			break;
		default:
			exit(1);
		}
	}

	if(prompt == nil)
		prompt = "308sh> ";

	/* loop */
	int run = true;
	while(run){
		char in[256];
		int bgstatus = -1;
		int bgpid = -1;
		
		// Check background process status (if any)
		bgpid = waitpid(-1, &bgstatus, WNOHANG);
		if(debug)
			printf("Status of bg: %d\nPID of bg: %d\n", bgstatus, bgpid);
		if(bgpid > 0) {
			printf("[%d] exited, status: %d\n", bgpid, bgstatus);
			ldel(&jobs, &bgpid, findpid);
		}
		
		printf("%s", prompt);
		/* read input */
		int i;
		for(i = 0; i < 256; i++){
			int c = fgetc(stdin);
			if(c == EOF || c == '\n'){
				in[i] = '\0';
				break;
			}
			in[i] = c;
		}
		
		if(debug)
			printf("%s\n", in);
		// Don't check empty commands
		if(strlen(in) == 0)
			continue;
		
		/* check for builtin */
		if(strcmp(in, "exit") == 0){
			run = false;
			continue;
		}else if(strcmp(in, "pid") == 0){
			printf("%d\n", getpid());

		}else if((in, "ppid") == 0){
			printf("%d\n", getppid());

		}else if(strcmp(in, "cd") == 0){
			char* buf;
			buf = getenv("HOME");
			if(buf == nil){
				printf("Error fetching $HOME. Is the variable set?\n");
				continue;
			}
			if(debug)
				printf("cd to: %s\n", buf);
			if(chdir(buf) < 0)
				printf("Error changing current working directory.\n");
			//free(buf);

		}else if(strncmp(in, "cd ", 3) == 0){
			char* buf = calloc(253, sizeof(char));
			strncpy(buf, in+3, 253);
			if(debug)
				printf("cd to: %s\n", buf);
			if(chdir(buf) < 0)
				printf("Error changing current working directory.\n");
			free(buf);

		}else if(strcmp(in, "pwd") == 0){
			char* buf = calloc(256, sizeof(char));
			if(buf == getcwd(buf, 256))
				printf("%s\n", buf);
			else
				printf("Error getting current working directory.\n");
			free(buf);

		}else if(strcmp(in, "jobs") == 0){
			Node* n = jobs.root;
			
			if(jobs.size == 0)
				printf("No jobs to print.\n");
			else if(jobs.size == 1){
				Proc* p = (Proc*)jobs.root->dat;
				printf("­\n");
				printf("> %d | %s\n", p->pid, p->name);
				printf("­\n");
			}else{
				printf("­\n");
				for(i = 1; i < jobs.size+1; i++){
					if(debug){
						Proc* p = (Proc*)jobs.root->dat;
						printf("Size of list: %d\nRoot info: %d ­ %s\n", jobs.size, p->pid, p->name);
					}
					Proc* p = (Proc*)n->dat;
					printf("> %d | %s\n", p->pid, p->name);
					n = n->next;
				}
				printf("­\n");
			}

		}else if(strncmp(in, "set", 3) == 0){
			// TODO -- use split here
			char* name = calloc(252, sizeof(char));
			char* value = calloc(252, sizeof(char));
			int valuePos = 0; //the array location of the first char of the value: 'set HOME xhome/seh'
			int err = 0;
			int clear = false;
			
			if((int)strlen(in) < 4){
				printf("Error. Please specify a variable to set.\n");
				free(name);
				free(value);
				continue;
			}
			
			for(i = 4; i < 256; i++){
				if(in[i] == ' '){
					valuePos = i+1;
					if(debug)
						printf("space found at: %d\n", i);
					break;
				}
				
				if(i == 255){
					printf("Error. Please  specify a variable.\n");
					err = 1;
					free(name);
					free(value);
					break;
				}
				// No value, so we clear the var
				if(in[i] == '\0'){
					if(debug)
						printf("null term at: %d\n", i);
					valuePos = i-2;
					clear = true;
					break;
				}
			}
			if(err > 0)
				continue;
			
			
			
			if(!clear){
				strncpy(name, in+4, valuePos-1-4);
				strncpy(value, in+valuePos, 255-valuePos);
				if(debug)
					printf("Value: %s\n", value);
				
				char* nstr = malloc(sizeof(name));
				strcpy(nstr, name);
			
				char* vstr = strcat(strcat(name, "="), value);
				err = setenv(nstr, vstr, 1);
				if(err > 0)
					printf("Error. No variable set!\n");
				
				if(debug)
					printf("Nstr: %s\nVstr: %s\n", nstr, vstr);
			
				free(vstr);
				free(nstr);
			} else {
				strncpy(name, in+4, valuePos-1);
				printf("Name: %s\n", name);
				unsetenv(name);
				free(name);
			}

			free(value);

		}else if(strncmp(in, "get", 3) == 0){
			// TODO -- use split here
			char* name = calloc(252, sizeof(char));
			char* value;
			if((int)strlen(in) < 4){
				printf("Error. Please specify a variable to get.\n");
				free(name);
				continue;
			}
			
			strncpy(name, in+4, 252);
			value = getenv(name);
			if(value == nil){
				printf("Error fetching %s. Is the variable set?\n", name);
				free(name);
				continue;
			}
			printf("%s\n", value);
			free(name);
			//free(value);

		}else{
			/* check for command */
			if(debug)
				printf("Len of in: %d\n", strlen(in));
			if(debug)
				printf("Searching for command…\n");
			

			// Check if we'll background the child
			int len = strlen(in);
			int bg = false;
			if(in[len-1] == '&'){
				bg = true;
				in[len-1] = '\0';
			}

			// Process command
			char* args[256];
			split(in, args);
			
			// Handle fork
			int pid;
			int status;
			pid = fork();
			if(pid < 0){
				printf("Error forking, are we out of PID\'s?\n");
				exit(1);
			}else if(pid == 0){
				// Child
				printf("[%d]\n", getpid());
				int err = execvp(*args, args);
				if(err < 0){
					printf("Error executing command, is the command in $PATH?\n");
					exit(2);
				}
			}else{
				// Parent
				if(!bg){
					waitpid(pid, &status, 0);
					// TODO -- be more verbose (man 2 wait)
					printf("[%d] exited, status: %d\n", pid, status);
				}else{
					Proc* p = malloc(sizeof(Proc));
					p->pid = pid;
					char cmd[256];
					strcpy(cmd, *args);
					p->name = cmd;
					ladd(&jobs, p);
				}
			}
			
		}
	}
	printf("Goodbye! ☺\n");
}

// Split a command + args to: char**{cmd, arg0, arg1, …, argn}
void
split(char* line, char** out)
{
	while(*line != '\0'){
		while(*line == ' ' || *line == '\n' || *line =='\t')
			*line++ = '\0';
		*out++ = line;
		while(*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
			line++;
	}
	*out = '\0';
}

// Comparator to match a given Proc type's PID to a given PID
int
findpid(void* vproc, void* vpid)
{
	Proc* proc = (Proc*)vproc;
	int* pid = (int*)vpid;
	if(proc->pid == *pid){
		return true;
	}
	return false;
}
