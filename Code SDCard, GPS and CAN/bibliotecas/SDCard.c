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
#include "pff.h"
#include "diskio.h"
//#include "sw_uart.h"

//global variables

unsigned char *rd;
unsigned char Result;
BYTE s;
DWORD fileSize;
BYTE txt[20];
BYTE data_buffer_32[32];


// File to read================================================================= 
BYTE fileName[10] = {"check.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
BYTE folder[48] = {""}; 
//==============================================================================

extern void readover(int);

BYTE response(void)
{
    
    unsigned char buff;
    chip_select = 1;//CS low   
    buff= recebe_dado_SPI();   //read buffer (R1) should be 0x01 = idle mode   
    chip_select = 0;//CS high
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
    for(i=0;i<n;i++)
    {
        chip_select = 0;
        WriteSPI_(0XFF);
        chip_select = 1;
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
    chip_select = 1; //CS Low
    WriteSPI_(0xFF); // Give Time For SD_CARD To Proceed
    chip_select = 0; //CS High
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
    LATBbits.LATB3 = ~LATBbits.LATB3;
    unsigned char argument = arg>>24;
    WriteSPI_(0xFF);
    WriteSPI_(CMD);
    WriteSPI_(argument);
    argument = arg>>16;
    WriteSPI_(argument);
    argument = arg>>8;
    WriteSPI_(argument);
    argument = arg;
    WriteSPI_(argument);
    WriteSPI_(CRC);
}



/******************************************************************************
 * Funcao:		SDCard(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do sistema de arquivos, cria o arquivo, escreve no arquivo
 * e fecha o arquivo
 *****************************************************************************/

void SDCard(void) 
{
    FRESULT FResult;
    FATFS fs;
    WORD br;
    
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("Attempting to");
    posicao_cursor_lcd(2,0);
    escreve_frase_ram_lcd("mount file sys");
    LIMPA_DISPLAY();
    
    /*READ FUNCTION=============================================================*/
    proceed();
	if((FResult = pf_mount(&fs)) == FR_OK )
	{
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("syst inicialized");
        
		//open file=======================================
		FResult = pf_open("check.txt"/*fileName*/);
        //=================================================
		if( FResult == FR_OK )
		{
			// Read 31 bytes from the file                        
            if( (FResult = pf_write("SD_Card", 5, &br)) == FR_OK )
			{
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("syst inicialized");
			} 
			else
			{
                LIMPA_DISPLAY();
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("SDCard error");
				while( 1 );
			}
            LIMPA_DISPLAY();
            
            //Starts to read the file for writing
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("lendo arquivo");

			// read file and print it until it ends
			do
			{
                readover(1); //Set Flag=1 in diskio.c before reading Card Data
                
				// Read 31 bytes from the file
				if( (FResult = pf_read(data_buffer_32, 31, &br)) == FR_OK )
				{
					// putsUSART needs a C-string (NULL terminated)
					data_buffer_32[br] = 0;
                    break;
					//putsUART(data_buffer_32);
				}
				else
				{
                    //write on the screen if it got an error
                    LIMPA_DISPLAY();
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("Erro tentando");
                    posicao_cursor_lcd(2,0);
                    escreve_frase_ram_lcd("ler o arquivo");
					
					while( 1 );
				}
			} while( br == 31 );	// if the pf_Read reads less then 31 bytes the file has ended
		}
		else 
		{
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Erro tentando");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("abrir o arquivo");
			while( 1 );
		}
	}
	else
	{
    //Write a error message if the file wasnt oppened 
		while( 1 );
	}

    
    
  	// do a directory listing and list all files on the SD-card ========

    
    
//============================================================================
	//scan_files(folder);????????????????? function depreciated ?
		
	// unmount drive
//	putsUART("\r\nAttempting to UNmount file system.\r\n");
	
	//FResult = pf_mount(NULL); ??????????????????????????????
	
//============================================================================  
    
	if( FResult != FR_OK )
	{
    //write on the screen it wanst possible to create     
//		putsUART("\r\nError trying to UNmount filesystem; ");
//		WriteUART(FResult + 0x30);
//		putsUART(".\r\n");
		while( 1 );
	}	

//	putsUART("\rFile system succesfully unmounted.\r\n");
	while(1);
/*WRITE FUNCTION*/
}		
 