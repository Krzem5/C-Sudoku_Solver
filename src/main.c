#include <sudoku.h>
#include <stdio.h>
#include <windows.h>



void print_board(uint8_t* b){
	printf("â•­â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â•®\nâ”‚ ");
	for (uint8_t i=0;i<81;i++){
		if (*(b+i)==0){
			printf("Â·");
		}
		else{
			putchar(48+(*(b+i)));
		}
		if (i%9==8){
			printf(" â”‚\n");
			if (i!=80){
				if (i%27==26){
					printf("â”œâ”€â”€â”€â”€â”€â”€â”€â”¼â”€â”€â”€â”€â”€â”€â”€â”¼â”€â”€â”€â”€â”€â”€â”€â”¤\nâ”‚ ");
				}
				else{
					printf("â”‚ ");
				}
			}
			else{
				printf("â•°â”€â”€â”€â”€â”€â”€â”€â”´â”€â”€â”€â”€â”€â”€â”€â”´â”€â”€â”€â”€â”€â”€â”€â•¯\n");
			}
		}
		else if (i%3==2){
			printf(" â”‚ ");
		}
		else{
			putchar(' ');
		}
	}
}



int main(int argc,const char** argv){
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleMode(GetStdHandle(-11),7);
	SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
	LARGE_INTEGER f;
	LARGE_INTEGER s;
	LARGE_INTEGER e;
	QueryPerformanceFrequency(&f);
	uint8_t b[]={5,1,7,6,0,0,0,3,4,2,8,9,0,0,4,0,0,0,3,4,6,2,0,5,0,9,0,6,0,2,0,0,0,0,1,0,0,3,8,0,0,6,0,4,7,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,7,8,7,0,3,4,0,0,5,6,0,0,0,0,0,0,0,0,0,0};
	// uint8_t b[]={0,0,0,0,0,0,0,1,2,0,0,0,0,3,5,0,0,0,0,0,0,6,0,0,0,7,0,7,0,0,0,0,0,3,0,0,0,0,0,4,0,0,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,8,0,0,0,0,0,4,0,0,5,0,0,0,0,6,0,0};
	print_board(b);
	QueryPerformanceCounter(&s);
	solve_sudoku(b);
	QueryPerformanceCounter(&e);
	putchar('\n');
	print_board(b);
	printf("Time: %.7fs\n",(e.QuadPart-s.QuadPart)*1e6/f.QuadPart*1e-6);
	return 0;
}
