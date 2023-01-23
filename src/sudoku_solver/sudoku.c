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
#define FIND_FIRST_SET_BIT(i,m) _BitScanForward(&(i),(m))
#define FIND_FIRST_SET_BIT64(i,m) _BitScanForward64(&(i),(m))
#else
#define POPCOUNT16(x) __builtin_popcount((x))
#define FIND_FIRST_SET_BIT(i,m) ((i)=(__builtin_ffs((m))-1))
#define FIND_FIRST_SET_BIT64(i,m) ((i)=(__builtin_ffsll((m))-1))
#endif



typedef struct __SOLVE_BOARD{
	uint64_t z64;
	uint32_t z32;
	uint16_t dt[27];
} solve_board_t;



static const uint8_t _div_9_table[81]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8};// i/9
static const uint8_t _mod_9_plus_9_table[81]={9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17,9,10,11,12,13,14,15,16,17};// (i%9)+9
static const uint8_t _div_27_times_3_plus_mod_9_div_3_plus_18[81]={18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26,24,24,24,25,25,25,26,26,26};// i/27*3+(i%9)/3+18



static uint8_t _solve(solve_board_t* b,uint8_t* o){
	unsigned short f;
	unsigned long nmi=0;
	do{
		f=10;
		uint64_t z64=b->z64;
		uint32_t z32=b->z32;
		while (z64||z32){
			unsigned long i;
			if (z64){
				FIND_FIRST_SET_BIT64(i,z64);
				z64&=z64-1;
			}
			else{
				FIND_FIRST_SET_BIT(i,z32);
				z32&=z32-1;
				i+=64;
			}
			uint16_t* j=b->dt+_div_9_table[i];
			uint16_t* k=b->dt+_mod_9_plus_9_table[i];
			uint16_t* l=b->dt+_div_27_times_3_plus_mod_9_div_3_plus_18[i];
			uint16_t s=(*j)&(*k)&(*l);
			if (!s){
				return 0;
			}
			if (!(s&(s-1))){
				unsigned long bi;
				FIND_FIRST_SET_BIT(bi,s);
				*(o+i)=(uint8_t)bi+1;
				if (i<64){
					b->z64&=~(1ull<<i);
				}
				else{
					b->z32&=~(1ull<<(i-64));
				}
				(*j)&=~s;
				(*k)&=~s;
				(*l)&=~s;
				f=0;
			}
			else if (f){
				unsigned short bc=POPCOUNT16(s);
				if (bc<f){
					f=bc;
					nmi=i;
				}
			}
		}
	} while (!f);
	if (f==10){
		return 1;
	}
	if (nmi<64){
		b->z64&=~(1ull<<nmi);
	}
	else{
		b->z32&=~(1u<<(nmi-64));
	}
	unsigned long j=_div_9_table[nmi];
	unsigned long k=_mod_9_plus_9_table[nmi];
	unsigned long l=_div_27_times_3_plus_mod_9_div_3_plus_18[nmi];
	uint16_t s=(b->dt[j])&(b->dt[k])&(b->dt[l]);
	solve_board_t nb;
	do{
		nb=*b;
		uint16_t m=(~s)|(s-1);
		nb.dt[j]&=m;
		nb.dt[k]&=m;
		nb.dt[l]&=m;
		if (_solve(&nb,o)){
			unsigned long i;
			FIND_FIRST_SET_BIT(i,~m);
			*(o+nmi)=(uint8_t)i+1;
			return 1;
		}
		s&=s-1;
	} while (s);
	return 0;
}



uint8_t solve_sudoku(uint8_t* b){
	solve_board_t sb={
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
	uint8_t tmp[81];
	memcpy(tmp,b,81*sizeof(uint8_t));
	for (uint8_t i=0;i<81;i++){
		if (*(b+i)){
			uint16_t m=~(1<<(*(b+i)-1));
			sb.dt[_div_9_table[i]]&=m;
			sb.dt[_mod_9_plus_9_table[i]]&=m;
			sb.dt[_div_27_times_3_plus_mod_9_div_3_plus_18[i]]&=m;
		}
		else if (i<64){
			sb.z64|=1ull<<i;
		}
		else{
			sb.z32|=1<<(i-64);
		}
	}
	if (_solve(&sb,tmp)){
		memcpy(b,tmp,81*sizeof(uint8_t));
		return 1;
	}
	return 0;
}
