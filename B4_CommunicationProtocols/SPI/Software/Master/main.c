/*
Author: Pham Tung Lam
Date: 17/2/2025
Description: SPI Master Software
*/

#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA 
#define SPI_RCC RCC_APB2Periph_GPIOA

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void delay_ms(uint32_t time) {
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time); //delay 1 ms
}
void RCC_Config() {
	
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void Tim_init() {
	TIM_TimeBaseInitTypeDef timeInitStruct;
	timeInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //72MHZ
	timeInitStruct.TIM_Prescaler = 36000 - 1; //Dem tu 0 -> 1ms dem len 1
	timeInitStruct.TIM_Period = 0xFFFF; //65535 -> tran
	timeInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &timeInitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void SPI_Soft_init() {
	GPIO_InitTypeDef gpioInitStruct;
	gpioInitStruct.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &gpioInitStruct);
	
	gpioInitStruct.GPIO_Pin = SPI_MISO_Pin;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &gpioInitStruct);
}

void SPI_Clock() {
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	delay_ms(4);
}

void SPI_init() { //Ham tao trang thai ban dau cua cac chan
	GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}

void SPI_Master_Transmit(uint8_t u8Data) {
	uint8_t u8Mask = 0x01;
	uint8_t tempData;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	for(int i = 0; i < 8; i++) {
		tempData = u8Mask & u8Data;
		if(tempData) {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		} else {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data = u8Data >> 1;
		SPI_Clock();
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	delay_ms(1);
}
int main() {
	RCC_Config();
	SPI_Soft_init();
	Tim_init();
	SPI_init();
	int test[4] = {0, 1, 2, 3};
	while(1) {
		for(int i = 0; i< 4; i++) {
			SPI_Master_Transmit(test[i]);
			delay_ms(4000);
		}
	}
	return 0;
}
