///******************************************************************************
// * Nome do Arquivo 	: SDCard.h
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB, XC8 versao v2.30, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceicao de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/



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

//#include "ffconf.h"
#include "integer.h"
#include "diskio.h"
#include <pic18f4550.h>
#include "hardware.h"
/*---------------------------------------*/


//Defines===========================    
//==================================    

typedef struct{
    unsigned char data[7];
    unsigned char hora[11];
    unsigned char data_name[2];
    unsigned char hora_name[2];
}string_tel;



    
/******************************************************************************
* Prototipos das funções
******************************************************************************/
void sdcard_init(void);
void escrita_sdcard(void);
unsigned char *leitura_sdcard(unsigned char num_spot);
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