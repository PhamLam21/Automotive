#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI1_NSS GPIO_Pin_4
#define SPI1_SCK GPIO_Pin_5
#define SPI1_MISO GPIO_Pin_6
#define SPI1_MOSI GPIO_Pin_7
#define SPI_Pin GPIOA

void RCC_config() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_config() {
	GPIO_InitTypeDef GPIO_init;
	
	GPIO_init.GPIO_Pin = SPI1_SCK | SPI1_MISO | SPI1_MOSI;
	GPIO_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI_Pin, &GPIO_init);
	
	GPIO_init.GPIO_Pin = SPI1_NSS;
	GPIO_init.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI_Pin, &GPIO_init);
}
void TIM_config() {
	TIM_TimeBaseInitTypeDef TIM_init;
	
	TIM_init.TIM_ClockDivision = TIM_CKD_DIV2; //36MHZ
	TIM_init.TIM_Prescaler = 36000 - 1; //1ms tang 1
	TIM_init.TIM_Period = 0xFFFF;
	TIM_init.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_init);
	TIM_Cmd(TIM2, ENABLE);
}
void SPI_config() {
	SPI_InitTypeDef SPI_init;
	
	SPI_init.SPI_Mode = SPI_Mode_Slave;
	SPI_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_init.SPI_CPOL = SPI_CPOL_Low;
	SPI_init.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_init.SPI_DataSize = SPI_DataSize_8b;
	SPI_init.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_init.SPI_CRCPolynomial = 7;
	SPI_init.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_init);
	SPI_Cmd(SPI1, ENABLE);
}
void delay_ms(uint32_t time){
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time){}
}

uint8_t SPI_SendReceiveByte(uint8_t dataSend){
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {}
    SPI_I2S_SendData(SPI1, dataSend); // G?i d? li?u tru?c
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {} // Ð?i SPI hoàn thành truy?n
			
    uint8_t temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}
    return temp;
}

uint8_t dataSend[] = {1, 2, 3, 4, 5, 6, 7};
uint8_t data;
int main() {
	RCC_config();
	GPIO_config();
	TIM_config();
	SPI_config();
	uint8_t index = 0;
	while(1) {
		while(GPIO_ReadInputDataBit(GPIOA, SPI1_NSS) == 1){}
		if(GPIO_ReadInputDataBit(GPIOA, SPI1_NSS) == 0){
			data = SPI_SendReceiveByte(dataSend[index]);
      index = (index + 1) % 7;
    }
	}
}
