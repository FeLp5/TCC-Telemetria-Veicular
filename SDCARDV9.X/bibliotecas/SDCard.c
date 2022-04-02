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
BYTE filename[15] = "teste.txt";
FATFS fs;
FIL fil;
//fat_time *time;
string_tel string_dado;
unsigned char data_hoje;
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
    desliga_uart();
    inicializa_SPI();
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
    static unsigned char flag;
    PORTBbits.RB3 = 0;
//    __delay_us(30);
    desliga_uart();
    inicializa_SPI();
    f_mount(0,&fs);
    

//    if(data_hoje)
//    {
        if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE ) == FR_OK)  /* Open or create a file */
        {	

            f_lseek(&fil, fsize(&fil));
            posicao_cursor_lcd(2,0);
            escreve_inteiro_lcd(fsize(&fil));
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd(string_dado.hora);
            fprintf(&fil, "\n%s ; %s ; %s ; %s ;", string_dado.hora, string_dado.data,string_dado.LAT, string_dado.LONG);

            /* Close the file */
            f_close(&fil);	
        }
//    }
    PORTBbits.RB3 = 1; 
    desliga_SPI();
    inicializa_uart();
    
    return;
}


/******************************************************************************
 * Funcao:		void monta_sd(unsigned char index, unsigned char *dado, float dado_localizacao)
 * Entrada:		unsigned char index, unsigned char *dado, float dado_localizacao
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a transferencia das informacoes para gravar no sdcard
 *****************************************************************************/


void monta_sd(unsigned char index, unsigned char *dado)
{
    unsigned char i, size;
    size = strlen(dado);
    switch(index)
    {
        case 0:
            for(i=0; i<6;i++)
            {
                string_dado.hora[i] = *dado;
                dado++;
            }
        break;
        
        case 1:
            for(i=0; i<6;i++)
            {
                string_dado.data[i] = *dado;
                dado++;
            }
            if(string_dado.data)
            {
                data_hoje = 1;
//                strcpy(filename, string_dado.data);
//                strcat(filename, ".txt");
            }
        break;
        
        case 2:
            for(i=0; i<13;i++)
            {
                string_dado.LAT[i] = *dado;
                dado++;
            }
        break;
        
       case 3:
//           string_dado.LONG = dado_localizacao;
            for(i=0; i<13;i++)
            {
                string_dado.LONG[i] = *dado;
                dado++;
            }
        break;
        
        case 4:
//           string_dado.LONG = dado_localizacao;
            for(i=0; i<size;i++)
            {
                string_dado.LONG[i] = *dado;
                dado++;
            }
        break;
        
    }
    
    return;
}