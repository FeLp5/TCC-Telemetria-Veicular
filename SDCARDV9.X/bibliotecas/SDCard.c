///******************************************************************************
// * Nome do Arquivo 	: SDCard.c
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB, XC8 versao 1.45, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceicao de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/

#include "SDCard.h"
#include "SPI.h"
#include "hardware.h"
#include "integer.h"
#include "display_lcd.h"
#include "tff.h"
#include "diskio.h"
#include "SHRC.h"
#include <string.h>
#include "GPS.h"
#include "uart.h"

/******************************************************************************
* Variaveis Globais
******************************************************************************/

// File to read================================================================= 
BYTE filename[15] = "gps2.txt";
FATFS fs;
FIL fil;
fat_time *time;


unsigned char data_buffer_sd[100];
//char buff[] = "0, -23.303930933 , -42.3309330933, 800, 45, 12764, 0";



/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void sdc_wait_ready(void);



/******************************************************************************
 * Funcao:		BYTE response(void)
 * Entrada:		Nenhuma (void)
 * Saida:		BYTE (void) (BYTE -> unsigned char)
 * Descricao	Realiza o processo de leitura do sdcard, selecionando e lendo o
 * buffer do SPI
 *****************************************************************************/
BYTE response(void)
{
    unsigned char buff;
    CHIP_SELECT = 0;//CS low   
    buff= leitura_SPI();   //read buffer (R1) should be 0x01 = idle mode   
    return buff;
}

/******************************************************************************
 * Funcao:		dummy_clocks(unsigned char n)
 * Entrada:		unsigned char n
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para que o sdcard e o 
 * Microcontrolador estejam em sincronia. 
 *****************************************************************************/
void dummy_clocks(unsigned char n)
{
    unsigned int i;
    __delay_ms(1);
    for(i=0;i<n;i++)
    {
        CHIP_SELECT = 1;
        leitura_SPI();
        CHIP_SELECT = 0;
    }
}

/******************************************************************************
 * Funcao:		proceed(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para manter o sdcard ativo
 *****************************************************************************/
void proceed(void)
{
    CHIP_SELECT = 0; //CS Low;
    escreve_SPI(0xFF); // Give Time For SD_CARD To Proceed
    CHIP_SELECT = 1; //CS High;
}

/******************************************************************************
 * Funcao:		static BYTE wait_ready (void)
 * Entrada:		Nenhuma (void)
 * Saida:		static BYTE (static unsigned char)
 * Descricao:	Verifica se o sdcard esta pronto para receber informacao
 *****************************************************************************/
static BYTE wait_ready (void)
{
	BYTE res;
	DWORD timeout = 0x7FFF;
	char msg[10];
	
	do
		res = leitura_SPI();
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
    escreve_SPI(0xFF);
    escreve_SPI(CMD);
    argument = arg>>24;
    escreve_SPI(argument);
    argument = arg>>16;
    escreve_SPI(argument);
    argument = arg>>8;
    escreve_SPI(argument);
    argument = arg;
    escreve_SPI(argument);
    escreve_SPI(CRC);
}




/******************************************************************************
 * Funcao:		sdcard_init(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do sistema de arquivos, inicializa o sd card 
 * criando o arquivo para escrita. Se o arquivo ja existe ele apenas abre e fecha
 * em seguida.
 *****************************************************************************/
void sdcard_init(void) 
{
    inicializa_SPI();
    desliga_uart();
    PORTBbits.RB3 = 0;
    FRESULT FResult;
    WORD bw;
    proceed();
    FResult = f_mount(0,&fs); //Mount FileSystem
    if(FResult != FR_OK)
    {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Erro ao montar");
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("o Sistema de arquivo");
    }
    FResult = f_open(&fil, filename, FA_OPEN_ALWAYS);
       
    if(FResult != FR_OK)
    {
        while((FResult = f_open(&fil, filename, FA_OPEN_ALWAYS)) != FR_OK)
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Erro ao criar");
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("o arquivo");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("Verifique");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("o cartao");
            
        }
    }
    f_close(&fil);
    return;
}


/******************************************************************************
 * Funcao:		void escrita_sdcard(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a escrita dos arquivos no sdcard
 *****************************************************************************/
void escrita_sdcard(void) 
{
    unsigned char i;
    desliga_uart();
//    TRISCbits.RC7  = 0;
    PORTBbits.RB3 = 0;
    T0CONbits.TMR0ON = 0;
    f_mount(0,&fs);
    LIMPA_DISPLAY();
   	if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE ) == FR_OK)  /* Open or create a file */
    {	

        f_lseek(&fil, fsize(&fil));
        posicao_cursor_lcd(2,0);
        escreve_inteiro_lcd(fsize(&fil));
        
        fprintf(&fil, "\n%s",  data_buffer_sd);
        
        /* Close the file */
        f_close(&fil);	
    } 
    inicializa_uart();
//    TRISCbits.RC7  = 1;
    PORTBbits.RB3 = 1;
    T0CONbits.TMR0ON = 1;
    return;
}


void monta_sd(unsigned char index, const unsigned char *dado)
{
    unsigned char i, j, count;
    unsigned char data_string[];
    
    if(!index)
    {
        strcat(data_string, dado);
        strcat(data_string, "|");
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd(data_string);
    }
    else if(index==1)
    {
        for(i=0; data_string[i] != '\0'; i++)
        {
            if(data_string[i] == ';')
            {
                count++;
                if(count==index)
                {
                    i++;
                    data_string[i] = *dado;
                    dado++;
                }
            }
        }
    }
    strcpy(data_buffer_sd, data_string);
    
    return;
}