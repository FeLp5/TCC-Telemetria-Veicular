/******************************************************************************
 * Nome do Arquivo 	: main.c
 *
 * Descricao       	: 
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Concei�ao de
                      Silva, Felipe Alves da
                      Souza, Ricardo de
			  
 * Contribui��o: Os codigos para comunica��o com o SDCard (SDCard.h)
 * foram retiradas do site OPENLAB (https://openlabpro.com/guide/raw-sd-readwrite-using-pic-18f4550/)
 * E os demais c�digos para inicializa��o do SDCard e todo o protocolo do sistema FAT 
 * Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014 retirado em
 * (https://github.com/etiq/OpenLab-PIC18F4550-SDCard-Examples)
 * 
 * 
 * 
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
 *
 *****************************************************************************/

#include <xc.h>
#include <p18f4550.h>

//Bibliotecas do OPENLAB================
#include "bibliotecas/config.h"
#include "bibliotecas/ff.h"
#include "bibliotecas/diskio.h"
//======================================
#include "main.h"
#include "bibliotecas/hardware.h"
#include "bibliotecas/uart.h"
#include "bibliotecas/chaves.h"
#include "bibliotecas/SHRC.h"
#include "bibliotecas/display_lcd.h"
//#include "bibliotecas/adc.h"
//#include "bibliotecas/SPI.h"
#include "bibliotecas/SDCard.h"
#include "bibliotecas/GPS.h"
//=-============================



/******************************************************************************
* Variaveis Globais
******************************************************************************/
//++++++++++++++++++++++++++um dos principais causadores de perda de mem�ria+++++++++++++++++++
////static unsigned char str[512];
//BYTE sector_buffer[512];
//============================

unsigned char data_uart_recebe;

/*Store function pointers of Task.*/
void (*p_tarefas[NUMBER_OF_TASKS])(void);  

/*Store task�s times (time period to execute)*/
unsigned int tempo_backup[NUMBER_OF_TASKS];       
 
/*Stores recent task�s times ("time to execute" each task) */
unsigned int tempo_tarefa[NUMBER_OF_TASKS];  

/*Tells if TImer0 interrupt has been generated*/
unsigned char sinaliza_int_timer;                        
 
/* Tells if a task in executing (used to check task timeout validation)*/
volatile char tarefa_em_execucao;                  

//variable for check timeout
unsigned int timeout_tarefa;




/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void mensagem_inicial(void);








/*****************************************************************************/
/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa a rotina de interrupcao
 *****************************************************************************/
void interrupt isr(void)
{
    unsigned char cont;
    
	/*Check UART interrupt reception */
	if (PIE1bits.RCIE && PIR1bits.RCIF)     
    {
        data_uart_recebe = recebe_dado_uart();
        PIR1bits.RCIF = 0;
        tratamento_uart(data_uart_recebe);
//        LATBbits.LATB4 = ~LATBbits.LATB4;
        
        		
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
 * Descricao:	Inicializa o ponteiro de fun��o e as temporiza��es de cada umas
				das tarefas.
 *****************************************************************************/ 
void inicializa_tarefas(void)
{

    p_tarefas[0] = SDCard;
//    p_tarefas[1] = SDCard;

	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_5000_MS;
//    tempo_backup[1] = TIME_5000_MS;
//    tempo_backup[1] = TIME_2000_MS;
	
	/*init recent temporization values of each task. 
	They�re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_5000_MS;
//    tempo_tarefa[1] = TIME_5000_MS;
//    tempo_tarefa[1] = TIME_2000_MS;

	//It indicates that there�s no task executing
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
    init_hardware();
//	inicializa_uart();
    init_lcd();
    inicializa_shrc();
//    inicializa_i2c();
	mensagem_inicial();
    inicializa_tarefas();
//    inicializa_SPI(0,3,1);
    ADCON1 = 0X0F;
    while(1)
    {
        //Verification: check if there�s a task to be executed
        if ((sinaliza_int_timer == YES)  && (NUMBER_OF_TASKS)) 
        {
            sinaliza_int_timer = NO;  
            escalonador();			
        }
        leitura_chaves_sistema();   /*Driver*/
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
    
    
//    posicao_cursor_lcd(1,0);
//	escreve_frase_ram_lcd("LAT:");
//    
//    posicao_cursor_lcd(2,0);
//	escreve_frase_ram_lcd("LONG:");
}

