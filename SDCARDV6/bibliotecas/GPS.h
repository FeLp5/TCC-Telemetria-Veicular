/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Souza, Deivide ConceiÃ§Ã£o de
			  Silva, Felipe Alves da
			  Souza, Ricardo de
			  
 *
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial 
 ******************************************************************************/



#ifndef GPS_H
#define	GPS_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/ 
#include <p18f4550.h>

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



/******************************************************************************
     * Defines
     ******************************************************************************/

#define GGA_BUFFER_SIZE 80
#define GGA_POINTERS_SIZE 20

#define RMC_BUFFER_SIZE  70
#define RMC_POINTERS_SIZE 12

/******************************************************************************
* Prototipos das funÃ§Ãµes
******************************************************************************/
void tratamento_uart(unsigned char received_char); 
void gps(void);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	/* _SDCARD_DEFINED */