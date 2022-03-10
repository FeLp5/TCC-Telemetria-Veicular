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
//
//#include "hardware.h"
//#include "SHRC.h"
/*****************************************************************************/
 
/******************************************************************************
* Definicoes de pinos do microcontrolador
******************************************************************************/

/*****************************************************************************/

#define modo_zero 0
#define modo_um   1
#define modo_dois 2
#define modo_tres 3

#define TRIS_CS     TRISBbits.RB4
#define TRIS_SCK    TRISBbits.RB1
#define TRIS_SDI    TRISBbits.RB0
#define TRIS_SDO    TRISCbits.RC7

#define SPI_DATA_OUT LATCbits.LATC7 
#define CHIP_SELECT LATBbits.LATB4

/******************************************************************************
* Prototipos das funções
******************************************************************************/
void inicializa_SPI(void);

unsigned char leitura_SPI(void);

void escreve_SPI(unsigned char dado);



/*****************************************************************************/
#endif	/* SPI_H */

