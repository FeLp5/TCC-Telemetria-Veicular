/******************************************************************************
 * Nome do Arquivo 	: SHRC.h
 *
 * Descricao       	: Implementa a controle do registrador de deslocamento
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 20/08/2020 - versao inicial 
 *
 *****************************************************************************/
 
#ifndef SHRC_H
#define	SHRC_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/ 

#include "hardware.h"

/*****************************************************************************/
 
/******************************************************************************
* Definicoes de pinos do microcontrolador
******************************************************************************/
#define SHIFT_CLOCK         LATBbits.LATB6 
#define SPI_DATA_OUT        LATBbits.LATB7
#define SPI_DATA_IN         PORTBbits.RB5
#define CHIP_SELECT         LATBbits.LATB4


#define PORT_CHIP_SELECT    TRISBbits.RB4
#define PORT_DATA_IN        TRISBbits.RB5
#define PORT_CLOCK          TRISBbits.RB6
#define PORT_DATA_OUT       TRISBbits.RB7

/*****************************************************************************/


/******************************************************************************
* Prototipos das funções
******************************************************************************/
void controle_shrc (void);

void inicializa_shrc(void);

void escreve_dado_SPI(unsigned char data_to_shrc);

void shrc_seta_bit(unsigned char posicao_bit);

void shrc_apaga_bit(unsigned char posicao_bit);

unsigned char recebe_dado_SPI();
/*****************************************************************************/
#endif	/* SHRC_H */

