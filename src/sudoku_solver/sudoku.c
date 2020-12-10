#include <sudoku.h>
#include <stdlib.h>
#include <stdio.h>



uint8_t _BIT_CNT[]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,5,6,6,7,6,7,7,8,6,7,7,8,7,8,8,9};



uint8_t* _solve(uint8_t* b,uint16_t* dt){
	while (true){
		uint8_t mssl=10;
		uint8_t msi;
		uint8_t msj;
		uint8_t msk;
		uint8_t msl;
		uint16_t mss;
		bool mv=false;
		for (uint8_t i=0;i<81;i++){
			if (*(b+i)==0){
				uint8_t j=i/9;
				uint8_t k=i%9+9;
				uint8_t l=j/3*3+k/3+15;
				uint16_t s=(*(dt+j))&(*(dt+k))&(*(dt+l));
				if (!s){
					return NULL;
				}
				else if (!(s&(s-1))){
					uint16_t m=~s;
					_l:
					(*(b+i))++;
					if (s>>=1){
						goto _l;
					}
					(*(dt+j))&=m;
					(*(dt+k))&=m;
					(*(dt+l))&=m;
					mv=true;
				}
				else{
					uint8_t sl=_BIT_CNT[s];
					if (sl<mssl){
						mssl=sl;
						msi=i;
						msj=j;
						msk=k;
						msl=l;
						mss=s;
					}
				}
			}
		}
		if (mv==false){
			if (mssl==10){
				return b;
			}
			uint8_t n=0;
			uint8_t* nb=malloc(81*sizeof(uint8_t));
			uint16_t* ndt=malloc(27*sizeof(uint16_t));
			uint16_t m=1;
			for (uint8_t i=0;i<9;i++){
				if (mss&m){
					n++;
					for (uint8_t j=0;j<81;j++){
						*(nb+j)=*(b+j);
						if (j<27){
							*(ndt+j)=*(dt+j);
						}
					}
					*(nb+msi)=i+1;
					(*(ndt+msj))&=~m;
					(*(ndt+msk))&=~m;
					(*(ndt+msl))&=~m;
					uint8_t* nbr=_solve(nb,ndt);
					if (nbr!=NULL){
						free(ndt);
						if (nbr!=nb){
							free(nb);
						}
						return nbr;
					}
					if (n==mssl){
						free(nb);
						free(ndt);
						return NULL;
					}
				}
				m<<=1;
			}
		}
	}
}



void solve_sudoku(uint8_t* b){
	uint16_t* dt=malloc(27*sizeof(uint16_t));
	for (uint8_t i=0;i<27;i++){
		*(dt+i)=0x1ff;
	}
	for (uint8_t i=0;i<81;i++){
		if (*(b+i)!=0){
			uint16_t m=~(1<<((uint16_t)*(b+i)-1));
			uint8_t j=i/9;
			uint8_t k=i%9+9;
			uint8_t l=j/3*3+k/3+15;
			(*(dt+j))&=m;
			(*(dt+k))&=m;
			(*(dt+l))&=m;
		}
	}
	uint8_t* sb=_solve(b,dt);
	free(dt);
	if (sb!=b){
		for (uint8_t i=0;i<81;i++){
			*(b+i)=*(sb+i);
		}
		free(sb);
	}
}
