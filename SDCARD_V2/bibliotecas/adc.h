/******************************************************************************
 * Nome do Arquivo 	: adc.h
 *
 * Descri��o       	: Implementa a controle do conversor AD
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Respons�vel		: Torres, Weslley
 *
 * Vers�o/Data		: v00.01 - 20/08/2020 - versao inicial
 *
 *****************************************************************************/
#ifndef __ADC_H
#define __ADC_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necess�rios nesse arquivo)
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das fun��es
******************************************************************************/

void inicializa_adc(void);

unsigned int leitura_adc(unsigned char canal);

/*****************************************************************************/

#endif


