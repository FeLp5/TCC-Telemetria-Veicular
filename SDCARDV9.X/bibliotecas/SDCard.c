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
BYTE filename[15] = "tel.txt"; //USE SMALLER ARRAY SIZE /testmapp/testtext 
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
char buff[45] = "LONG: -1.303930933 , LAT: -2.3309330933\n";

void sdcard_init(void) 
{
//    T0CONbits.TMR0ON = 0;
    FRESULT FResult;
   
    WORD bw;
    UINT br;
    DWORD tete;
    int i = 0;
    /*Mount FUNCTION=============================================================*/
    proceed();
    f_mount(&fs, "", 0); //Mount FileSystem
    f_open(&fil, filename, FA_OPEN_ALWAYS);
    FResult = f_expand(&fil, 32, 0);
    f_close(&fil);
    
    posicao_cursor_lcd(1,0);
    escreve_inteiro_lcd(FResult);
    __delay_ms(2000);
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

void escrita_sdcard(void) 
{
    static unsigned char count;
     f_mount(&fs, "", 0);
   	if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE | FA_OPEN_APPEND) == FR_OK) 
    {	/* Open or create a file */
        posicao_cursor_lcd(2,11);
        escreve_inteiro_lcd(f_size(&fil));
        f_puts(buff, &fil);	/* Write data to the file */							/* Close the file */
        f_close(&fil);	
    } 

}