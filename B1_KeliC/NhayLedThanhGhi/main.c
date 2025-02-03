/*
	Author: Pham Tung Lam
	Date: 2/2/2025
	Description: bat tat led tai chan PC13 bang nut nhan
*/
typedef struct 
{
	unsigned int CR;
	unsigned int CFGR;
	unsigned int CIR;
	unsigned int APB2RSTR;
	unsigned int APB1RSTR;
	unsigned int AHBENR;
	unsigned int APB2ENR;
	unsigned int APB1ENR;
	unsigned int BDCR;
	unsigned int CSR;
} RCC_t;

typedef struct
{
	unsigned int CRL;
	unsigned int CRH;
	unsigned int IDR;
	unsigned int ODR;
	unsigned int BSRR;
	unsigned int BRR;
	unsigned int LCKR;
} GPIO_t;

#define RCC ((RCC_t *) 0x40021000)
#define GPIOA ((GPIO_t *)  0x40010800) 
#define GPIOC ((GPIO_t *)  0x40011000) 
	
void delay(unsigned int time) {
	for(unsigned int i = 0; i < time; i++) {}
}

void setUp() {
	//Cap clock cho GPIO A va C
	RCC->APB2ENR |= (1 << 4) | (1 << 2);
	
	//Cau hinh input pull down PA0
	GPIOA->CRL &= ~(0xF << 0);
	GPIOA->CRL |= (8 << 0);
	GPIOA->ODR &= ~(1 << 0);
	
	//Cau hinh output push-pull PC13
	GPIOC->CRH &= ~(0xF << 20);
	GPIOC->CRH |= (3 << 20);
	
	//
}
int main(){
	setUp();
	while(1) {
		if((GPIOA->IDR & (1 << 0)) == 1) {
			GPIOC->ODR &= ~(1 << 13);
		} else {
			GPIOC->ODR |= (1 << 13);
		}
			
	}
}
