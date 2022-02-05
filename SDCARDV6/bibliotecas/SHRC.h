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
#define SHIFT_DATA          LATBbits.LATB7
#define SHIFT_LATCH         LATBbits.LATB5

/*****************************************************************************/


/******************************************************************************
* Prototipos das funções
******************************************************************************/
void controle_shrc (void);

void inicializa_shrc(void);

void shrc_seta_bit(unsigned char posicao_bit);

void shrc_apaga_bit(unsigned char posicao_bit);

void shrc_recebe_byte(unsigned char dado_recebido);
/*****************************************************************************/
#endif	/* SHRC_H */

