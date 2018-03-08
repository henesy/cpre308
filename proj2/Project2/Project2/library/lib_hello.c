/**
 * @file      lib_hello.c
 * @author    Jeramie Vens
 * @date      2015-02-24: Last updated
 * @brief     Library example source
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

#include "lib_hello.h"


void print_hello(void)
{
	printf("Hello\n");
}

int print_world(int num)
{
	int i;
	for(i=0;i<num;i++)
	{
		printf("World\n");
	}
	return 42;
}

