#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"

/* function prototype bloc */
void	split(char* line, char** out);
int	findpid(void* Node, void* pid);
void	evalstatus(int pid, int status, char*);

/* shell is a novel shell written by seh for CPRE 308 section G */
void
main(int argc, char** argv)
{
	char* prompt = nil;
	int debug = false;
	List jobs = mklist();

	/* arg processing */
	int arg;
	while((arg = getopt(argc, argv, "dhp:")) != -1){
		switch(arg){
		case 'p': 
			prompt = optarg;
			break;
		case 'd':
			debug = true;
			break;
		case 'h':
			fprintf(stdout, "usage: shell [-p prompt] [-d]\n");
		default:
			exit(1);
		}
	}

	if(prompt == nil)
		prompt = "308sh> ";
	
	int truestdout = dup(1);

	/* loop */
	int run = true;
	while(run){
		char in[256];
		int bgstatus = -1;
		int bgpid = -1;
		int redir = -1;
		FILE* outf = nil;
		
		dup2(truestdout, STDOUT_FILENO);
		
		// Check background process status (if any)
		bgpid = waitpid(-1, &bgstatus, WNOHANG);
		if(debug)
			fprintf(stderr, "Status of bg: %d\nPID of bg: %d\n", bgstatus, bgpid);
		if(bgpid > 0) {
			//fprintf(stderr, "[%d] ended, status: %d\n", bgpid, bgstatus);
			char* bgname = calloc(256, sizeof(char));
			Node* n = jobs.root;
			int i;
			for(i = 0; i < jobs.size; i++){
				Proc* p = (Proc*)n->dat;
				if(p->pid == bgpid){
					strcpy(bgname, p->name);
					break;
				}
				n = n->next;
			}
			fprintf(stderr, "Background Child End: ");
			evalstatus(bgpid, bgstatus, bgname);
			ldel(&jobs, &bgpid, findpid);
			free(bgname);
		}
		
		// Sleep to prevent output buffer being overrun by bgpid printing and prompt
		usleep(100);
		fprintf(stdout, "%s", prompt);

		/* read input */
		int i;
		for(i = 0; i < 256; i++){
			int c = fgetc(stdin);
			if(c == EOF || c == '\n'){
				in[i] = '\0';
				break;
			}
			if(c == '>'){
				if(debug)
					fprintf(stderr, "Found redir at %d\n", i);
				redir = i;
			}
			in[i] = c;
		}
		
		/* check if we'll background the child */
		int len = strlen(in);
		int bg = false;
		if(in[len-1] == '&'){
			bg = true;
			in[len-2] = '\0';
		}
		
		/* check for > redirection (this is hardcoded and bad, but works) */
		if(redir > 0){
			char* full = calloc(256, sizeof(char));
			strcpy(full, in+redir);
			char* fname[256];
			if(debug)
				fprintf(stderr, "redir to split: %s\n", full);
			split(full, fname);
			
			if(fname[1] != nil)
				outf = fopen(fname[1], "w+");
			if(outf == nil && strlen(full) > 1){
				// Format used was >test
				outf = fopen(full+1, "w+");
				if(debug)
					fprintf(stderr, "0 Attempted to open %s for write.\n", fname[0]+1);
				if(outf == nil){
					fprintf(stderr, "Error opening %s for write. Did you specify a file?\n", full+1);
					free(full);
					continue;
				}
			}
			if(outf == nil){
				fprintf(stderr, "Error opening %s for write. Did you specify a file?\n", fname[1]);
				free(full);
				continue;
			}
			
			in[redir-1] = '\0';
			dup2(fileno(outf), STDOUT_FILENO);
			fclose(outf);
			free(full);
		}
		
		if(debug)
			fprintf(stderr, "%s\n", in);
		// Don't check empty commands
		if(strlen(in) == 0)
			continue;
		
		/* check for builtin */
		if(strncmp(in, "exit", 4) == 0){
			run = false;
			continue;

		}else if(strncmp(in, "pid", 3) == 0){
			fprintf(stdout, "%d\n", getpid());

		}else if(strncmp(in, "ppid", 4) == 0){
			fprintf(stdout, "%d\n", getppid());

		}else if(strcmp(in, "cd") == 0){
			char* buf;
			buf = getenv("HOME");
			if(buf == nil){
				fprintf(stderr, "Error fetching $HOME. Is the variable set?\n");
				continue;
			}
			if(debug)
				fprintf(stderr, "cd to: %s\n", buf);
			if(chdir(buf) < 0)
				fprintf(stderr, "Error changing current working directory.\n");

		}else if(strncmp(in, "cd ", 3) == 0){
			char* buf = calloc(253, sizeof(char));
			strncpy(buf, in+3, 253);
			if(debug)
				fprintf(stderr, "cd to: %s\n", buf);
			if(chdir(buf) < 0)
				fprintf(stderr, "Error changing current working directory.\n");
			free(buf);

		}else if(strncmp(in, "pwd", 3) == 0){
			char* buf = calloc(256, sizeof(char));
			if(buf == getcwd(buf, 256))
				fprintf(stdout, "%s\n", buf);
			else
				fprintf(stderr, "Error getting current working directory.\n");
			free(buf);

		}else if(strncmp(in, "jobs", 4) == 0){
			Node* n = jobs.root;
			
			if(jobs.size == 0)
				fprintf(stdout, "No jobs to print.\n");
			else if(jobs.size == 1){
				Proc* p = (Proc*)jobs.root->dat;
				fprintf(stdout, "­­­\n");
				fprintf(stdout, "%-10s | %10s\n", "PID", "Name");
				fprintf(stdout, "%-10d | %10s\n", p->pid, p->name);
				fprintf(stdout, "­­­\n");
			}else{
				fprintf(stdout, "­­­\n");
				fprintf(stdout, "%-10s | %10s\n", "PID", "Name");
				for(i = 1; i < jobs.size+1; i++){
					if(debug){
						Proc* p = (Proc*)jobs.root->dat;
						fprintf(stdout, "Size of list: %d\nRoot info: %d ­ %s\n", jobs.size, p->pid, p->name);
					}
					Proc* p = (Proc*)n->dat;
					fprintf(stdout, "%-10d | %10s\n", p->pid, p->name);
					n = n->next;
				}
				fprintf(stdout, "­­­\n");
			}

		}else if(strncmp(in, "set", 3) == 0){
			/* get and set are kind of a mess */
			char* full = calloc(256, sizeof(char));
			strcpy(full, in);
			char* out[256];
			split(full, out);
			if(debug)
				fprintf(stderr, "in: %s ;; out: %s\n", in, out[1]);
			for(i = 1; i < 256; i++){
				if(out[i] == nil){
					if(debug)
						fprintf(stderr, "No args at/past %d\n", i);
					break;
				}
				if(strlen(out[i]) < 1){
					break;
				}
			}
			int argloc = i;
			if(argloc < 2){
				fprintf(stderr, "Error. Please specify a variable name.\n");
				free(full);
				continue;
			}
			
			char* name = out[1];
			char* value = calloc(252, sizeof(char));
			// The array location of the first char of the value: 'set HOME xhome/seh'
			int valuePos = 0;
			int err = 0;
			int clear = false;
			if(argloc == 2)
				clear = true;
			
			if((int)strlen(in) < 4){
				fprintf(stderr, "Error. Please specify a variable to set.\n");
				free(value);
				free(full);
				continue;
			}
			
			for(i = 4; i < 256; i++){
				if(in[i] == ' '){
					valuePos = i+1;
					if(debug)
						fprintf(stderr, "space found at: %d\n", i);
					break;
				}
				
				if(i == 255){
					fprintf(stderr, "Error. Please  specify a variable.\n");
					err = 1;
					free(value);
					break;
				}

				// No value, so we clear the var
				if(in[i] == '\0'){
					if(debug)
						fprintf(stderr, "null term at: %d\n", i);
					valuePos = i-2;
					clear = true;
					break;
				}
			}

			if(err > 0){
				free(full);
				continue;
			}
				
			if(!clear){
				strncpy(value, in+valuePos, 255-valuePos);
				if(debug)
					fprintf(stderr, "Value: %s\n", value);
				
				char* nstr = malloc(sizeof(name));
				strcpy(nstr, name);
			
				char* vstr = strcat(strcat(name, "="), value);
				err = setenv(nstr, vstr, 1);
				if(err > 0)
					fprintf(stderr, "Error. No variable set!\n");
				
				if(debug)
					fprintf(stderr, "Nstr: %s\nVstr: %s\n", nstr, vstr);
			
				free(nstr);
			} else {
				strncpy(name, in+4, valuePos-1);
				if(debug)
					fprintf(stderr, "Name: %s\n", name);
				unsetenv(name);
			}

			free(value);
			free(full);

		}else if(strncmp(in, "get", 3) == 0){
			char* name = calloc(252, sizeof(char));
			char* value;
			if((int)strlen(in) < 4){
				fprintf(stderr, "Error. Please specify a variable to get.\n");
				free(name);
				continue;
			}
			
			strncpy(name, in+4, 252);
			value = getenv(name);
			if(value == nil){
				if(name[0] == ' ' || name[0] == '\t')
					fprintf(stderr, "Error. You can't make a variable name out of whitespace. Whitespace for variable naming is considered harmful.\n");
				else
					fprintf(stderr, "Error fetching %s. Is the variable set?\n", name);
				free(name);
				continue;
			}
			
			// Locate if Name=Val tuple format ;; first validate Name= left hand side
			char lefthand[256];
			strcpy(lefthand, name);
			int leftlen = strlen(name)+1;
			lefthand[strlen(name)] = '=';
			lefthand[strlen(name)+1] = '\0';
			
			int matches = true;
			for(i = 0; i < leftlen && i < strlen(value); i++){
				if(debug)
					fprintf(stderr, "left: %c\nvalue: %d\n", lefthand[i], value[i]);
				if(lefthand[i] != value[i]){
					matches = false;
					break;
				}
			}
			
			if(matches)
				fprintf(stdout, "%s\n", value+i);
			else
				fprintf(stdout, "%s\n", value);
			free(name);

		}else{
			/* check for command */
			if(debug)
				fprintf(stderr, "Len of in: %d\n", strlen(in));
			if(debug)
				fprintf(stderr, "Searching for command…\n");

			// Process command
			char* args[256];
			split(in, args);
			
			// Handle fork
			int pid;
			int status;
			pid = fork();
			if(pid < 0){
				fprintf(stderr, "Error forking, are we out of PID\'s?\n");
				exit(1);
			}else if(pid == 0){
				// Child
				fprintf(stderr, "[%d] %s\n", getpid(), *args);
				int err = execvp(*args, args);
				if(err < 0){
					fprintf(stderr, "Error executing command, is the command in $PATH?\n");
					exit(2);
				}
			}else{
				// Parent
				if(!bg){
					waitpid(pid, &status, 0);
					evalstatus(pid, status, *args);
				}else{
					// Add bg child to jobs list
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
	fprintf(stderr, "Goodbye! ☺\n");
}

// Split a command + args to: char**{cmd, arg0, arg1, …, argn} ;; empty slots are nil
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

// Evaluate and print details about a given status
void
evalstatus(int pid, int status, char* name)
{
	// Stopped and Continued aren't actually used by the shell, but exist for posterity
	if(WIFEXITED(status)){
		fprintf(stderr, "[%d] %s exited, status: %d\n", pid, name, WEXITSTATUS(status));
	}else if (WIFSIGNALED(status)){
		fprintf(stderr, "[%d] %s killed, signal: %d\n", pid, name, WTERMSIG(status));
	}else if (WIFSTOPPED(status)){
		fprintf(stderr, "[%d] %s stopped, signal %d\n", pid, name, WSTOPSIG(status));
	}else if (WIFCONTINUED(status)){
		fprintf(stderr, "[%d] %s continued, status %d\n", pid, name, status);
	}
}
