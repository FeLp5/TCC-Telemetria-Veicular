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
//#include "SPI.h"
#include "hardware.h"
#include "integer.h"
#include "display_lcd.h"
#include "ff.h"
#include "diskio.h"
#include "SHRC.h"
//#include "sw_uart.h"

//global variables

unsigned char *rd;
unsigned char Result;
BYTE s;
DWORD fileSize;
BYTE txt[20];
BYTE data_buffer_32[32];


// File to read================================================================= 
BYTE fileName[15] = {"teste.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
BYTE folder[48] = {""}; 
//==============================================================================

extern void readover(int);

BYTE response(void)
{
    
    unsigned char buff;
    CHIP_SELECT = 0;//CS low   
    buff= recebe_dado_SPI();   //read buffer (R1) should be 0x01 = idle mode   
//    CHIP_SELECT = 1;//CS high
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
        escreve_dado_SPI(0XFF);
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
    CHIP_SELECT = 0; //CS Low
    escreve_dado_SPI(0xFF); // Give Time For SD_CARD To Proceed
    CHIP_SELECT = 1; //CS High
}

//extern  void check();


/******************************************************************************
 * Funcao:		command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
 * Entrada:		unsigned char CMD, unsigned long int arg, unsigned char CRC
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza o envio do comando para o SDCard
 *****************************************************************************/

void command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
{
    unsigned char argument;
    escreve_dado_SPI(0xFF);
    escreve_dado_SPI(CMD);
    argument = arg>>24;
    escreve_dado_SPI(argument);
    argument = arg>>16;
    escreve_dado_SPI(argument);
    argument = arg>>8;
    escreve_dado_SPI(argument);
    argument = arg;
    escreve_dado_SPI(argument);
    escreve_dado_SPI(CRC);
}



/******************************************************************************
 * Funcao:		SDCard(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do sistema de arquivos, cria o arquivo, escreve no arquivo
 * e fecha o arquivo
 *****************************************************************************/
FATFS fs;
void SDCard(void) 
{
    T0CONbits.TMR0ON = 0;
//    inicializa_SPI(0,3,1);
    FRESULT FResult;
    FIL fil;
    WORD br, bw;
    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Attempting to");
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("mount file sys");
//    SPI_DATA_OUT = 0;
//    __delay_ms(2000);
//    SPI_DATA_OUT = 1;
//    __delay_ms(2000);
    LIMPA_DISPLAY();
    
    /*READ FUNCTION=============================================================*/
//    proceed();
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("montando sistema");
	if ((FResult = f_mount(&fs, "", 1)) == FR_OK) {	/* Mount SD */

        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("Abrindo arquivo");
		if ((FResult = f_open(&fil, "teste.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE)) == FR_OK) {	/* Open or create a file */
            
             posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("lendo tamanho");
			if ((fil.fsize != 0) && (f_lseek(&fil, fil.fsize) != FR_OK)) goto endSD;	/* Jump to the end of the file */
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("escrevendo");
			f_write(&fil, "Hello world!\r\n", 14, &bw);	/* Write data to the file */
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("fechando");
			endSD: f_close(&fil);								/* Close the file */
		}
        posicao_cursor_lcd(1,0);
        escreve_inteiro_lcd(FResult);
	}
    posicao_cursor_lcd(1,0);
    escreve_inteiro_lcd(FResult);
	while (1) {
		// Add your application code
	}
}		
 