/**
 * @file      hello_world.c
 * @author    Sean Hinchee
 * @date      2015-03-08: Last updated
 * @brief     Simple driver example
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


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Sean Hinchee seh@iastate.edu"
#define DRIVER_DESC   "A simple cat driver"

int __init hello_init(void)
{
	printk(KERN_INFO "we [people of #cat-v] should get renamed to cat --warning_level verbose\n");
	return 0;
}

void __exit hello_exit(void)
{
	printk(KERN_INFO "I can no longer sit back and allow NIX infiltration, NIX indoctrination, NIXsubversion and the international NIX conspiracy to sap and impurify all of ourprecious cats.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");

