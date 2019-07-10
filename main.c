/*
Author: Nishant Sood
Org:    Winacro innovations
*/

#include "stm32f4xx.h"                  // Device header

#define LED_green       (1u << 5)
#define LED_BIT_green   (1u << 10)
#define LED_clock_green (1u << 2)
#define LED_red         (1u << 3)
#define LED_BIT_red     (1u << 6)
#define LED_clock_red   (1u << 4)

uint32_t LED_green_stack[40], LED_red_stack[40];
uint32_t *sp_green = &LED_green_stack[40]; //Assigning Stack pointer of depth 40 
uint32_t *sp_red  =  &LED_red_stack[40];   // ditto


volatile uint32_t  tick;
volatile uint32_t _tick;

void DelayS(uint32_t secs);

int LED_green_main(void){
	
	while(1){
		GPIOC->ODR ^= LED_green; // Toggle LED bit state
		DelayS(1);
	}
	
}

int LED_red_main(void){
	
	while(1){
		GPIOE->ODR ^= LED_red;  //Toggle LED bit state
		DelayS(1);
	}
	
}

int main(){
	
	RCC->AHB1ENR |= LED_clock_green | LED_clock_red; // Assigning system clock to GPIO
	GPIOC->MODER |= LED_BIT_green;                   // Mode setup output for GPIO LED green
	GPIOE->MODER |= LED_BIT_red;                     // Ditto as above, but for GPIO LED Red
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100u);
	__enable_irq();

	/*
	 Assigning dummy values to Program Stack Register to
	 simulate register manupulation for different threads
	*/
    *(--sp_green) = (1u << 24); //xPSR	             
	*(--sp_green) = (uint32_t) &LED_green_main;
	*(--sp_green) = 0x0000000Du;
	*(--sp_green) = 0x0000000Eu;
	*(--sp_green) = 0x0000000Au;
	*(--sp_green) = 0x0000000Du;
	*(--sp_green) = 0x0000000Eu;
	*(--sp_green) = 0x0000000Au;
	
	*(--sp_red) = (1u << 24); //xPSR	
	*(--sp_red) = (uint32_t) &LED_red_main;
	*(--sp_red) = 0x0000000Bu;
	*(--sp_red) = 0x0000000Eu;
	*(--sp_red) = 0x0000000Eu;
	*(--sp_red) = 0x0000000Fu;
	*(--sp_red) = 0x0000000Eu;
	*(--sp_red) = 0x0000000Fu;
	
	for(;;){
				
	}
}

void SysTick_Handler(void){ // System timer interrupt handler
	
	++tick;
}

uint32_t getTick(void){     // System call for getting ticks from timer interrupt handler
	
	__disable_irq();
	_tick = tick;
	__enable_irq();
	
	return _tick;
	
}
 
void DelayS(uint32_t secs){ // Blocking delayS calls
	secs *= 100;
	uint32_t temp = getTick();
 	 while((getTick() - temp) < secs){}
	
}
