#include "stm32f103xb.h"//hacer que cuente para abajo
#include"seg.h"
#define pinInterrupt 8
int numero=0;
int subiBaja=1;
int main(void){
    d7s_init(0,1,2,3,4,5,6);
    d7s(0,1,2,3,4,5,6,7);

    
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 

    GPIOA->CRH&=~(0xf<<((pinInterrupt%8)*4));
    GPIOA->CRH|=(0x8<<((pinInterrupt%8)*4));
    GPIOA->BSRR|=(1<<(pinInterrupt+16));

    AFIO->EXTICR[0] &= ~(0xf<<(pinInterrupt * 4));

    EXTI->IMR  |= (1 <<  pinInterrupt);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
    EXTI->RTSR |= (1 <<  pinInterrupt);   // Habilitar flanco de subida
    EXTI->FTSR &= ~(1 << pinInterrupt);  // Deshabilitar flanco de bajada


    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 0);  

    while(1){
    d7s(0,1,2,3,4,5,6,numero);

    }
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << pinInterrupt)) {    // Pregunto por bandera 
        EXTI->PR |= (1 << pinInterrupt);
        if(subiBaja==1){
            if(numero<8){
                numero++;
            }else{
                numero--;
                subiBaja=0;
            }
            
        }else{
            if(numero>0){
                numero++;
            }else{
                numero++;
                subiBaja=1;
            }
        }
    }
}