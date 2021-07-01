#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <time.h>
#endif
#include <sudoku.h>
#include <stdio.h>



#define USE_HARD_SUDOKU 0



void print_board(uint8_t* b){
	printf("╭───────┬───────┬───────╮\n│ ");
	for (uint8_t i=0;i<81;i++){
		if (*(b+i)==0){
			printf("·");
		}
		else{
			putchar(48+(*(b+i)));
		}
		if (i%9==8){
			printf(" │\n");
			if (i!=80){
				if (i%27==26){
					printf("├───────┼───────┼───────┤\n│ ");
				}
				else{
					printf("│ ");
				}
			}
			else{
				printf("╰───────┴───────┴───────╯\n");
			}
		}
		else if (i%3==2){
			printf(" │ ");
		}
		else{
			putchar(' ');
		}
	}
}



int main(int argc,const char** argv){
#if USE_HARD_SUDOKU
	uint8_t b[81]={0,0,0,0,0,0,0,1,2,0,0,0,0,3,5,0,0,0,0,0,0,6,0,0,0,7,0,7,0,0,0,0,0,3,0,0,0,0,0,4,0,0,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,8,0,0,0,0,0,4,0,0,5,0,0,0,0,6,0,0};
#else
	uint8_t b[81]={0,6,9,0,5,0,3,0,0,0,8,1,0,9,3,0,0,5,0,0,5,4,8,0,0,1,0,9,2,6,0,0,0,7,0,8,0,5,0,0,0,0,0,4,9,0,0,0,0,0,9,6,0,1,0,0,4,0,3,8,0,2,7,0,0,0,0,4,5,0,0,0,5,1,0,2,7,6,8,0,4};
#endif
	print_board(b);
#ifdef _MSC_VER
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleMode(GetStdHandle(-11),7);
	HANDLE p=GetCurrentProcess();
	SetPriorityClass(p,HIGH_PRIORITY_CLASS);
	LARGE_INTEGER f;
	LARGE_INTEGER s;
	LARGE_INTEGER e;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&s);
#else
	struct timespec s;
	struct timespec e;
	clock_gettime(CLOCK_REALTIME,&s);
#endif
	uint8_t o=solve_sudoku(b);
#ifdef _MSC_VER
	QueryPerformanceCounter(&e);
	double dt=(e.QuadPart-s.QuadPart)*1e6/f.QuadPart*1e-6;
#else
	clock_gettime(CLOCK_REALTIME,&e);
	double dt=e.tv_sec-s.tv_sec+(e.tv_nsec-s.tv_nsec)*1e-9;
#endif
	putchar('\n');
	if (o){
		print_board(b);
	}
	else{
		printf("Failed to Solve Sudoku!\n");
	}
	printf("Time: %.6fs\n",dt);
	return 0;
}
