/**
 * @file      print_server_client.h
 * @author    Jeramie Vens
 * @date      2015-02-23
 * @date      2015-02-23: Last updated
 * @brief     The public API of libprinter.so library
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

#ifndef PRINT_SERVER_CLIENT_H
#define PRINT_SERVER_CLIENT_H


typedef struct PRINTER_DRIVER_STRUCT printer_driver_t;
/// A printer driver returned by printer_list_drivers()
struct PRINTER_DRIVER_STRUCT
{
	/// The name of the printer
	char* printer_name;
	/// The driver name
	char* driver_name;
	/// The driver version
	char* driver_version;
};

/**
 * @brief     Send a print job to the print server daemon program.
 * @details   This function should send the given job to the print server program using
 *            your chosen method of IPC.
 * @param     handle
 *                 You may optionally implement this for extra credit.  If used this should return
 *                 a unique number that represents this print job and can be used to get information
 *                 about the job with other function calls.  If you choose not to implement this
 *                 you can safely ignore this param.
 * @param     driver
 *                 The name of the driver to print the job to.  Required.
 * @param     job_name
 *                 A name given to the job.  Required
 * @param     description
 *                 A discription of the job.  Optional, must handel being set to NULL if no
 *                 description is given.
 * @param     data
 *                 The actual print job to be printed in Postscript format.  Must start
 *                 with a "%" sign and end with "%EOF".
 * @return    This function should return 0 if the print server accepts the job as being valid.
 *            Note, this should return as soon as the print server accepts the job, but it should
 *            NOT wait for the server to finish printing the job.  It should return a number < 0
 *            on error.
 */
int printer_print(int* handle, char* driver, char* job_name, char* description, char* data);

/**
 * @brief     List the currently installed printer drivers from the print server
 * @details   This function should query the print server for a list of currently installed drivers
 *            and return them as a NULL terminated array of printer_driver_t objects.
 * @param     number
 *                 Returns the number of printer drivers currently installed in the print server daemon
 * @return    An array of number printer_driver_t* objects followed by NULL
 * @example
 *
 * int num;
 * printer_driver* list[] = printer_list_driver(&num);
 * printf("printer_name=%s", list[0]->printer_name);
 *
 */
printer_driver_t** printer_list_drivers(int *number);


// Optional additional functions you may choose to implement for extra credit.
#if 0

/**
 * @brief     Install a new driver into the print server daemon.
 * @details   This function is used to hot-plug a new printer driver into the daemon at run time.
 * @param     driver_location
 *                 The file location of the printer driver (this will be in lab 7)
 * @param     driver
 *                 Details about the name and description of the driver.
 * @return    0 if successful, < 0 if something goes wrong.
 */
int printer_install_driver(char* driver_location, printer_driver_t driver);

/**
 * @brief     Uninstall a currently installed printer driver.
 * @param     driver
 *                 The driver to uninstall
 * @return    0 if successful, < 0 if something goes wrong
 */
int printer_uninstall_driver(printer_driver_t driver);

/**
 * @brief     Determine if a print job has finished yet
 * @param     handle
 *                 The handle to the print job returned by printer_print() function
 * @return    1 if the job has finished, 0 if it has not finished, and < 0 is something goes wrong
 */
int printer_is_finished(int handle);

/**
 * @brief     Wait for a print job to finish printing before continuing
 * @param     handle
 *                 The handle to the print job returned by printer_print() function
 * @return    0 if successful, < 0 if something goes wrong
 */
int printer_wait(int handle)
 
/**
 * @brief     Cancel an already submitted job if it has not already been sent to the printer.
 * @param     handle
 *                 The handle to the print job returned by printer_print() function
 * @return    0 if the job was successfully canceled, > 1 if the job has already printed, 
 *            < 0 if something goes wrong
 */
int printer_cancel_job(int handle);

/**
 * @brief     Pause an already submitted job if it has not already been sent to the printer.
 * @param     handle
 *                 The handle to the print job returned by printer_print() function
 * @return    0 if the job was successfully paused, > 1 if the job has already printed,
 *            < 0 if something goes wrong.
 */
int printer_pause_job(int handle);

/**
 * @brief     Resume printing a job that was paused
 * @param     handle
 *                 The handle to the print job returned by printer_print() function
 * @return    0 if the job was successfully resumed, < 0 if something goes wrong
 */
int printer_resume_job(int handle);

*/
#endif

#endif
