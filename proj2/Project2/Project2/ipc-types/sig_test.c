/**
 * @file      sig_test.c
 * @author    Jeramie Vens
 * @date      2015-02-24: Last updated
 * @brief     Signal example
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
#include <signal.h>

void my_quit_handler(int i)
{
	printf("\nAh Ah Ah, you didn't say the magic word\n");
	fflush(stdout);
}

int main(int argc, char** argv)
{
	signal(SIGINT, my_quit_handler);
	while(1)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	return 0;
}



