/*
	Author: Pham Tung Lam
	Date: 3/2/2025
	Description: Su dung GPIO voi thu vien SPL
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
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

int main() {
	setUp();
	while(1) {
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(10000000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(10000000);
	}
}
