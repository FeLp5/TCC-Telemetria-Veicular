/******************************************************************************
 * Nome do Arquivo 	: I2C_SW.c
 *
 * Descricao       	: Implementa protocolo I2C
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: versao inicial - 01/10/2020 - Versão Inicial 
 *					  FATEC Santo Andre - TAPM
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "I2C_SW.h"

/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

void delay_i2c(void);

/*****************************************************************************/


/******************************************************************************
 * Funcao:		void delay_i2c(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Cria "delay" entre comandos I2C.
 *****************************************************************************/
void delay_i2c(void)
{
  #asm  
      NOP
      NOP
      NOP
      NOP
 #endasm; 
}

/******************************************************************************
 * Funcao:		void inicializa_i2c(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Inicializa portas SCL e SDA do I2C.
 *****************************************************************************/
void inicializa_i2c(void)
{
  SDA_EEPROM_PORT_MODE = OUTPUT;
  SCL_EEPROM_PORT = 1;
  SDA_EEPROM_PORT = 1;
}

/******************************************************************************
 * Funcao:		void start_condition_i2c(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Cria condição de start para I2C.
 *****************************************************************************/
void start_condition_i2c(void)
{
  SCL_EEPROM_PORT = 1;
  SDA_EEPROM_PORT = 1;
  delay_i2c();
  SDA_EEPROM_PORT = 0;
  delay_i2c();
}

/******************************************************************************
 * Funcao:		void stop_condition_i2c(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Cria condição de stop para I2C.
 *****************************************************************************/
void stop_condition_i2c(void)
{
  SCL_EEPROM_PORT = 1;
  SDA_EEPROM_PORT = 0;
  delay_i2c();
  SDA_EEPROM_PORT = 1;
  delay_i2c();
}

/******************************************************************************
 * Funcao:		void escrita_i2c(uint8_t dado)
 * Entrada:		dado    (uint8_t) - Byte a ser escrito no I2C
 * Saida:		Nenhuma (void)
 * Descricao:	Envia um byte atraves do I2C.
 *****************************************************************************/
void escrita_i2c(uint8_t dado)
{
  uint8_t contador;
  
  SCL_EEPROM_PORT = 0;
  
  for(contador = 0; contador < 8; contador++)
  {
    SDA_EEPROM_PORT = ((dado>>(7-contador))& 0x01);
    SCL_EEPROM_PORT = 1;
    delay_i2c();
    SCL_EEPROM_PORT = 0;
  }	
  delay_i2c();
  SCL_EEPROM_PORT = 1;
  delay_i2c();
  SCL_EEPROM_PORT = 0;
}


/******************************************************************************
 * Funcao:		uint8_t leitura_i2c(uint8_t i2c_ack)
 * Entrada:		i2c_ack   (uint8_t) - I2C acknowledge 
 * Saida:		dado_lido (uint8_t) - byte lido no I2C 
 * Descricao:	Realiza a leitura de um byte atraves do I2C.
 *****************************************************************************/
uint8_t leitura_i2c(uint8_t i2c_ack)
{
  uint8_t contador;
  uint8_t dado_lido = 0;

  SCL_EEPROM_PORT = 0;
  SDA_EEPROM_PORT_MODE = INPUT;
  SDA_EEPROM_PORT = 1;
  for(contador=0;contador < 8;contador++)
  {
    SCL_EEPROM_PORT = 1;
    delay_i2c();
    dado_lido|=(SDA_EEPROM_PORT<<(7-contador));
    SCL_EEPROM_PORT = 0;
  }
  SDA_EEPROM_PORT_MODE = OUTPUT;
  if(i2c_ack)
  {
    SDA_EEPROM_PORT = 0;
  }
  else
  {
	SDA_EEPROM_PORT = 1;
  }
  delay_i2c();
  SCL_EEPROM_PORT = 1;
  delay_i2c();
  SCL_EEPROM_PORT = 0;

  return dado_lido;
}

/******************************************************************************
 * Funcao:		uint8_t leitura_byte_eeprom(uint16_t endereco)
 * Entrada:		endereco (uint16_t) - endereço na EEPROM para leitura
 * Saida:		dado_lido (uint8_t) - byte lido na EEPROM
 * Descricao:	Realiza a leitura de um byte na EEPROM.
 *****************************************************************************/
uint8_t leitura_byte_eeprom(uint16_t endereco)
{
  uint8_t dado_lido;
  uint8_t endereco_sup;
  uint8_t endereco_inf;
  
  endereco_sup = (endereco&0xFF00)>>8;
  endereco_inf = endereco&0x00FF;

  start_condition_i2c();	
  escrita_i2c(0xA0);
  escrita_i2c(endereco_sup);
  escrita_i2c(endereco_inf);

  start_condition_i2c();
  escrita_i2c(0xA1);
  dado_lido = leitura_i2c(ACK);
  stop_condition_i2c();

  return dado_lido;	
}

/******************************************************************************
 * Funcao:		void escreve_byte_eeprom(   uint16_t endereco, 
 *                                          uint8_t valor)
 * Entrada:		
 *              endereco (uint16_t) - endereço na EEPROM para leitura
 *              valor ( uint8_t)    - valor a ser escrito na EEPROM
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a escrita de um byte na EEPROM.
 *****************************************************************************/
void escreve_byte_eeprom(   uint16_t endereco, uint8_t valor)
{
  uint8_t dado_recebido;
  uint8_t endereco_sup;
  uint8_t endereco_inf;
  uint8_t contador = 0;

  dado_recebido = valor;
  endereco_sup=(endereco&0xFF00)>>8;
  endereco_inf=endereco&0x00FF;

  do
  {
    start_condition_i2c();
    escrita_i2c(0xA0);
    escrita_i2c(endereco_sup);
    escrita_i2c(endereco_inf);
    escrita_i2c(dado_recebido);
    stop_condition_i2c();

    contador++;
  }
  while((dado_recebido != leitura_byte_eeprom(endereco))&&(contador < 10));
}
