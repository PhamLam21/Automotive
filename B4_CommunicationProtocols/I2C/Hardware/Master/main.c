#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C

#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_7
#define I2C_GPIO GPIOB

typedef enum {
	NOT_OK = 0,
	OK
} status;

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
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}
void I2C_Config(){
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00; //Dia chi cua thiet bi 0x00 la master
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
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
void I2C_Write(uint8_t pData) {
	I2C_SendData(I2C1, pData);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
uint8_t I2C_Read() {
	uint8_t data = I2C_ReceiveData(I2C1);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	return data;
}
status EPROM_Read(uint16_t MemAddr, uint8_t SlaveAddress, uint8_t NumByte, uint8_t *pData)
{
    uint8_t i;
    
    // Generate START condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Send EEPROM slave address with write operation
    I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // Send memory address (high byte first for 16-bit addressing)
    I2C_Write(MemAddr >> 8);
    I2C_Write(MemAddr);
    
    // Generate RESTART condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Send EEPROM slave address with read operation
    I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    // Read data bytes
    for(i = 0; i < NumByte - 1; i++) {
        pData[i] = I2C_ReceiveData(I2C1);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    }
    
    // Disable ACK before reading last byte
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    pData[i] = I2C_ReceiveData(I2C1);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    
    // Generate STOP condition
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    // Re-enable ACK for future communications
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return OK;
}

status EPROM_Write(uint16_t MemAddr,uint8_t SlaveAddress, uint8_t NumByte, uint8_t *pData)
{
	uint8_t i;
    
    // Generate START condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Send EEPROM slave address with write operation
    I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // Send memory address (high byte first for 16-bit addressing)
    I2C_Write(MemAddr >> 8);
    I2C_Write(MemAddr & 0xFF);
    
    // Send data bytes
    for(i = 0; i < NumByte; i++) {
        I2C_Write(pData[i]);
    }
    
    // Generate STOP condition
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    // Wait for EEPROM internal write cycle to complete (typically 5ms for EEPROMs like AT24Cxx)
    delay_ms(5);
    
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
