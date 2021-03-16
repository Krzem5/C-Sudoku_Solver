#include <sudoku.h>
#include <intrin.h>
#pragma intrinsic(__popcnt16)
#pragma intrinsic(__movsq)
#pragma intrinsic(_BitScanForward)



typedef struct __SOLVE_BOARD{
	uint8_t b[81];
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
	for (uint8_t i=0;i<81;i++){
		if (!b->b[i]){
			uint8_t j=i/9;
			uint8_t k=i%9+9;
			uint8_t l=j/3*3+k/3+15;
			uint16_t s=(b->dt[j])&(b->dt[k])&(b->dt[l]);
			if (!s){
				return 0;
			}
			unsigned short bc=__popcnt16(s);
			if (bc==1){
				unsigned long lg;
				_BitScanForward(&lg,s);
				b->b[i]=(uint8_t)lg+1;
				s=~s;
				b->dt[j]&=s;
				b->dt[k]&=s;
				b->dt[l]&=s;
				mssl|=16;
			}
			else if (bc<(mssl&15)){
				mssl=bc|(mssl&16);
				msi=i;
				msj=j;
				msk=k;
				msl=l;
				mss=s;
			}
		}
	}
	if (mssl&16){
		mssl=10;
		goto _nxt_move;
	}
	if (mssl==10){
		__movsq((uint64_t*)o,(uint64_t*)b->b,10);
		*(o+80)=b->b[80];
		return 1;
	}
	solve_board_t nb;
	uint16_t m=1;
	uint8_t n=0;
	for (uint8_t i=0;i<9;i++){
		if (mss&m){
			__movsq((uint64_t*)(&nb),(uint64_t*)b,17);
			nb.b[msi]=i+1;
			uint16_t mm=~m;
			nb.dt[msj]&=mm;
			nb.dt[msk]&=mm;
			nb.dt[msl]&=mm;
			if (_solve(&nb,o)){
				return 1;
			}
			n++;
			if (n==mssl){
				return 0;
			}
		}
		m<<=1;
	}
	__assume(0);
}



void solve_sudoku(uint8_t* b){
	solve_board_t sb;
	__stosw((unsigned short*)sb.dt,0x1ff,27);
	for (uint8_t i=0;i<81;i++){
		if (sb.b[i]=*(b+i)){
			uint8_t j=i/9;
			uint8_t k=i%9+9;
			uint16_t m=~(1<<((uint16_t)*(b+i)-1));
			sb.dt[j]&=m;
			sb.dt[k]&=m;
			sb.dt[j/3*3+k/3+15]&=m;
		}
	}
	_solve(&sb,b);
}
