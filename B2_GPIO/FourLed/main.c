/*
	Author: Pham Tung Lam
	Date: 3/2/2025
	Description: 4 led nhay lan luot
*/

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void delay(uint32_t time) {
	for(uint32_t i = 0; i < time; i++) {}
}

void setUp() {
	//Cap clock cho GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//Cau hinh GPIOC
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void chaseLed(uint8_t loop) {
	uint16_t ledVal;
	for(int j = 0; j < loop; j++) {
		ledVal = 0x0010; //0b0 0001 0000
		for(int i = 0; i < 4; i++) {
			GPIO_Write(GPIOC, ledVal);
			ledVal = ledVal << 1;
			delay(10000000);
		}
	}
}

int main() {
	setUp();
	while(1) {
		chaseLed(2);
		break;
	}
}
