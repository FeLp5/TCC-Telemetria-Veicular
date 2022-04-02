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
 ******************************************************************************/



#ifndef SDCARD_H
#define	SDCARD_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/ 


/*****************************************************************************/
 
/******************************************************************************
* Definicoes de pinos do microcontrolador
******************************************************************************/
/*-----------------------------------------------------------------------
/  PFF - Low level disk interface modlue include file    (C)ChaN, 2014
/-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#include "ffconf.h"
#include "integer.h"
#include "diskio.h"
#include <p18f4550.h>
#include "hardware.h"
/*---------------------------------------*/


//Defines===========================    
//==================================    

typedef struct{
    unsigned char data[7];
    unsigned char hora[11];
    unsigned char LAT[13];
    unsigned char LONG[13];
}string_tel;
    
/******************************************************************************
* Prototipos das funções
******************************************************************************/
void sdcard_init(void);
//void escrita_sdcard(unsigned char *dado);
void escrita_sdcard(void);
void monta_sd(unsigned char index,  unsigned  char *dado);
BYTE response(void);
void proceed(void);
void dummy_clocks(unsigned char n);
void command(unsigned char CMD, unsigned long int arg, unsigned char CRC);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	/* _SDCARD_DEFINED */