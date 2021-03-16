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
	uint16_t nms;
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
		uint8_t j=(uint8_t)i/9;
		uint8_t k=(uint8_t)i%9+9;
		uint8_t l=j/3*3+k/3+15;
		uint16_t s=(b->dt[j])&(b->dt[k])&(b->dt[l]);
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
			s=~s;
			b->dt[j]&=s;
			b->dt[k]&=s;
			b->dt[l]&=s;
			f=0;
		}
		else if (f&&bc<f){
			f=(uint8_t)bc;
			nmi=(uint8_t)i;
			nms=s;
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
	solve_board_t nb;
_check_all:
	unsigned long i;
	_BitScanForward(&i,nms);
	__movsq((uint64_t*)(&nb),(uint64_t*)b,8);
	nb.dt[26]=b->dt[26];
	*(o+nmi)=(uint8_t)i+1;
	uint16_t m=~(1<<i);
	nms&=m;
	nb.dt[j]&=m;
	nb.dt[k]&=m;
	nb.dt[l]&=m;
	if (_solve(&nb,o)){
		return 1;
	}
	if (nms){
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
		if (*(b+i)){
			uint8_t j=i/9;
			uint8_t k=i%9+9;
			uint16_t m=~(1<<((uint16_t)(*(b+i))-1));
			sb.dt[j]&=m;
			sb.dt[k]&=m;
			sb.dt[j/3*3+k/3+15]&=m;
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
