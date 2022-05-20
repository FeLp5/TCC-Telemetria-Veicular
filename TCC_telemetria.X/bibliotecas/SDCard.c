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
#include <string.h>
#include "GPS.h"
#include "uart.h"

/******************************************************************************
* Variaveis Globais
******************************************************************************/
BYTE filename[20];
FATFS fs;
FIL fil;
BYTE dado_arquivo[20];
string_tel string_dado;
unsigned char data_hoje;


static unsigned char lt[12];
static unsigned char lo[13];
static unsigned char fence[6];
static unsigned char vel[6];
static unsigned char hora[7];
static unsigned char data[6];

/******************************************************************************
* Prototipos das funcoes
******************************************************************************/

void sdc_wait_ready(void);
void data_nome (unsigned char *p_buff);
void hora_nome (unsigned char *p_buff);

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
	
	do{
		res = leitura_SPI();
    }while ((res != 0xFF) && (--timeout));
	
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
void escrita_sdcard() 
{
    unsigned char *fix_gps;
    unsigned char i, j;
    static unsigned char f_fix = 0;
    WORD bw;
//    PORTBbits.RB3 = 0;
//    dados_gps_to_sd();
    f_mount(0,&fs);
    if(!f_fix)
    {  
        if(data[0] != ' ' && hora[0] != ' ')
        {
            data_nome(data);
            hora_nome(hora);
//            strcpy(filename, "teste3");;
            strcat(filename, ".tlm");
            f_fix = 1;
        }
    } 
    
    if(f_fix)
    {
        if (f_open(&fil, filename, FA_WRITE ) == FR_OK)  /* Open or create a file */
        {	

            f_lseek(&fil, fsize(&fil));
            fprintf(&fil, "v%s;lt%s;lo%s;", vel, lt, lo);
//            fprintf(&fil, "lt%s;lo%s;", lt, lo);
            fprintf(&fil, "r0;c0;k0;");
            fprintf(&fil, "h%s;dN/A;\n", hora);
            fprintf(&fil, "f%s;\n", fence);
            
            /* Close the file */
            f_close(&fil);	
        }
        else
        {
            f_open(&fil, filename, FA_CREATE_ALWAYS );
            f_close(&fil);

        }
    }
//    PORTBbits.RB3 = 1; 
    
    return;
}



/******************************************************************************
 * Funcao:		void leitura_sdcard(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura dos arquivos no sdcard
 *****************************************************************************/
char *leitura_sdcard(unsigned char num_spot) 
{
    
    static unsigned char size = 88;
    WORD br;
    unsigned int offset = 0;
    
    
    PORTBbits.RB3 = 0;
    desliga_uart();
    inicializa_SPI();
    f_mount(0,&fs);
    
    if (f_open(&fil, "extfence.txt", FA_READ ) == FR_OK)  /* Open or create a file */
    {	

        f_read(&fil, dado_arquivo, size, &br);
        /* Close the file */
        f_close(&fil);	
    }
    PORTBbits.RB3 = 1; 
    desliga_SPI();
    inicializa_uart();
    
    return (dado_arquivo);
}








/******************************************************************************
 * Funcao:		void monta_sd(unsigned char index, unsigned char *dado, float dado_localizacao)
 * Entrada:		unsigned char index, unsigned char *dado
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
            for(i=0; i<7;i++)
            {
                if(*dado != '\0')
                {
                   hora[i] = *dado; 
                }
                dado++;
            }
        break;
        
        case 1:
            for(i=0; i<6;i++)
            {
                if(*dado != '\0')
                {
                    data[i] = *dado;  
                }
                dado++;
            }
        break;
        
        case 2:      
            for(i=0; i<11;i++)
            {
                if(*dado != '\0')
                {
                    lt[i] = *dado;
                }
                dado++;
            }
        break;
        
       case 3:
            for(i=0; i<12;i++)
            {
                if(*dado != '\0')
                {
                    lo[i] = *dado;
                }
                dado++;
            }
        break;
        
        case 4:
            //monta string do estado do fence
            for(i=0; i<6;i++)
            {
                if(*dado != ' ' && *dado != '\0')
                {
                    fence[i] = *dado;
                }
                dado++;
            }
        break;
                
        case 5:
            for(i=0; i<4;i++)
            {
                string_dado.hora_name[i] = *dado;
                dado++;
            }
        break;
        
                
        case 6:
            for(i=0; i<4;i++)
            {
                string_dado.data_name[i] = *dado;
                dado++;
            }
        break;
        
        case 7:
            for(i=0; i<6;i++)
            {
                if(*dado != ' ' && *dado != '\0')
                {
                    vel[i] = *dado;
                }
               
                dado++;
            }

        break;
           
    }
    
    return;
}


void data_nome (unsigned char *p_buff)
{
    unsigned char i;
    for(i=0; i<4;i++)
    {
        filename[i] = *p_buff;
        p_buff++;
    }
}


/******************************************************************************
 * Funcao:		void monta_sd(unsigned char index, unsigned char *dado, float dado_localizacao)
 * Entrada:		unsigned char index, unsigned char *dado, float dado_localizacao
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do nome do arquivo
 *****************************************************************************/

void hora_nome (unsigned char *p_buff)
{
    unsigned char i;
    for(i=4; i<8;i++)
    {
        filename[i] = *p_buff;
        p_buff++;
    }
}