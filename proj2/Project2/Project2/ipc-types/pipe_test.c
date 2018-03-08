/**
 * @file      pipe_test.c
 * @author    Jeramie Vens
 * @date      2015-02-24: Last updated
 * @brief     pipe example program
 * @copyright MIT License (c) 2015
 */
 
/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

int main(int argc, char** argv)
{
	int my_pipe[2];
	if(pipe(my_pipe))
	{
		perror("Failed to create pipe\n");
		return -1;
	}
	
	pid_t child = fork();
	if(child == 0)
	{
		// this is the child
		close(my_pipe[PIPE_READ_END]);
		FILE* out = fdopen(my_pipe[PIPE_WRITE_END], "w"); // open up as a file stream
		sleep(2); // sleep for a little bit
		fprintf(out, "Are you my mummy?"); // print something
		return 42;
	}
	else if(child > 0)
	{
		// this is the parent
		close(my_pipe[PIPE_WRITE_END]);
		FILE* in = fdopen(my_pipe[PIPE_READ_END], "r"); // open up as a file stream
		char buffer[100];
		fgets(buffer, 100, in);
		printf("My child asked \"%s\"\n", buffer);
		int status;
		wait(&status);
		printf("And then returned %d\n", WEXITSTATUS(status));
	}
	else
	{
		perror("Failed to fork\n");
		return -1;
	}
	return 0;
}

