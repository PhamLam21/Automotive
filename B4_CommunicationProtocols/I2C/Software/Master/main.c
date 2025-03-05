#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C

#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_7
#define I2C_GPIO GPIOB

#define Write_SDA_0 GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define Write_SCL_0 GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define Write_SDA_1 GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define Write_SCL_1 GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define Read_SDA_Val GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
	NOT_OK = 0,
	OK
} status;

typedef enum {
	NACK = 0,
	ACK 
} ACK_Bit;

void RCC_Config() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void Tim_init() {
	TIM_TimeBaseInitTypeDef timeInitStruct;
	timeInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //72MHZ
	timeInitStruct.TIM_Prescaler = 72 - 1; //Dem tu 0 -> 1us dem len 1
	timeInitStruct.TIM_Period = 0xFFFF; //65535 -> tran
	timeInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &timeInitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}
void delay_1ms() {
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < 1000);
}
void delay_us(uint32_t time) {
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time);
}
void delay_ms(uint32_t time) {
	while(time) {
		delay_1ms();
		--time;
	}
}
void I2C_Config() {
	Write_SDA_1;
	delay_us(3);
	Write_SCL_1;
	delay_us(3);
}
//I2C start khi keo SDA -> 0 sau do SCL -> 0
void I2C_Start() {
	Write_SCL_1;
	delay_us(3);
	Write_SDA_1;
	delay_us(3);
	Write_SDA_0;
	delay_us(3);
	Write_SCL_0;
	delay_us(3);
}
//I2C stop khi SCL -> 1 sau do SDA -> 1s
void I2C_Stop() {
	Write_SDA_0;
	delay_us(3);
	Write_SCL_1;
	delay_us(3);
	Write_SDA_1;
	delay_us(3);
}

status I2C_Write(uint8_t pData) {
	status stRet;
	for(int i = 0; i < 8; i++) {
		if(pData & 0x80) {
			Write_SDA_1;
		} else {
			Write_SDA_0;
		}
		delay_us(3);
		
		//Clock
		Write_SCL_1;
		delay_us(5);
		Write_SCL_0;
		delay_us(5);
		
		pData <<= 1;
	}
	
	//Wait ACK
	Write_SDA_1;
	delay_us(3);
	Write_SCL_1;
	delay_us(3);
	
	if(Read_SDA_Val) {
		stRet = NOT_OK;
	} else {
		stRet = OK;
	}
	
	delay_us(2);
	Write_SCL_0;
	delay_us(5);
	
	return stRet;
}
uint8_t I2C_Read(ACK_Bit _ACK) {
	uint8_t u8Ret = 0x00;
	
	Write_SDA_1;
	delay_us(1);
	
	for(int i = 0; i < 8; i++) {
		u8Ret <<= 1;
		
		//CLock
		Write_SCL_1;
		delay_us(3);
		if (Read_SDA_Val) {
			u8Ret |= 0x01;
		}
		delay_us(2);
		Write_SCL_0;
		delay_us(5);
	}
	
	if(_ACK) {
		Write_SDA_0;
	} else {
		Write_SDA_1;
	}
	delay_us(1);
	
	//Clock
	Write_SCL_1;
	delay_us(5);
	Write_SCL_0;
	delay_us(5);
	
	return u8Ret;
}
status EPROM_Read(uint16_t MemAddr, uint8_t SlaveAddress, uint8_t NumByte, uint8_t *pData)
{
	uint8_t i;
	
	I2C_Start();
	if (I2C_Write(SlaveAddress << 1) == NOT_OK) {		//Wtite the address of eprom. with Write bit
		I2C_Stop();
		return NOT_OK;
	}
	
	if (I2C_Write(MemAddr >> 8) == NOT_OK) {		//Write the 8 high bits of the adddress to write data
		I2C_Stop();
		return NOT_OK;
	}
	
	if (I2C_Write(MemAddr) == NOT_OK) {			//Write the 8 low bits of the adddress to write data
		I2C_Stop();
		return NOT_OK;
	}
	
	I2C_Start();
	
	if (I2C_Write((SlaveAddress<<1) | 1) == NOT_OK) {//Wtite the address of eprom. with Read bit
		I2C_Stop();
		return NOT_OK;
	}
	
	for (i = 0; i < NumByte - 1; ++i) { 				//Write data to 16bit MemAddr.
		pData[i] = I2C_Read(ACK);
	}
	
	pData[i] = I2C_Read(NACK);
	
	I2C_Stop();
	
	return OK;
}

status EPROM_Write(uint16_t MemAddr,uint8_t SlaveAddress, uint8_t NumByte, uint8_t *pData)
{
	uint8_t i;
	
	for (i = 0; i < NumByte; ++i) {
		I2C_Start();
		if (I2C_Write(SlaveAddress << 1) == NOT_OK) {
			I2C_Stop();
			return NOT_OK;
		}
		if (I2C_Write((MemAddr + i) >> 8) == NOT_OK) {
			I2C_Stop();
			return NOT_OK;
		}
		
		if (I2C_Write(MemAddr + i) == NOT_OK) {
			I2C_Stop();
			return NOT_OK;
		}
		if (I2C_Write(pData[i]) == NOT_OK) {
			I2C_Stop();
			return NOT_OK;
		}
		I2C_Stop();
		delay_ms(10);
	}
	return OK;
}

uint8_t Data1[10] = {0x03, 0x05, 0x0E, 0xDA, 0xA6, 0x6F, 0x50, 0x00, 0x00, 0xF0};
uint8_t Data2[10] = {0x01, 0x0A, 0x19, 0x24, 0xFA, 0x10, 0x3C, 0x48, 0x59, 0x77};
uint8_t Rcv[10] = {0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00};


int main(){
	RCC_Config();
	Tim_init();
	GPIO_Config();
	I2C_Config();
	
	while(EPROM_Write(0x0045, 0x50 ,10, Data1) == NOT_OK){}	//Default address of eprom is 0x50
	
	while(EPROM_Write(0x0060, 0x50 ,10, Data2) == NOT_OK){}
	
	while(1){
		while(EPROM_Read(0x0045, 0x50 ,10, Rcv) == NOT_OK) {}
		while(EPROM_Read(0x0060, 0x50 ,10, Rcv) == NOT_OK) {}
	}
}
