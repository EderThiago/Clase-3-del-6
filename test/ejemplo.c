#include "stm32f103xb.h"

#define PA3 3
#define PC7 7
#define led 5

int main(void) {
    // 1. Habilitar reloj para GPIOA, GPIOB, GPIOC y AFIO  
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPBEN |RCC_APB2ENR_IOPCEN |RCC_APB2ENR_AFIOEN;   

    // 2. Configurar I/O
    // led como salida
    GPIOB->CRL &=~(0XF<<led*4);
    GPIOB->CRL |= (0X1<<led*4);
    
    // PC como salida
    GPIOA->CRL &=~(0XF<<PA3*4);
    GPIOA->CRL |= (0X8<<PA3*4);
    GPIOA->BSRR|=(1<<(PA3+16));
    GPIOB->CRL &=~(0XF<<PC7*4);
    GPIOB->CRL |= (0X8<<PC7*4);
    GPIOA->BSRR|=(1<<(PC7+16));
    // 3. Configurar EXTI3 para usar PA3 - A EXTI3 lo asocio con PA3
    // lo mismo para PC7
    /* EXTICR[n]
     n = 0:EXTICR1 - 1:EXTICR2 - 2:EXTICR3 - 3:EXTICR4
    */
    AFIO->EXTICR[0] &= ~(0xF << (PA3 * 4)); // EXTICR[0] controla EXTI0–3, y PA = 0000
    AFIO->EXTICR[1] |=  (0x1 << ((PC7%4)*4));
    /* PARA USAR PINES DEL 4 EN ADELANTE USAR ESTA LÍNEA DE CÓDIGO
    
    AFIO->EXTICR[n] &= ~(0xF << (n%4 * 4)); -> Pongo todo en 0
    AFIO->EXTICR[n] |=  (bitsPuerto << (n%4 * 4)); 

    Donde bitsPuerto va a ser: 0x0 PA - 0x1 PB - 0x2 PC - 0x3 PD - 0x4 PE - 0x5 PF - 0x6 PG 

    */


    // 4. Configurar interrupción por flanco de subida
    EXTI->IMR  |= (1 <<  PA3);   // No enmascarar EXTI3 (dejar que EXTI3 sea interrupción)
    EXTI->RTSR |= (1 <<  PA3);   // Habilitar flanco de subida
    EXTI->FTSR &= ~(1 << PA3);  // Deshabilitar flanco de bajada
    /* PARA PC7*/
    EXTI->IMR |= (1<<PC7);
    EXTI->FTSR |= (1<<PC7);
    EXTI->RTSR &=~ (1<<PC7);
    // 5. Habilitar interrupción EXTI3 en NVIC
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_SetPriority(EXTI3_IRQn, 1);  // Prioridad baja (0 = más alta)
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn,0);
    
    while (1) {

    }
    return 0;
}

// 6. ISR para EXTI3
void EXTI3_IRQHandler(void) {
    if (EXTI->PR & (1 << PA3)) {    // Pregunto por bandera
        GPIOB->BSRR = (1 << led);   // Encender LED en PB2
        EXTI->PR |= (1 << PA3);     // Limpiar bandera de EXTI3
    }
}

void EXTI9_5_IRQHandler(void){
    if(EXTI->PR&(1<<PC7)){
        EXTI->PR|=(1<<PC7);
        GPIOB->BSRR = (1 << (led+16));   // Apago LED en PB2
        
    }
}