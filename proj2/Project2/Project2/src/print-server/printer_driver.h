/**
 * @file      printer.h
 * @author    Jeramie Vens
 * @date      2015-02-13: Created
 * @date      2015-02-15: Last updated
 * @date      2016-02-16: Complete re-write of the code
 * @brief     A virtual printer for the print sever program
 * @copyright MIT License (c) 2015, 2016
 */
 
/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/

#ifndef PRINTER_H
#define PRINTER_H

#include "print_job.h"

#ifdef __cplusplus
extern "C" {
#endif 


/**
 * A printer driver object
 */
struct printer_driver
{
	// the name of the printer
	char * name;
	// the description of the printer
	char * description;
	// the location of the printer
	char * location;
	// the write endpoint to talk to the driver
	FILE * driver_write;
	// the read endpoint to query the driver
	FILE * driver_read;
};

// install a new driver
int printer_install(struct printer_driver * printer, const char * driver);
// uninstall the given driver
int printer_uninstall(struct printer_driver * printer);
// send a print job to the driver
int printer_print(const struct printer_driver * printer, const struct print_job * job);

#ifdef __cplusplus
}
#endif

#endif

