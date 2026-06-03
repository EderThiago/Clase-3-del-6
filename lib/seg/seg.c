#include"seg.h"
int est[10][7]= {
                {1,1,1,1,1,0,1},
                {0,1,1,0,0,0,0},
                {1,1,0,1,1,0,1},
                {1,1,1,1,0,0,1},
                {0,1,1,0,0,1,1},
                {1,0,1,1,0,1,1},
                {1,0,1,1,1,1,1},
                {1,1,1,0,0,0,0},
                {1,1,1,1,1,1,1},
                {1,1,1,1,0,1,1}
};
void d7s_init(int a, int b, int c, int d, int e , int f , int g){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    int pines[7]={a,b,c,d,e,f,g};
    for(int i=0;i<7;i++){
        if(pines[i]<8){
            GPIOA->CRL&=~(0xf<<(pines[i]*4));
            GPIOA->CRL|=(0x1<<(pines[i]*4));
        }else{
            GPIOA->CRH&=~(0xf<<((pines[i]%8)*4));
            GPIOA->CRH|=(0x1<<((pines[i]%8)*4));
        }
    }
    
}
void d7s(int a, int b, int c, int d, int e ,int f ,int g, int num){
    int pines[7]={a,b,c,d,e,f,g};
    for(int i=0;i<7;i++){
        if(est[num][i]){
            GPIOA->BSRR|=(1<<(pines[i]));
    }else{
        GPIOA->BSRR|=(1<<(pines[i]+16));
    }
}}