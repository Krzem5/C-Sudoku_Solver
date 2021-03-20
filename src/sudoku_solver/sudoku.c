#include <sudoku.h>
#include <intrin.h>
#include <stdint.h>
#pragma intrinsic(__movsq)
#pragma intrinsic(__popcnt16)
#pragma intrinsic(__stosw)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanForward64)



typedef struct __SOLVE_BOARD{
	uint64_t z64;
	uint32_t z32;
	uint16_t dt[27];
} solve_board_t;



uint8_t _solve(solve_board_t* b,uint8_t* o){
	uint8_t f=10;
	uint8_t nmi;
_nxt_move:
	uint64_t z64=b->z64;
	uint32_t z32=b->z32;
	while (z64||z32){
		unsigned long i;
		if (z64){
			_BitScanForward64(&i,z64);
			z64&=~(1ull<<i);
		}
		else{
			_BitScanForward(&i,z32);
			z32&=~(1<<i);
			i+=64;
		}
		uint16_t* j=b->dt+i/9;
		uint16_t* k=b->dt+i%9+9;
		uint16_t* l=b->dt+i/27*3+(i%9)/3+18;
		uint16_t s=(*j)&(*k)&(*l);
		if (!s){
			return 0;
		}
		unsigned short bc=__popcnt16(s);
		if (bc==1){
			unsigned long bi;
			_BitScanForward(&bi,s);
			*(o+i)=(uint8_t)bi+1;
			if (i<64){
				b->z64&=~(1ull<<i);
			}
			else{
				b->z32&=~(1<<(i-64));
			}
			(*j)&=~s;
			(*k)&=~s;
			(*l)&=~s;
			f=0;
		}
		else if (f&&bc<f){
			f=(uint8_t)bc;
			nmi=(uint8_t)i;
		}
	}
	if (!f){
		f=10;
		goto _nxt_move;
	}
	if (f==10){
		return 1;
	}
	if (nmi<64){
		b->z64&=~(1ull<<nmi);
	}
	else{
		b->z32&=~(1<<(nmi-64));
	}
	uint8_t j=nmi/9;
	uint8_t k=nmi%9+9;
	uint8_t l=j/3*3+k/3+15;
	uint16_t s=(b->dt[j])&(b->dt[k])&(b->dt[l]);
	solve_board_t nb;
	uint16_t* nb_j=nb.dt+j;
	uint16_t* nb_k=nb.dt+k;
	uint16_t* nb_l=nb.dt+l;
_check_all:
	unsigned long i;
	_BitScanForward(&i,s);
	__movsq((uint64_t*)(&nb),(uint64_t*)b,8);
	nb.dt[26]=b->dt[26];
	uint16_t m=~(1<<(uint16_t)i);
	s&=m;
	(*nb_j)&=m;
	(*nb_k)&=m;
	(*nb_l)&=m;
	if (_solve(&nb,o)){
		*(o+nmi)=(uint8_t)i+1;
		return 1;
	}
	if (s){
		goto _check_all;
	}
	return 0;
}



uint8_t solve_sudoku(uint8_t* b){
	solve_board_t sb={
		0,
		0
	};
	__stosw((unsigned short*)sb.dt,0x1ff,27);
	for (uint8_t i=0;i<81;i++){
		uint8_t v=*(b+i);
		if (v){
			uint16_t m=~(1<<(v-1));
			sb.dt[i/9]&=m;
			sb.dt[i%9+9]&=m;
			sb.dt[i/27*3+(i%9)/3+18]&=m;
		}
		else{
			if (i<64){
				sb.z64|=(1ull<<i);
			}
			else{
				sb.z32|=(1<<(i-64));
			}
		}
	}
	return _solve(&sb,b);
}
