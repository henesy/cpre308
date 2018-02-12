#include <u.h>
#include <libc.h>
#include <bio.h>
#include "shell.h"

/* function prototype bloc */
void	split(char* line, char** out);
int	findpid(void* Node, void* pid);

/* shell is a novel shell written by seh for CPRE 308 section G */
void
main(int argc, char** argv)
{
	char* prompt = nil;
	int debug = false;
	List jobs = mklist();
	// This is standard out
	int fd;

	/* arg processing */
	ARGBEGIN {
		case 'p':
			prompt = ARGF();
			break;
		case 'd':
			debug = true;
			break;
		case 'h':
			print("usage: shell [-p prompt] [-d]\n");
		default:
			print("badflag('%c')\n", ARGC());
	} ARGEND


	if(prompt == nil)
		prompt = "308sh> ";

	int cons = open("/dev/cons", OWRITE);
	fd = cons;

	/* loop */
	int run = true;
	while(run){
		char in[256];
		//Waitmsg* bgm;
		int redir = -1;
		int outf = -1;
		Biobuf* bp = Bfdopen(0, OREAD);
		int i;
		if(fd != cons)
			close(fd);
		fd = cons;
		
		// Sleep to prevent output buffer being overrun by bgm->pid printing and prompt
		sleep(100);
		fprint(fd, "%s", prompt);

		/* read input */
		for(i = 0; i < 256; i++){
			Rune c = Bgetrune(bp);
			if(c == '\n'){
				in[i] = '\0';
				break;
			}
			if(c == '>'){
				if(debug)
					fprint(2, "Found redir at %d\n", i);
				redir = i;
			}
			in[i] = c;
		}
		
		if(debug)
			fprint(2, "In is: %s\n", in);
		
		/* check if , '\004' is EOT */
		if(in[0] == -1){
			run = false;
			continue;
		}
		
		/* check if we'll background the child */
		long len = 256.0;
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
				fprint(2, "redir to split: %s\n", full);
			split(full, fname);
			
			if(fname[1] != nil)
				outf = create(fname[1], ORDWR, 0666);
			if(outf == -1 && strlen(full) > 1){
				// Format used was >test
				outf = create(full+1, ORDWR, 0666);
				if(debug)
					fprint(2, "0 Attempted to open %s for write.\n", fname[0]+1);
				if(outf == -1){
					fprint(2, "Error opening %s for write. Did you specify a file?\n", full+1);
					free(full);
					continue;
				}
			}
			if(outf == -1){
				fprint(2, "Error opening %s for write. Did you specify a file?\n", fname[1]);
				free(full);
				continue;
			}
			
			in[redir-1] = '\0';
			fd = outf;
			free(full);
		}
		
		if(debug)
			fprint(2, "%s\n", in);
		// Don't check empty commands
		if(strlen(in) == 0)
			continue;
		
		/* check for builtin */
		if(strncmp(in, "exit", 4) == 0){
			run = false;
			continue;

		}else if(strncmp(in, "pid", 3) == 0){
			fprint(fd, "%d\n", getpid());

		}else if(strncmp(in, "ppid", 4) == 0){
			fprint(fd, "%d\n", getppid());

		}else if(strcmp(in, "cd") == 0){
			char* buf;
			buf = getenv("home");
			if(buf == nil){
				fprint(2, "Error fetching $home. Is the variable set?\n");
				continue;
			}
			if(debug)
				fprint(2, "cd to: %s\n", buf);
			if(chdir(buf) < 0)
				fprint(2, "Error changing current working directory.\n");

		}else if(strncmp(in, "cd ", 3) == 0){
			char* buf = calloc(253, sizeof(char));
			strncpy(buf, in+3, 253);
			if(debug)
				fprint(2, "cd to: %s\n", buf);
			if(chdir(buf) < 0)
				fprint(2, "Error changing current working directory.\n");
			free(buf);

		}else if(strncmp(in, "pwd", 3) == 0){
			char* buf = calloc(256, sizeof(char));
			if(buf == getwd(buf, 256))
				fprint(fd, "%s\n", buf);
			else
				fprint(2, "Error getting current working directory.\n");
			free(buf);

		}else if(strncmp(in, "jobs", 4) == 0){
			Node* n = jobs.root;
			
			if(jobs.size == 0)
				fprint(fd, "No jobs to print.\n");
			else if(jobs.size == 1){
				Proc* p = (Proc*)jobs.root->dat;
				fprint(fd, "­­­\n");
				fprint(fd, "%-10s | %10s\n", "PID", "Name");
				fprint(fd, "%-10d | %10s\n", p->pid, p->name);
				fprint(fd, "­­­\n");
			}else{
				fprint(fd, "­­­\n");
				fprint(fd, "%-10s | %10s\n", "PID", "Name");
				for(i = 1; i < jobs.size+1; i++){
					if(debug){
						Proc* p = (Proc*)jobs.root->dat;
						fprint(fd, "Size of list: %d\nRoot info: %d ­ %s\n", jobs.size, p->pid, p->name);
					}
					Proc* p = (Proc*)n->dat;
					fprint(fd, "%-10d | %10s\n", p->pid, p->name);
					n = n->next;
				}
				fprint(fd, "­­­\n");
			}

		}else if(strncmp(in, "set", 3) == 0){
			/* get and set are kind of a mess */
			char* full = calloc(256, sizeof(char));
			strcpy(full, in);
			char* out[256];
			split(full, out);
			if(debug)
				fprint(2, "in: %s ;; out: %s\n", in, out[1]);
			for(i = 1; i < 256; i++){
				if(out[i] == nil){
					if(debug)
						fprint(2, "No args at/past %d\n", i);
					break;
				}
				if(strlen(out[i]) < 1){
					break;
				}
			}
			int argloc = i;
			if(argloc < 2){
				fprint(2, "Error. Please specify a variable name.\n");
				free(full);
				continue;
			}
			
			char* name = out[1];
			char* value = calloc(252, sizeof(char));
			// The array location of the first char of the value: 'set home x/usr/seh'
			int valuePos = 0;
			int err = 0;
			int clear = false;
			if(argloc == 2)
				clear = true;
			
			if((int)strlen(in) < 4){
				fprint(2, "Error. Please specify a variable to set.\n");
				free(value);
				free(full);
				continue;
			}
			
			for(i = 4; i < 256; i++){
				if(in[i] == ' '){
					valuePos = i+1;
					if(debug)
						fprint(2, "space found at: %d\n", i);
					break;
				}
				
				if(i == 255){
					fprint(2, "Error. Please  specify a variable.\n");
					err = 1;
					free(value);
					break;
				}

				// No value, so we clear the var
				if(in[i] == '\0'){
					if(debug)
						fprint(2, "null term at: %d\n", i);
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
					fprint(2, "Value: %s\n", value);

				err = putenv(name, value);
				if(err < 0)
					fprint(2, "Error. No variable set!\n");
			
			} else {
				strncpy(name, in+4, valuePos-1);
				if(debug)
					fprint(2, "Name: %s\n", name);
				
				char envf[256];
				sprint(envf, "/env/%s", name);
				remove(envf);
			}

			free(value);
			free(full);

		}else if(strncmp(in, "get", 3) == 0){
			char* name = calloc(252, sizeof(char));
			char* value;
			if((int)strlen(in) < 4){
				fprint(2, "Error. Please specify a variable to get.\n");
				free(name);
				continue;
			}
			
			strncpy(name, in+4, 252);
			value = getenv(name);
			if(value == nil){
				if(name[0] == ' ' || name[0] == '\t')
					fprint(2, "Error. You can't make a variable name out of whitespace. Whitespace for variable naming is considered harmful.\n");
				else
					fprint(2, "Error fetching %s. Is the variable set?\n", name);
				free(name);
				continue;
			}
			
			fprint(fd, "%s\n", value);
			free(name);

		}else{
			/* check for command */
			if(debug)
				fprint(2, "Len of in: %ld\n", strlen(in));
			if(debug)
				fprint(2, "Searching for command…\n");

			// Process command
			char* args[256];
			split(in, args);
			
			// Handle fork
			int pid;
			pid = fork();
			if(pid < 0){
				fprint(2, "Error forking, are we out of PID\'s?\n");
				exits("Error forking.");
			}else if(pid == 0){
				// Child
				fprint(2, "[%d] %s\n", getpid(), *args);
				dup(fd, 1);

				if((*args)[0] == '/')
					goto NOSRCH;
				
				// Search $path
				char* path[256];
				char* opath;
				int err;

				opath = getenv("path");
				if(path == nil){
					fprint(2, "Error fetching $path. Is the variable set?\n");
				}
				split(opath, path);
				
				int j;
				for(j = 0; j < 256; j++){
					if(path[j] == nil){
						fprint(2, "Command not in $path. Provide full path.\n");
						break;
					}
					
					// lsdot code courtesy of nemo
					Dir* dents;
					int ndents, fd, i;
					
					fd = open(path[j], OREAD);
					if (fd < 0)
						sysfatal("Error opening dir: %r");
					for(;;){
						ndents = dirread(fd, &dents);
						if (ndents == 0)
							break;
						for (i = 0; i < ndents; i++){
							if(strcmp(dents[i].name, *args) == 0){
								// Found command
								err = exec(strcat(path[j], strcat("/", *args)), args);
								free(dents);
								goto PATHD;
							}
						}
						free(dents);
					}
				}
				NOSRCH:;
				

				err = exec(*args, args);
				PATHD:;
				if(err < 0){
					exits("Error executing command.");
				}
			}else{
				// Parent
				if(!bg){
					// rendezvous(2)?
					Waitmsg* m;

					while(m = wait()){
						fprint(2, "[%d] %s\n", m->pid, m->msg);
						if (m->pid == pid){
							free(m);
							break;
						}
						free(m);
					}
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
	fprint(2, "Goodbye! ☺\n");
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
