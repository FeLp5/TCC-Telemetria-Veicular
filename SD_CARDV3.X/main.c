/*
 * File:   main.c
 * Author: Etiq Lab Desktop
 *
 * Created on January 27, 2017, 4:44 PM
 */


#include <xc.h>
//#include "pic_18f4550.h"
#include <p18f4550.h>
//#include <pic18f4550.h>
//#include <spi.h>
//#include <usart.h>
//#include <sw_uart.h>
//#include "config.h"
#include "delay.h"
#include "pff.h"
#include "diskio.h"
#include "main.h"
#include "bibliotecas/hardware.h"
#include "bibliotecas/uart.h"
#include "bibliotecas/chaves.h"
#include "bibliotecas/SHRC.h"
#include "bibliotecas/display/display_lcd.h"
//Nossas bibliotecas=-============================
#include "bibliotecas/SPI.h"
#include "bibliotecas/SDCard.h"
//=-============================



//#define Baud 9600;
//#define spbrg ((FOSC/Baud)/64)-1                                 /*
//                                      ((FOSC/Desired Baud Rate)/64) ? 1
//                                       = ((16000000/9600)/64) ? 1        */
//                                       /*void Open1USART ( unsigned char config,  unsigned int spbrg);*/



//++++++++++++++++++++++++++um dos principais causadores de perda de memória+++++++++++++++++++


//BYTE sector_buffer[512];
unsigned char *rd;
unsigned char Result;
////static unsigned char str[512];
BYTE s;
DWORD fileSize;
BYTE txt[20];
BYTE data_buffer_32[32];



extern void readover(int);






// File to read================================================================= 
BYTE fileName[10] = {"check.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
BYTE folder[48] = {""}; 
//==============================================================================




/******************************************************************************
* Variaveis Globais
******************************************************************************/

unsigned char data_uart_recebe;

/*Store function pointers of Task.*/
void (*p_tarefas[NUMBER_OF_TASKS])(void);  

/*Store task´s times (time period to execute)*/
unsigned int tempo_backup[NUMBER_OF_TASKS];       
 
/*Stores recent task´s times ("time to execute" each task) */
unsigned int tempo_tarefa[NUMBER_OF_TASKS];  

/*Tells if TImer0 interrupt has been generated*/
unsigned char sinaliza_int_timer;                        
 
/* Tells if a task in executing (used to check task timeout validation)*/
volatile char tarefa_em_execucao;                  

unsigned int timeout_tarefa;




/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void mensagem_inicial(void);
void SDCard(void);



void acende_led4(void);








/*****************************************************************************/
/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Implementa a rotina de interrupcao
 *****************************************************************************/
void interrupt isr(void)
{
    unsigned char cont;
    
	/*Check UART interrupt reception */
	if (PIE1bits.RCIE && PIR1bits.RCIF)     
    {
        data_uart_recebe = recebe_dado_uart();
        PIR1bits.RCIF = 0;
    } //End if interrupt Recepcao UART
	
    
    unsigned char cont;
	//check if Timer0 interupt was triggered
	if (TMR0IF && T0IF)   
    {
        INTCONbits.TMR0IF = 0; 
        sinaliza_int_timer = YES;
        //Please use HEX.
        for(cont= 0; cont<NUMBER_OF_TASKS;cont++)
        {
            if(tempo_tarefa[cont]>0)    tempo_tarefa[cont]--;
        }

        
        if(tarefa_em_execucao == YES)
        {
            timeout_tarefa--;
            if(!timeout_tarefa)
            {
                //Timeout has reached (possibly a task has crashed)
                PCLATH = 0;
                PCL = 0;
            }
        }  
    
        //Please use HEX.
        write_timer_zero(0x0083);
    
    }

    

    
    
	if (INT0IF && INT0IE)   
    {
        INT0IF = 0;

    }
}



/******************************************************************************
 * Funcao:		void inicializa_tarefas(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Inicializa o ponteiro de função e as temporizações de cada umas
				das tarefas.
 *****************************************************************************/ 
void inicializa_tarefas(void)
{

	p_tarefas[0] = acende_led4;
    p_tarefas[1] = SDCard;
//    p_tarefas[2] = task_controla_led_vinte;
//    p_tarefas[3] = task_gerencia_canal_um;

	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_1000_MS;
    tempo_backup[1] = TIME_2000_MS;
//    tempo_backup[2] = TIME_50_MS;
//    tempo_backup[3] = TIME_100_MS;
	
	/*init recent temporization values of each task. 
	They´re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_1000_MS;
    tempo_tarefa[1] = TIME_2000_MS;
//    tempo_tarefa[2] = TIME_50_MS;
//    tempo_tarefa[3] = TIME_100_MS;

	//It indicates that there´s no task executing
    tarefa_em_execucao = NO;
}






/******************************************************************************
 * Funcao:		void escalonador(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Executa as tarefas do sistema.
 *****************************************************************************/ 
void escalonador()
{
    unsigned char cont;
    for(cont = 0; cont<NUMBER_OF_TASKS;cont++)
    {
        if((p_tarefas[cont]!= 0 ) && tempo_tarefa[cont] == 0)
        {
            tarefa_em_execucao = YES;
            timeout_tarefa = TASK_TIMEOUT;
            p_tarefas[cont]();
            tarefa_em_execucao = NO;
            tempo_tarefa[cont] = tempo_backup[cont];
        }
    }
    
}


/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Funcao principal
 *****************************************************************************/
void main(void) 
{
//variables for SDCard interfacing ====================
//    FRESULT FResult;
//    FATFS fs;
//    WORD br;
//==================================================
    
    
    init_hardware();
	inicializa_uart();
    init_lcd();
    inicializa_shrc();
//    inicializa_i2c();
	mensagem_inicial();
    inicializa_tarefas();
    inicializa_SPI(0);
	
    while(1)
    {
        //Verification: check if there´s a task to be executed
        if ((sinaliza_int_timer == YES)  && (NUMBER_OF_TASKS)) 
        {
            sinaliza_int_timer = NO;  
            escalonador();			
        }
//        leitura_chaves_sistema();   /*Driver*/;
        controle_shrc();            /*Driver*/
//        leitura_continua_adc();     /*Driver*/
        
    }
}


/******************************************************************************
 * Funcao:		void mensagem_inicial(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Funcao principal
 *****************************************************************************/
void mensagem_inicial(void)
{
    const unsigned char msg_linha_um[NUM_CARACTERES]      = "SDCard Init   ";
	const unsigned char msg_dois[NUM_CARACTERES]          = " 18/01/2022   ";
	
	posicao_cursor_lcd(1,0);
	escreve_frase_ram_lcd(msg_linha_um);
	
    posicao_cursor_lcd(2,0);
	escreve_frase_ram_lcd(msg_dois);
    
    __delay_ms(1000);
    LIMPA_DISPLAY();
}

//SDCARD==================================================================================================
void SDCard(void) 
{
    FRESULT FResult;
    FATFS fs;
    WORD br;
    
    
    
//    soft_hard_init(); // SOFTWARE AND HARWARE INITIALISE FUNCTION
//    OpenUART();       // SOFTWARE UART INITIALIZATION

    
    /*READ FUNCTION=============================================================*/
//    WriteSPI_(50);
//    chip_select = PIR1bits.SSPIF;
//    if(PIR1bits.SSPIF){
//        shrc_seta_bit(2);
//        controle_shrc();            /*Driver*/
//    }
//    else{
//        shrc_seta_bit(5);
//        controle_shrc();            /*Driver*/
//    }
//    __delay_ms(1000);
//    chip_select = 0;
   
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("Attempting to");
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("             ");
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("mount file sys");

    LIMPA_DISPLAY();
  
    proceed();
	if((FResult = pf_mount(&fs)) == FR_OK )
	{
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("syst inicialized");
		// open file=======================================
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
                
//					WriteUART(FResult + 0x30);????????????????????
				while( 1 );//lock the program
			}
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("lendo arquivo");
				
            
            //Starts to read the file for writing
            
            
            
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
                    
//					putsUART("\r\nError trying to read file; ");
//					WriteUART(FResult + 0x30);
//					putsUART(".\r\n");
					
					while( 1 );
				}
			} while( br == 31 );	// if the pf_Read reads less then 31 bytes the file has ended
		}
		else 
		{
//			putsUART("\r\nError trying to open file; ");
//			WriteUART(FResult + 0x30);
//			putsUART(".\r\n");
			
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


void acende_led4(void){
    static unsigned char flag = 0;
    flag = !flag;
    if(!flag)
    {
        shrc_seta_bit(BIT_0);
    }
    else
    {
        shrc_apaga_bit(BIT_0);
    }
    
}