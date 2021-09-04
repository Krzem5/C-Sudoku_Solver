#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <sudoku.h>
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



uint8_t _solve(solve_board_t* b,uint8_t* o){
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
			uint16_t* j=b->dt+i/9;
			uint16_t* k=b->dt+i%9+9;
			uint16_t* l=b->dt+i/27*3+(i%9)/3+18;
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
	unsigned long j=nmi/9;
	unsigned long k=nmi%9+9;
	unsigned long l=j/3*3+k/3+15;
	uint16_t s=(b->dt[j])&(b->dt[k])&(b->dt[l]);
	solve_board_t nb=*b;
	uint16_t* nb_j=nb.dt+j;
	uint16_t* nb_k=nb.dt+k;
	uint16_t* nb_l=nb.dt+l;
	while (1){
		uint16_t m=(~s)|(s-1);
		(*nb_j)&=m;
		(*nb_k)&=m;
		(*nb_l)&=m;
		if (_solve(&nb,o)){
			unsigned long i;
			FIND_FIRST_SET_BIT(i,~m);
			*(o+nmi)=(uint8_t)i+1;
			return 1;
		}
		s&=s-1;
		if (!s){
			break;
		}
		nb=*b;
	}
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
			uint16_t m=~(1<<((*(b+i))-1));
			sb.dt[i/9]&=m;
			sb.dt[i%9+9]&=m;
			sb.dt[i/27*3+(i%9)/3+18]&=m;
		}
		else if (i<64){
			sb.z64|=(1ull<<i);
		}
		else{
			sb.z32|=(1<<(i-64));
		}
	}
	if (_solve(&sb,tmp)){
		memcpy(b,tmp,81*sizeof(uint8_t));
		return 1;
	}
	return 0;
}
