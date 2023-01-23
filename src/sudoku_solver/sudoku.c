#ifdef _MSC_VER
#include <intrin.h>
#else
#include <immintrin.h>
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
#define ATTRIBUTES
#else
#define POPCOUNT16(x) __builtin_popcount(x)
#define FIND_FIRST_SET_BIT(m) (__builtin_ffs(m)-1)
#define FIND_FIRST_SET_BIT64(m) (__builtin_ffsll(m)-1)
#define ATTRIBUTES __attribute__((flatten,hot,no_stack_protector))
#endif



typedef struct __SOLVE_BOARD{
	unsigned int data[27];
	unsigned int possibilities;
	uint64_t z64;
	uint32_t z32;
	unsigned int guess_index;
} solve_board_t;



static const uint8_t _div_9_table[81]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8};// i/9
static const uint8_t _mod_9_plus_9_table[81]={9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17};// (i%9)+9
static const uint8_t _div_27_times_3_plus_mod_9_div_3_plus_18[81]={18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26};// i/27*3+(i%9)/3+18



static ATTRIBUTES _Bool _solve(solve_board_t* board,uint8_t* out){
	solve_board_t stack[81]={*board};
	unsigned int stack_depth=0;
	while (1){
		board=stack+stack_depth;
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
				unsigned int* j=board->data+_div_9_table[i];
				unsigned int* k=board->data+_mod_9_plus_9_table[i];
				unsigned int* l=board->data+_div_27_times_3_plus_mod_9_div_3_plus_18[i];
				unsigned int s=(*j)&(*k)&(*l);
				if (!s){
					goto _fail;
				}
				if (!_blsr_u32(s)){
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
		board->guess_index=guess_index;
		board->possibilities=(board->data[_div_9_table[guess_index]])&(board->data[_mod_9_plus_9_table[guess_index]])&(board->data[_div_27_times_3_plus_mod_9_div_3_plus_18[guess_index]]);
		stack_depth++;
		goto _check_first_possibility;
_fail:;
		if (!stack_depth){
			return 0;
		}
		board=stack+stack_depth-1;
		if (!board->possibilities){
			stack_depth--;
			goto _fail;
		}
_check_first_possibility:;
		unsigned int m=(~board->possibilities)|(board->possibilities-1);
		board->possibilities=_blsr_u32(board->possibilities);
		solve_board_t* new_board=stack+stack_depth;
		*new_board=*board;
		out[board->guess_index]=FIND_FIRST_SET_BIT(~m)+1;
		new_board->data[_div_9_table[board->guess_index]]&=m;
		new_board->data[_mod_9_plus_9_table[board->guess_index]]&=m;
		new_board->data[_div_27_times_3_plus_mod_9_div_3_plus_18[board->guess_index]]&=m;
		continue;
	}
}



_Bool solve_sudoku(uint8_t* board){
	solve_board_t board_state={
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
		},
		0,
		0
	};
	uint8_t solved_board[81];
	memcpy(solved_board,board,81*sizeof(uint8_t));
	for (uint8_t i=0;i<81;i++){
		if (board[i]){
			uint16_t m=~(1<<(board[i]-1));
			board_state.data[_div_9_table[i]]&=m;
			board_state.data[_mod_9_plus_9_table[i]]&=m;
			board_state.data[_div_27_times_3_plus_mod_9_div_3_plus_18[i]]&=m;
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
