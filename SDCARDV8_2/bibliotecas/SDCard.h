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

#include "tff.h"
#include "integer.h"
#include "diskio.h"
#include <p18f4550.h>
#include "hardware.h"
/*---------------------------------------*/


//Defines===========================    
#define CHIP_SELECT LATBbits.LATB4
//==================================    
    

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */


//#define STA_NOINIT		0x01	/* Drive not initialized */
//#define STA_NODISK		0x02	/* No medium in the drive */
//
///* Card type flags (CardType) */
//#define CT_MMC				0x01	/* MMC ver 3 */
//#define CT_SD1				0x02	/* SD ver 1 */
//#define CT_SD2				0x04	/* SD ver 2 */
//#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
//#define CT_BLOCK			0x08	/* Block addressing */



/*****************************************************************************/


    
#define GO_IDLE_STATE           0x40

#define SEND_OP_COND            0x41
    
#define CHECK_VERSION           0x48

#define SET_BLOCK_SIZE          0x50
    
#define GO_READ_MBLK            0x52 // Ler Multimplos blocos

#define COMMAND_55              0x77 // Iniciar a comunicação cmd 55
    
#define COMMAND_41              0X69 //  Iniciar a comunicação cmd 41
    
#define BLCK_SIZE               0x00000200
    
/******************************************************************************
* Prototipos das funÃ§Ãµes
******************************************************************************/
//void init_SDcard();
//void envia_CMD16(void);
//unsigned char envia_CMD8(void);
//void envia_CMD1(void);
//void envia_CMD0(void);
void SDCard(void);
BYTE response(void);
void proceed(void);
void dummy_clocks(unsigned char n);
void command(unsigned char CMD, unsigned long int arg, unsigned char CRC);


//disk management
void sdc_reset(void);
DRESULT sdc_disk_read(BYTE *buff,DWORD sector, BYTE count);
DRESULT sdc_disk_write(const BYTE *p_buff, DWORD sector, BYTE count);
DSTATUS sdc_disk_initialize(void);
DSTATUS sdc_disk_ini_old(void);
DSTATUS sdc_disk_status(BYTE pdrv);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	/* _SDCARD_DEFINED */