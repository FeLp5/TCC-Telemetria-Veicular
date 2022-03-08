///******************************************************************************
// * Nome do Arquivo 	: SPI.c
// *
// * Descricao       	: Implementa o controle protocolo SPI
// *
// * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceição de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "SDCard.h"
#include "SPI.h"
#include "hardware.h"
#include "integer.h"
#include "display_lcd.h"
#include "ff.h"
#include "diskio.h"
#include "SHRC.h"
//#include "sw_uart.h"

//global variables





//=================







//unsigned char *rd;
//unsigned char Result;
//BYTE s;
//DWORD fileSize;




// File to read================================================================= 
BYTE filename[15] = "check.txt"; //USE SMALLER ARRAY SIZE /testmapp/testtext 
//BYTE folder[48] = {""}; 
//==============================================================================

extern void readover(int);
void sdc_wait_ready(void);

FRESULT open_append (FIL* fp,const char* path);

BYTE response(void)
{
    unsigned char buff;
    CHIP_SELECT = 0;//CS low   
    buff= ReadSPI_();   //read buffer (R1) should be 0x01 = idle mode   
    return buff;
}

/******************************************************************************
 * Funcao:		dummy_clocks(unsigned char n)
 * Entrada:		unsigned char n
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para que o SDCard e o 
 * Microcontrolador estejam em sincronia
 *****************************************************************************/

void dummy_clocks(unsigned char n)
{
    unsigned int i;
    __delay_ms(1);
//    LATB5 = 1;
    for(i=0;i<n;i++)
    {
        CHIP_SELECT = 1;
        ReadSPI_();
        CHIP_SELECT = 0;
    }
}


/******************************************************************************
 * Funcao:		proceed(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para manter o SDCard ativo
 * ou para ativar
 *****************************************************************************/
void proceed(void)
{
    CHIP_SELECT = 0; //CS Low;
    WriteSPI_(0xFF); // Give Time For SD_CARD To Proceed
    CHIP_SELECT = 1; //CS High;
}

//extern  void check();


static
BYTE wait_ready (void)
{
	BYTE res;
	DWORD timeout = 0x7FFF;
	char msg[10];
	
	do
		res = ReadSPI_();
	while ((res != 0xFF) && (--timeout));
	
	return res;
}
/******************************************************************************
 * Funcao:		command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
 * Entrada:		unsigned char CMD, unsigned long int arg, unsigned char CRC
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza o envio do comando para o SDCard
 *****************************************************************************/

void command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
{
    unsigned char argument;
    
    if (wait_ready() != 0xFF) 
    {
        while(wait_ready() != 0xFF)
        {
            dummy_clocks(10);
        }
    }
    
    WriteSPI_(0xFF);
    WriteSPI_(CMD);
    argument = arg>>24;
    WriteSPI_(argument);
    argument = arg>>16;
    WriteSPI_(argument);
    argument = arg>>8;
    WriteSPI_(argument);
    argument = arg;
    WriteSPI_(argument);
    WriteSPI_(CRC);
    
//    if ( CMD == 12 )
//    {
//        response();
//    }
		
}



/******************************************************************************
 * Funcao:		SDCard(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do sistema de arquivos, cria o arquivo, escreve no arquivo
 * e fecha o arquivo
 *****************************************************************************/
FATFS fs;
 FIL fil;
//FIL fil;
char buff[10] = "felipe\n";

void SDCard(void) 
{
    T0CONbits.TMR0ON = 0;
    
//    inicializa_SPI(0,3,1);
    FRESULT FResult;
   
    WORD bw;
    UINT br;
    int i = 0;
//   
    
    /*READ FUNCTION=============================================================*/
    proceed();
    
    	if (f_mount(&fs, "", 1) == FR_OK) {	/* Mount SD */
//    while(i<2){
		if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Open or create a file */

//			if ((fil.fsize != 0) && (f_lseek(&fil, fil.fsize) != FR_OK)) goto endSD;	/* Jump to the end of the file */
//            f_write(&fil, "FELIPE � LINDO DMS!", 20, &bw);	/* Write data to the file */
//            f_close(&fil);if ((fil.fsize != 0) && (f_lseek(&fil, fil.fsize) != FR_OK)) goto endSD;	/* Jump to the end of the file */
            open_append(&fil, filename);
			f_write(&fil, "felipe!", 20, &bw);	/* Write data to the file */

//			endSD: f_close(&fil);								/* Close the file */
            f_close(&fil);	
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Escrita OK");
		}
//        i++;
//	}
       
  }  
//    __delay_ms(1);
//    FResult = f_mount(&fs, "", 1);
//    if(FResult == FR_OK)
//    {
//    
//                posicao_cursor_lcd(1,0);
//                escreve_frase_ram_lcd("esc arquivo");
////                f_open(&fil, filename, FA_CREATE_NEW);
//                f_close(&fil);
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("Adicionando");
////                open_append(&fil, filename);
//                f_write(&fil, buff, 6, &bw);
//                f_close(&fil);
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("OK");
//            
//        

   
//    }
//    else
//    {
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(FResult);
//    }
    return;
}

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS );
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}