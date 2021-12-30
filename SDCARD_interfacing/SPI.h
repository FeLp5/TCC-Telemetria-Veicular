/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Conceição de
			  Silva, Felipe Alves da
			  Souza, Ricardo de
			  
 *
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial 
 *
 *****************************************************************************/

#ifndef SPI_H
#define	SPI_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/ 

#include "hardware.h"
#include "SHRC.h"
/*****************************************************************************/
 
/******************************************************************************
* Definicoes de pinos do microcontrolador
******************************************************************************/

/*****************************************************************************/

#define modo_zero 0
#define modo_um   1
#define modo_dois 2
#define modo_tres 3

#define chip_select LATBbits.LATB4

/******************************************************************************
* Prototipos das funções
******************************************************************************/
void inicializa_SPI(unsigned char SPI_mode);

unsigned char recebe_dado_SPI(void);

char WriteSPI_(unsigned char dado);

unsigned char verifica_buffer_SPI(void);


/*****************************************************************************/
#endif	/* SPI_H */

