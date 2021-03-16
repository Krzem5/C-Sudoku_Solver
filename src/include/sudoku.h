#ifndef __SUDOKU_H__
#define __SUDOKU_H__
#include <stdint.h>
#include <limits.h>
#include <signal.h>



#define bool _Bool
#define false 0
#define true 1
#ifdef NULL
#undef NULL
#endif
#define NULL ((void*)0)
#define assert(x) \
	do{ \
		if (!(x)){ \
			printf("%s:%i (%s): %s: Assertion Failed\n",__FILE__,__LINE__,__func__,#x); \
			raise(SIGABRT); \
		} \
	} while (0)



uint8_t solve_sudoku(uint8_t* b);



#endif
