/******************************************************************************
 * Nome do Arquivo 	: int_eeprom.h
 *
 * Descricao       	: Implementa a escrita e leitura da memoria EEPROM interna
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 01/08/2021 - versao inicial 
 *
 *****************************************************************************/  
#ifndef __INT_EEPROM_H_
#define	__INT_EEPROM_H_

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

void escreve_eeprom_imterna (unsigned int address, unsigned char data);
unsigned char leitura_eeprom_imterna(unsigned int address);
void escreve_string_eeprom_imterna(unsigned int address, unsigned char *data);

/*****************************************************************************/




#endif	/* __INT_EEPROM_H_ */

