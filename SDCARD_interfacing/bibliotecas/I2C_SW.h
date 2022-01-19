/******************************************************************************
 * Nome do Arquivo 	: I2C_SW.h
 *
 * Descrição       	: Implementa protocolo I2C
 *
 * Ambiente			: MPLAB, XC8 versão 1.45, PIC18F4550
 *
 * Responsável		: Torres, Weslley
 *
 * Versão/Data		: v00.01 - 16/08/2020 - versão inicial 
 *
 *****************************************************************************/
#ifndef  _I2C_SW_H_
#define  _I2C_SW_H_

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"

/*****************************************************************************/

/******************************************************************************
* Defines
******************************************************************************/
/*
* Porta do microcontrolador conectada ao pino de dados (SDA)
*/
#define	SDA_EEPROM_PORT									PORTAbits.RA4

/*
* Registrador de configuração da porta do microcontrolador conectada ao pino
* de dados (SDA)
*/
#define	SDA_EEPROM_PORT_MODE							TRISAbits.RA4

/*
* Porta do microcontrolador conectada ao pino de clock (SCL)
*/
#define	SCL_EEPROM_PORT									PORTAbits.RA3

/*
* Define valores de acknowledge do protocolo I2C
*/
#define ACK     1
#define NACK    0

/*
* Define valores para IN e OUT nas portas do microcontrolador
*/
#define INPUT   1
#define OUTPUT  0

/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/
void inicializa_i2c(void);
void start_condition_i2c(void);
void stop_condition_i2c(void);
void escrita_i2c(uint8_t dado);
uint8_t leitura_i2c(uint8_t i2c_ack);
void i2c_acktst(unsigned char val);
uint8_t leitura_byte_eeprom(uint16_t endereco);
void escreve_byte_eeprom(   uint16_t endereco, uint8_t valor);
/*****************************************************************************/

#endif