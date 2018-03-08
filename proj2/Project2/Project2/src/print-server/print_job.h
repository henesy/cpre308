/**
 * @file      print_job.h
 * @author    Jeramie Vens
 * @date      2015-02-14: Created
 * @date      2015-02-14: Last updated
 * @brief     A print job which can be passed to a printer by the server
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


#ifndef PRINT_JOB_H
#define PRINT_JOB_H

#ifdef __cplusplus
extern "C" {
#endif 


struct print_job
{
	struct print_job * next_job;
	char* file_name;
	char* job_name;
	char* description;
	char* group_name;
	time_t arrival_time;
	time_t finish_time;
	long long job_number;
};


/// A print job object
//typedef struct PRINT_JOB_STRUCT print_job_t;
/// A print job object
//struct PRINT_JOB_STRUCT
//{
	/// File stream of the tmp file for the print job
//	FILE* file;
	/// The name of the tmp file
//	char input_file_name[50];
	/// The name of the output file
//	char* output_file_name;
	/// The name of the print driver that should be used
//	char* driver_name;
	/// The name of this print job, used for the log file
//	char* job_name;
	/// The time this print job arrived, used for the log file
//	time_t arrival_time;
	/// The description of this print job, used for the log file
//	char* description;
	/// A unique number for this print job set by the create funtion
//	int number;
//};



// prototypes
//print_job_t*       print_job_create    (void)              __attribute__ ((malloc));
//print_job_t*       print_job_destroy   (print_job_t *job)  __attribute__ ((warn_unused_result));
//char*              print_job_tostring  (print_job_t* job)  __attribute__ ((warn_unused_result,malloc));

#ifdef __cplusplus
}
#endif 

#endif

