/**
 * @file      shm_test1.c
 * @author    Jeramie Vens
 * @date      2015-02-24: Last updated
 * @brief     Shared memory space example
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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "shm_test.h"

int main(int argc, char** argv)
{
	int fd;
	fd = shm_open("/308LabIPC", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP); // open the shared memory area creating it if it doesn't exist
	if(!fd)
	{
		perror("shm_open\n");
		return -1;
	}
	if(ftruncate(fd, sizeof(struct SHM_SHARED_MEMORY)))
	{
		perror("ftruncate\n");
		return -1;
	}
	struct SHM_SHARED_MEMORY* shared_mem;
	shared_mem = mmap(NULL, sizeof(struct SHM_SHARED_MEMORY), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(!shared_mem)
	{
		perror("mmap\n");
		return -1;
	}
	if(close(fd))
	{
		perror("close\n");
		return -1;
	}
	int i;
	for(i = 0; i < 5; i++)
	{
		shared_mem->an_array[i] = i*i;
	}
	char my_string[] = "I am a string allocated on main's stack!";
	shared_mem->a_ptr = my_string;
	sleep(5);
	printf("a_string = \"%s\"\n", shared_mem->a_string);
	printf("an_array[] = {%d, %d, %d, %d, %d}\n", shared_mem->an_array[0], shared_mem->an_array[1], shared_mem->an_array[2], shared_mem->an_array[3], shared_mem->an_array[4]);
	if(shared_mem->a_ptr > 0)
	{
		printf("a_ptr = %lu = \"%s\"\n", shared_mem->a_ptr, shared_mem->a_ptr);
	}
	else
	{
		printf("a_ptr = NULL\n");
	}
}
