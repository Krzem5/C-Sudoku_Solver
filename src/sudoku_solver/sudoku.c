#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <stdint.h>
#include <string.h>



#ifdef _MSC_VER
#pragma intrinsic(__popcnt16)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanForward64)
#define POPCOUNT16(x) __popcnt16((x))
static inline unsigned int FIND_FIRST_SET_BIT(unsigned int m){
	unsigned int out;
	_BitScanForward(&out,m);
	return out;
}
static inline unsigned int FIND_FIRST_SET_BIT64(unsigned int m){
	unsigned int out;
	_BitScanForward64(&out,m);
	return out;
}
#else
#define POPCOUNT16(x) __builtin_popcount(x)
#define FIND_FIRST_SET_BIT(m) (__builtin_ffs(m)-1)
#define FIND_FIRST_SET_BIT64(m) (__builtin_ffsll(m)-1)
#endif



typedef struct __SOLVE_BOARD{
	uint64_t z64;
	uint32_t z32;
	uint16_t dt[27];
} solve_board_t;



static const uint8_t _div_9_table[81]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8};// i/9
static const uint8_t _mod_9_plus_9_table[81]={9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17};// (i%9)+9
static const uint8_t _div_27_times_3_plus_mod_9_div_3_plus_18[81]={18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26};// i/27*3+(i%9)/3+18



static uint8_t _solve(solve_board_t* board,uint8_t* out){
	unsigned short guess_possibility_count;
	unsigned long guess_index=0;
	do{
		guess_possibility_count=10;
		uint64_t z64=board->z64;
		uint32_t z32=board->z32;
		while (z64||z32){
			unsigned int i;
			if (z64){
				i=FIND_FIRST_SET_BIT64(z64);
				z64&=z64-1;
			}
			else{
				i=FIND_FIRST_SET_BIT(z32);
				z32&=z32-1;
				i+=64;
			}
			uint16_t* j=board->dt+_div_9_table[i];
			uint16_t* k=board->dt+_mod_9_plus_9_table[i];
			uint16_t* l=board->dt+_div_27_times_3_plus_mod_9_div_3_plus_18[i];
			uint16_t s=(*j)&(*k)&(*l);
			if (!s){
				return 0;
			}
			if (!(s&(s-1))){
				out[i]=FIND_FIRST_SET_BIT(s)+1;
				if (i<64){
					board->z64&=~(1ull<<i);
				}
				else{
					board->z32&=~(1ull<<(i-64));
				}
				(*j)&=~s;
				(*k)&=~s;
				(*l)&=~s;
				guess_possibility_count=0;
			}
			else if (guess_possibility_count){
				unsigned short possibility_count=POPCOUNT16(s);
				if (possibility_count<guess_possibility_count){
					guess_possibility_count=possibility_count;
					guess_index=i;
				}
			}
		}
	} while (!guess_possibility_count);
	if (guess_possibility_count==10){
		return 1;
	}
	if (guess_index<64){
		board->z64&=~(1ull<<guess_index);
	}
	else{
		board->z32&=~(1u<<(guess_index-64));
	}
	unsigned long j=_div_9_table[guess_index];
	unsigned long k=_mod_9_plus_9_table[guess_index];
	unsigned long l=_div_27_times_3_plus_mod_9_div_3_plus_18[guess_index];
	uint16_t possibilities=(board->dt[j])&(board->dt[k])&(board->dt[l]);
	solve_board_t new_board;
	do{
		new_board=*board;
		uint16_t m=(~possibilities)|(possibilities-1);
		new_board.dt[j]&=m;
		new_board.dt[k]&=m;
		new_board.dt[l]&=m;
		if (_solve(&new_board,out)){
			out[guess_index]=(uint8_t)FIND_FIRST_SET_BIT(~m)+1;
			return 1;
		}
		possibilities&=possibilities-1;
	} while (possibilities);
	return 0;
}



uint8_t solve_sudoku(uint8_t* board){
	solve_board_t board_state={
		0,
		0,
		{
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff,
			0x1ff
		}
	};
	uint8_t solved_board[81];
	memcpy(solved_board,board,81*sizeof(uint8_t));
	for (uint8_t i=0;i<81;i++){
		if (board[i]){
			uint16_t m=~(1<<(board[i]-1));
			board_state.dt[_div_9_table[i]]&=m;
			board_state.dt[_mod_9_plus_9_table[i]]&=m;
			board_state.dt[_div_27_times_3_plus_mod_9_div_3_plus_18[i]]&=m;
		}
		else if (i<64){
			board_state.z64|=1ull<<i;
		}
		else{
			board_state.z32|=1<<(i-64);
		}
	}
	if (_solve(&board_state,solved_board)){
		memcpy(board,solved_board,81*sizeof(uint8_t));
		return 1;
	}
	return 0;
}
