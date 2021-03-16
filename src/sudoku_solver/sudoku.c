#include <sudoku.h>
#include <intrin.h>
#pragma intrinsic(__popcnt16)
#pragma intrinsic(__movsq)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanForward64)



typedef struct __SOLVE_BOARD{
	uint64_t za;
	uint32_t zb;
	uint16_t dt[27];
} solve_board_t;



uint8_t _solve(solve_board_t* b,uint8_t* o){
	uint8_t mssl=10;
	uint8_t msi;
	uint8_t msj;
	uint8_t msk;
	uint8_t msl;
	uint16_t mss;
_nxt_move:
	uint64_t za=b->za;
	uint32_t zb=b->zb;
	while (za||zb){
		unsigned long i;
		if (za){
			_BitScanForward64(&i,za);
			za&=~(1ull<<i);
		}
		else{
			_BitScanForward(&i,zb);
			zb&=~(1ull<<i);
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
			unsigned long lg;
			_BitScanForward(&lg,s);
			*(o+i)=(uint8_t)lg+1;
			if (i<64){
				b->za&=~(1ull<<i);
			}
			else{
				b->zb&=~(1ull<<(i-64));
			}
			s=~s;
			b->dt[j]&=s;
			b->dt[k]&=s;
			b->dt[l]&=s;
			mssl|=16;
		}
		else if (bc<(mssl&15)){
			mssl=bc|(mssl&16);
			msi=(uint8_t)i;
			msj=j;
			msk=k;
			msl=l;
			mss=s;
		}
	}
	if (mssl&16){
		mssl=10;
		goto _nxt_move;
	}
	if (mssl==10){
		return 1;
	}
	if (msi<64){
		b->za&=~(1ull<<msi);
	}
	else{
		b->zb&=~(1ull<<(msi-64));
	}
	solve_board_t nb;
_check_all:
	unsigned long i;
	_BitScanForward(&i,mss);
	__movsq((uint64_t*)(&nb),(uint64_t*)b,8);
	nb.dt[26]=b->dt[26];
	*(o+msi)=(uint8_t)i+1;
	uint16_t m=~(1<<i);
	mss&=m;
	nb.dt[msj]&=m;
	nb.dt[msk]&=m;
	nb.dt[msl]&=m;
	if (_solve(&nb,o)){
		return 1;
	}
	if (mss){
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
			uint16_t m=~(1<<((uint16_t)*(b+i)-1));
			sb.dt[j]&=m;
			sb.dt[k]&=m;
			sb.dt[j/3*3+k/3+15]&=m;
		}
		else{
			if (i<64){
				sb.za|=(1ull<<i);
			}
			else{
				sb.zb|=(1ull<<(i-64));
			}
		}
	}
	return _solve(&sb,b);
}
