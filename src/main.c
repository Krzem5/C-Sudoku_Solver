#include <sudoku.h>
#include <stdio.h>
#include <windows.h>



void print_board(uint8_t* b){
	for (uint8_t i=0;i<81;i++){
		putchar((*(b+i)==0?'.':48+(*(b+i))));
		putchar((i%9==8?'\n':' '));
	}
}



int main(int argc,const char** argv){
	// uint8_t b[]={5,1,7,6,0,0,0,3,4,2,8,9,0,0,4,0,0,0,3,4,6,2,0,5,0,9,0,6,0,2,0,0,0,0,1,0,0,3,8,0,0,6,0,4,7,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,7,8,7,0,3,4,0,0,5,6,0,0,0,0,0,0,0,0,0,0};
	uint8_t b[]={0,0,0,0,0,0,0,1,2,0,0,0,0,3,5,0,0,0,0,0,0,6,0,0,0,7,0,7,0,0,0,0,0,3,0,0,0,0,0,4,0,0,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,8,0,0,0,0,0,4,0,0,5,0,0,0,0,6,0,0};
	print_board(b);
	LARGE_INTEGER s;
	QueryPerformanceCounter(&s);
	solve_sudoku(b);
	LARGE_INTEGER e;
	QueryPerformanceCounter(&e);
	putchar('\n');
	print_board(b);
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	printf("Time: %.7fs\n",(e.QuadPart-s.QuadPart)*1e6/f.QuadPart*1e-6);
	return 0;
}
