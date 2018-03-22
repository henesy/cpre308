


#ifndef DEBUG_H
#define DEBUG_H

#include "bash_color.h"

extern int verbose_flag;

#define dprintf(fmt, ...)														\
	do{																			\
		if(verbose_flag){														\
				flockfile(stderr);												\
				fprintf(stderr, BASH_FORE_BLUE fmt BASH_RESET, ##__VA_ARGS__);	\
				funlockfile(stderr);											\
		}																		\
	}while(0);

#define eprintf(fmt, ...)														\
	do{																			\
		flockfile(stderr);														\
		fprintf(stderr, BASH_FORE_RED fmt BASH_RESET, ##__VA_ARGS__);			\
		funlockfile(stderr);													\
	}while(0);


#endif
