

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "printer_driver.h"


int printer_install(struct printer_driver * printer, const char * driver)
{
	int i = 0;
	char driver_name[500];
	char line[1024];
	snprintf(driver_name, 500, "%s-r", driver);
	printer->driver_write = fopen(driver_name, "w");
	if(printer->driver_write == NULL)
	{
		eprintf("Failed to open printer driver %s\n", driver);
		return -1;
	}

	snprintf(driver_name, 500, "%s-w", driver);
	printer->driver_read = fopen(driver_name, "r");
	if(printer->driver_read == NULL)
	{
		eprintf("Failed to open printer driver %s\n", driver);
		return -1;
	}

	fprintf(printer->driver_write, "##NAME##\n");
	fflush(printer->driver_write);
	printer->name = NULL;
	fgets(line, 1024, printer->driver_read);
	i = strlen(line);
	line[i - 1] = '\0';
	printer->name = strdup(line);

	fprintf(printer->driver_write, "##DESCRIPTION##\n");
	fflush(printer->driver_write);
	printer->description = NULL;
	fgets(line, 1024, printer->driver_read);
	i = strlen(line);
	line[i - 1] = '\0';
	printer->description = strdup(line);

	fprintf(printer->driver_write, "##LOCATION##\n");
	fflush(printer->driver_write);
	printer->location = NULL;
	fgets(line, 1024, printer->driver_read);
	i = strlen(line);
	line[i - 1] = '\0';
	printer->location = strdup(line);

	dprintf("Installed Printer:\n"
					"\tDriver: %s\n"
					"\tName: %s\n"
					"\tDescription: %s\n"
					"\tLocation: %s\n",
					driver, printer->name, printer->description, printer->location);
	
	return 0;
}

int printer_uninstall(struct printer_driver * printer)
{
	free(printer->name);
	free(printer->description);
	free(printer->location);
	fclose(printer->driver_write);
	fclose(printer->driver_read);
	memset(printer, 0, sizeof(struct printer_driver));
	return 0;
}

int printer_print(const struct printer_driver * printer, const struct print_job * job)
{
	char buffer[1024];
	FILE* ps = fopen(job->file_name, "r");
	if(!ps)
	{
		eprintf("Failed to open print job file %s\n", job->file_name);
		return -1;
	}
	
	fprintf(printer->driver_write, "##NAME: %s##\n", job->job_name);
	while(fgets(buffer, 1024, ps))
	{
		fputs(buffer, printer->driver_write);
	}
	fprintf(printer->driver_write, "##END##\n");
	fflush(printer->driver_write);
	
	return 0;
}

