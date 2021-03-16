#include <sudoku.h>
#include <windows.h>
#include <stdio.h>



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
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleMode(GetStdHandle(-11),7);
	SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
	LARGE_INTEGER f;
	LARGE_INTEGER s;
	LARGE_INTEGER e;
	QueryPerformanceFrequency(&f);
	// uint8_t b[81]={5,1,7,6,0,0,0,3,4,2,8,9,0,0,4,0,0,0,3,4,6,2,0,5,0,9,0,6,0,2,0,0,0,0,1,0,0,3,8,0,0,6,0,4,7,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,7,8,7,0,3,4,0,0,5,6,0,0,0,0,0,0,0,0,0,0};
	uint8_t b[81]={0,0,0,0,0,0,0,1,2,0,0,0,0,3,5,0,0,0,0,0,0,6,0,0,0,7,0,7,0,0,0,0,0,3,0,0,0,0,0,4,0,0,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,8,0,0,0,0,0,4,0,0,5,0,0,0,0,6,0,0};
	print_board(b);
	QueryPerformanceCounter(&s);
	uint8_t o=solve_sudoku(b);
	QueryPerformanceCounter(&e);
	putchar('\n');
	if (o){
		print_board(b);
	}
	else{
		printf("Failed to solve Sudoku!\n");
	}
	printf("Time: %.6fs\n",(e.QuadPart-s.QuadPart)*1e6/f.QuadPart*1e-6);
	return 0;
}
