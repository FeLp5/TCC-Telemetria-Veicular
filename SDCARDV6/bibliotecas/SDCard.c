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
#include "pff.h"
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
BYTE fileName[15] = {"telemetria.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
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

void SDCard(void) 
{
    T0CONbits.TMR0ON = 0;
//    inicializa_SPI(0,3,1);
    FRESULT FResult;
    FATFS fs;
    WORD br = ".";
    
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
    proceed();
	if((FResult = pf_mount(&fs)) == FR_OK )
	{
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("syst initialized");
        
		//open file=======================================
		FResult = pf_open(fileName/*fileName*/);
        posicao_cursor_lcd(1,0);
        escreve_inteiro_lcd(FResult);
        __delay_ms(2000);
        //=================================================
		if( FResult == FR_OK )
		{
            __delay_ms(1000);
			// Read 31 bytes from the file                        
            if( (FResult = pf_write("SCard", 5, &br)) == FR_OK )
			{
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("Escrevendo");
                __delay_ms(2000);
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
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("To aqui");
				// Read 31 bytes from the file
				if( (FResult = pf_read(data_buffer_32, 31, &br)) == FR_OK )
				{
					// putsUSART needs a C-string (NULL terminated)
                    posicao_cursor_lcd(2,0);
                    escreve_frase_ram_lcd("Sistema OK");
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
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("Preso");
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
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("ERRO!");
		while( 1 );
        T0CONbits.TMR0ON = 1;
	}

    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("N�o to Preso");
    
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
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("Erro filesyst");
//		putsUART("\r\nError trying to UNmount filesystem; ");
//		WriteUART(FResult + 0x30);
//		putsUART(".\r\n");
		while( 1 );
	}	

//	putsUART("\rFile system succesfully unmounted.\r\n");
	while(1){
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("to Preso aq");
    }
/*WRITE FUNCTION*/
}		
 