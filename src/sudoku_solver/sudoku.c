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
#define POPCOUNT16(x) __popcnt16(x)
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
	unsigned int data[27];
	unsigned int possibilities;
	uint64_t z64;
	uint32_t z32;
	unsigned int indices;
} solve_board_t;



static const uint8_t _div_9_table[81]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8};// i/9
static const uint8_t _mod_9_plus_9_table[81]={9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17};// (i%9)+9
static const uint8_t _div_27_times_3_plus_mod_9_div_3_plus_18[81]={18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26};// i/27*3+(i%9)/3+18
static const unsigned int _combined_indices[81]={0x12090000,0x120a0001,0x120b0002,0x130c0003,0x130d0004,0x130e0005,0x140f0006,0x14100007,0x14110008,0x12090109,0x120a010a,0x120b010b,0x130c010c,0x130d010d,0x130e010e,0x140f010f,0x14100110,0x14110111,0x12090212,0x120a0213,0x120b0214,0x130c0215,0x130d0216,0x130e0217,0x140f0218,0x14100219,0x1411021a,0x1509031b,0x150a031c,0x150b031d,0x160c031e,0x160d031f,0x160e0320,0x170f0321,0x17100322,0x17110323,0x15090424,0x150a0425,0x150b0426,0x160c0427,0x160d0428,0x160e0429,0x170f042a,0x1710042b,0x1711042c,0x1509052d,0x150a052e,0x150b052f,0x160c0530,0x160d0531,0x160e0532,0x170f0533,0x17100534,0x17110535,0x18090636,0x180a0637,0x180b0638,0x190c0639,0x190d063a,0x190e063b,0x1a0f063c,0x1a10063d,0x1a11063e,0x1809073f,0x180a0740,0x180b0741,0x190c0742,0x190d0743,0x190e0744,0x1a0f0745,0x1a100746,0x1a110747,0x18090848,0x180a0849,0x180b084a,0x190c084b,0x190d084c,0x190e084d,0x1a0f084e,0x1a10084f,0x1a110850};// i+i/9*256+i/27*50331648+(i%9)*65536+(i%9)/3*16777216+302579712



static _Bool _solve(solve_board_t* board,uint8_t* out){
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
				unsigned int possibilities=(*j)&(*k)&(*l);
				if (!possibilities){
					goto _fail;
				}
				if (!_blsr_u32(possibilities)){
					out[i]=FIND_FIRST_SET_BIT(possibilities)+1;
					if (i<64){
						board->z64&=~(1ull<<i);
					}
					else{
						board->z32&=~(1ull<<(i-64));
					}
					(*j)&=~possibilities;
					(*k)&=~possibilities;
					(*l)&=~possibilities;
					guess_possibility_count=0;
				}
				else if (guess_possibility_count){
					unsigned short possibility_count=POPCOUNT16(possibilities);
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
		board->indices=_combined_indices[guess_index];
		board->possibilities=(board->data[(board->indices>>8)&0xff])&(board->data[(board->indices>>16)&0xff])&(board->data[board->indices>>24]);
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
		out[board->indices&0xff]=(uint8_t)FIND_FIRST_SET_BIT(~m)+1;
		new_board->data[(board->indices>>8)&0xff]&=m;
		new_board->data[(board->indices>>16)&0xff]&=m;
		new_board->data[board->indices>>24]&=m;
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
		0,
		0,
		0
	};
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
	return _solve(&board_state,board);
}
