#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <stdint.h>



#ifdef _MSC_VER
#pragma intrinsic(__popcnt16)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanForward64)
#define POPCOUNT16(x) __popcnt16((x))
static inline __force_inline unsigned long FIND_FIRST_SET_BIT(uint64_t mask){
	unsigned long out;
	_BitScanForward(&out,mask);
	return out;
}
static inline __force_inline unsigned long FIND_FIRST_SET_BIT64(uint64_t mask){
	unsigned long out;
	_BitScanForward64(&out,mask);
	return out;
}
#else
#define POPCOUNT16(x) __builtin_popcount((x))
#define FIND_FIRST_SET_BIT(mask) (__builtin_ffs((mask))-1)
#define FIND_FIRST_SET_BIT64(mask) (__builtin_ffsll((mask))-1)
#endif



typedef struct __SOLVE_BOARD{
	uint64_t z64;
	uint32_t z32;
	uint16_t data[27];
} solve_board_t;



static const uint8_t _index_to_row[81]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8};
static const uint8_t _index_to_column[81]={9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17};
static const uint8_t _index_to_square[81]={18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26};



static uint8_t _solve(solve_board_t* board,uint8_t* out){
	unsigned short shortest_guess_length;
	uint8_t next_guess_index=0;
	do{
		shortest_guess_length=10;
		uint64_t z64=board->z64;
		uint32_t z32=board->z32;
		while (z64||z32){
			unsigned long index;
			if (z64){
				index=FIND_FIRST_SET_BIT64(z64);
				z64&=z64-1;
			}
			else{
				index=FIND_FIRST_SET_BIT(z32)+64;
				z32&=z32-1;
			}
			uint16_t* j=board->data+_index_to_row[index];
			uint16_t* k=board->data+_index_to_column[index];
			uint16_t* l=board->data+_index_to_square[index];
			uint16_t possible_numbers=(*j)&(*k)&(*l);
			if (!possible_numbers){
				return 0;
			}
			if (!(possible_numbers&(possible_numbers-1))){
				out[index]=(uint8_t)FIND_FIRST_SET_BIT(possible_numbers)+1;
				if (index<64){
					board->z64&=~(1ull<<index);
				}
				else{
					board->z32&=~(1ull<<(index-64));
				}
				(*j)&=~possible_numbers;
				(*k)&=~possible_numbers;
				(*l)&=~possible_numbers;
				shortest_guess_length=0;
			}
			else if (shortest_guess_length){
				unsigned short number_count=POPCOUNT16(possible_numbers);
				if (number_count<shortest_guess_length){
					shortest_guess_length=number_count;
					next_guess_index=index;
				}
			}
		}
	} while (!shortest_guess_length);
	if (shortest_guess_length==10){
		return 1;
	}
	if (next_guess_index<64){
		board->z64&=~(1ull<<next_guess_index);
	}
	else{
		board->z32&=~(1u<<(next_guess_index-64));
	}
	uint8_t j=_index_to_row[next_guess_index];
	uint8_t k=_index_to_column[next_guess_index];
	uint8_t l=_index_to_square[next_guess_index];
	uint16_t possible_numbers=(board->data[j])&(board->data[k])&(board->data[l]);
	solve_board_t new_board;
	do{
		new_board=*board;
		uint16_t mask=(~possible_numbers)|(possible_numbers-1);
		new_board.data[j]&=mask;
		new_board.data[k]&=mask;
		new_board.data[l]&=mask;
		if (_solve(&new_board,out)){
			out[next_guess_index]=(uint8_t)FIND_FIRST_SET_BIT(~mask)+1;
			return 1;
		}
		possible_numbers&=possible_numbers-1;
	} while (possible_numbers);
	return 0;
}



_Bool solve_sudoku(uint8_t* board_data){
	solve_board_t board={
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
	for (uint8_t i=0;i<81;i++){
		if (board_data[i]){
			uint16_t m=~(1<<(board_data[i]-1));
			board.data[_index_to_row[i]]&=m;
			board.data[_index_to_column[i]]&=m;
			board.data[_index_to_square[i]]&=m;
		}
		else if (i<64){
			board.z64|=1ull<<i;
		}
		else{
			board.z32|=1<<(i-64);
		}
	}
	if (_solve(&board,board_data)){
		return 1;
	}
	return 0;
}
