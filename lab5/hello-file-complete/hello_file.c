/**
 * @file      hello_file.c
 * @author    Sean Hinchee
 * @date      2015-03-08: Last updated
 * @brief     Character driver example
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

///--- Prototypes ---///
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

///--- Macros ---///
#define SUCCESS 0
#define DEVICE_NAME "cpre308"
/// The max length of the message from the device
#define BUF_LEN 80
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
 
///--- Global Variables ---///
static int Major;			// Major number assigned to this device driver
static int Device_Open = 0;	// is the device open already?
static char msg[BUF_LEN];	// The message the device will return
static char *msg_ptr;

///--- Register Callbacks ---///
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

///--- Init Module ---///
int __init device_init(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	
	if(Major < 0)
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");

	return SUCCESS;
}

///--- Cleanup Module ---///
void __exit device_exit(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
} 

///--- register init and exit functions ---///
module_init(device_init);
module_exit(device_exit);

///--- Device Open ---///
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;
	
	if (Device_Open)
		return -EBUSY;
	
	Device_Open++;
	sprintf(msg, "I already told you %d times Hello World!\n", counter++);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

///--- Device Release ---///
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

///--- Device Read ---///
static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;
	
	// if we are at the end of the message return 0 signifying end of file
	if (*msg_ptr == 0)
		return 0;

	while (length && *msg_ptr)
	{
		// the buffer is in the user data segment, not the kernel
		// segment so "*" assignment won't work.  We have to use
		// put_user which copies data from the kernel data segment
		// to the user data segment.
		put_user(*(msg_ptr++), buffer++);
		
		length--;
		bytes_read++;
	}
	
	return bytes_read;
}

///--- Device Write ---///
static ssize_t device_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
	int i, k;
	int end = MIN(len-1, BUF_LEN-1);
	for(i = 0, k = end; i < len && i < BUF_LEN && k >= 0; i++, k--){
		get_user(msg[k], buffer+i);
	}
	end = MIN(BUF_LEN-1, i);
	msg[end] = '\0';
	
	msg_ptr = msg;
	printk(KERN_INFO "CPRE308: %s", msg);
	return i;



	//sprintf(msg, "%s(%zu letters)", buffer, len);
	//printk(KERN_INFO "CPRE308: Device received %zu characters\n", len);
	//return len;
	
	//printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	//return -EINVAL;
}
















