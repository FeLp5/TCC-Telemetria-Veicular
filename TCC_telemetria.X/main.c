/******************************************************************************
 * Nome do Arquivo 	: main.c
 *
 * Descricao       	: 
 *
 * Ambiente			    : MPLAB v3.51, XC8 v2.32, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Concei�ao de
                      Silva, Felipe Alves da
                      Souza, Ricardo de
			  
 * Contribuicao: Os codigos para comunicacao com o SDCard
 * foram retiradas do site OPENLAB (https://openlabpro.com/guide/raw-sd-readwrite-using-pic-18f4550/)
 * E os demais codigos para inicializacao do SDCard
 * Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014 retirado em
 * (https://github.com/etiq/OpenLab-PIC18F4550-SDCard-Examples)
 * 
 * O sistema FAT � opensource disponibilizado por (C) ChaN, 2008
 * Versao: FatFs - Tiny FAT file system module  R0.06                 (C)ChaN, 2008
 * 
 * 
 * 
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
 *
 *****************************************************************************/

#include <xc.h>
//Bibliotecas do OPENLAB================
#include "bibliotecas/tff.h"
#include "bibliotecas/diskio.h"
//======================================
#include "main.h"
#include "bibliotecas/hardware.h"
#include "bibliotecas/uart.h"
#include "bibliotecas/display_lcd.h"
#include "bibliotecas/SPI.h"
#include "bibliotecas/SDCard.h"
#include "bibliotecas/GPS.h"
#include "bibliotecas/fence.h"
#include "stdlib.h"
#include "string.h"
//Biblioteca MCP2515=========
#include "bibliotecas/MCP2515.h"
//===========================

/******************************************************************************
* Variaveis Globais
******************************************************************************/
//estrutura de dados para o fence
//fence_ext_struct poligono_ext[2];
bit_field fence_flag[3];

  


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

unsigned int time_sd;
unsigned int time_recovery;

unsigned int time_atualizacao;

unsigned char flag_anomalia;

unsigned char *point_buff_gps_lat;

unsigned char *point_buff_gps_long;

/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void mensagem_inicial(void);

void verifica_fence_externo(void);

void verifica_dados_operacionais(void);

void disparo_gravacao(void);

void troca_de_tela(void);

void recebe_dados_gps(void);
//void stringcpy(char *str1, char *str2, int dir);

/*****************************************************************************/
/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao	Implementa a rotina de interrupcao
 *****************************************************************************/
void __interrupt() isr(void)
{
    unsigned char cont;
    
	/*Check UART interrupt reception */
	if (PIE1bits.RCIE && PIR1bits.RCIF)     
    {
        PIR1bits.RCIF = 0;
        data_uart_recebe = recebe_dado_uart();
        GPSRead(data_uart_recebe);
//        gps_flag = 1;
        PORTBbits.RB2 =	!PORTBbits.RB2;	
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
        if(time_sd > 0) time_sd--;
        if(time_atualizacao > 0) time_atualizacao--; 
//        if(time_recovery>0) time_recovery--;
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
//        write_timer_zero(0x0083);
          write_timer_zero(0x45);
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

    p_tarefas[0] = verifica_dados_operacionais; //executada a cada 5 segundos
    p_tarefas[1] = disparo_gravacao; // executada a cada 100ms
//    p_tarefas[2] = verifica_fence_externo; // executada a cada 100ms
    p_tarefas[2] = troca_de_tela;
    
	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_100_MS; 
    tempo_backup[1] = TIME_100_MS;
    tempo_backup[2] = TIME_100_MS;
	tempo_backup[3] = TIME_1000_MS;
	/*init recent temporization values of each task. 
	They�re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_100_MS;
    tempo_tarefa[1] = TIME_100_MS;
    tempo_tarefa[2] = TIME_100_MS;
    tempo_tarefa[3] = TIME_1000_MS;
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
    T0CONbits.TMR0ON = 0;
    init_lcd();
	mensagem_inicial();
    inicializa_tarefas();
    inicializa_uart();
//    inicializa_SPI();
//    MCP2515_INIT();

    T0CONbits.TMR0ON = 1;
    while(1)
    {
        //Verification: check if there�s a task to be executed
        if ((sinaliza_int_timer == YES)  && (NUMBER_OF_TASKS)) 
        {
            sinaliza_int_timer = NO;  
            escalonador();			
        }
        dados_gps_to_sd();
//        escrita_sdcard();
       
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
    const unsigned char msg_linha_um[NUM_CARACTERES]      = "FATEC ST ANDRE";
	const unsigned char msg_dois[NUM_CARACTERES]          = "TCC--TELEMTRIA";
	
	posicao_cursor_lcd(1,0);
	escreve_frase_ram_lcd(msg_linha_um);
	
    posicao_cursor_lcd(2,0);
	escreve_frase_ram_lcd(msg_dois);
    __delay_ms(1000);
    LIMPA_DISPLAY(); 
  
}


/******************************************************************************
 * Funcao:		void verifica_dados(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Verifica os dados do fence atual
 *****************************************************************************/

void verifica_fence_externo(void)
{
    armazena_latitude();
    armazena_longitude();
    
    verifica_diferenca_graus(0);
    calcula_diff_graus(0);
    
    verifica_diferenca_graus(1);
    calcula_diff_graus(1);
    
    
    verifica_diferenca_minutos(0);
    calcula_diff_minutos(0);
    
    verifica_diferenca_minutos(1);
    calcula_diff_minutos(1);
    
    
    fence_flag[0].point = verifica_plausibilidade_long();
    fence_flag[1].point = verifica_plausibilidade_lat();
    
    if(fence_flag[0].point || fence_flag[1].point)
    {
        fence_flag[2].point = 1;
        monta_sd(4, "FORA");
    }
    else
    {
        fence_flag[2].point = 0;
        monta_sd(4, "DENTRO");
    }
 

  
}


/******************************************************************************
 * Funcao:		void verifica_dados_operacionais(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Verifica os dados operacionais do veiculo vindos da rede CAN
 *****************************************************************************/

void verifica_dados_operacionais(void)
{
    unsigned char rpm[4];
    
   
    
    if(!time_sd)
    {

        flag_anomalia = 0;
        
//        MCP2515_RECEBE(0x30, 4, rpm);
    }   
     
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("enviando msg");
//        LIMPA_DISPLAY(); 
//        time_sd = 1000;

    
}



/******************************************************************************
 * Funcao:		void disparo_gravacao(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Dispara uma gravacao no SDCard a cada 30 segundos
 *****************************************************************************/
void disparo_gravacao(void)
{
    unsigned char *fix_gps;
    
//    recebe_dados_gps();
    fix_gps = fix();
    if(!time_sd)
    {
        recebe_dados_gps();
        if(fix_gps[0] == '1')
        {     
            verifica_fence_externo();
            escrita_sdcard();
        }
        else
        {
            verifica_fence_externo();
        }
        

        time_sd = 1000;
//      time_recovery = 5000;
            
//            flag_anomalia = 0;
//            MCP2515_INIT(); 
    } 

//    recebe_dados_gps();

}


/******************************************************************************
 * Funcao:		void disparo_gravacao(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Troca a tela para ajudar o usuario a entender se o sistema esta em 
 * funcionamento ou se o usuario ultrapassou os limites de sua rota
 *****************************************************************************/
void troca_de_tela(void)
{
    
    static unsigned char state = 0;
    static unsigned char last_state = 0;;
   
    
    switch(fence_flag[2].point)
    {
        case 0:
            
            if(!PORTEbits.RE0 && !fence_flag[2].point || (state == 1 ))
            {

                if(!time_atualizacao)
                {
                    posicao_cursor_lcd(2,6);
                    escreve_frase_ram_lcd(fix()); 
                    time_atualizacao = 10000;
                }

                if(last_state != 1)
                {
                    state = 1;
                    LIMPA_DISPLAY();
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("RPM:");
                    posicao_cursor_lcd(2,0);
                    escreve_frase_ram_lcd("VEL:"); 
                    posicao_cursor_lcd(2,6);
                    escreve_frase_ram_lcd(fix()); 
                }
                last_state = 1;

            } 
            //segunda tela

            if(!PORTEbits.RE1 && !fence_flag[2].point || (state == 2))
            {
                if(!time_atualizacao)
                {
                    point_buff_gps_lat = Latitude();
                    point_buff_gps_long = Longitude();
                    posicao_cursor_lcd(1,4);
                    escreve_frase_ram_lcd(point_buff_gps_lat);
                    posicao_cursor_lcd(2,4);
                    escreve_frase_ram_lcd(point_buff_gps_long);
                    time_atualizacao = 2000;
                }


                if(last_state != 2)
                {
                    state = 2;
                    LIMPA_DISPLAY();
                    point_buff_gps_lat = Latitude();
                    point_buff_gps_long = Longitude();
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("lt:");
                    posicao_cursor_lcd(1,4);
                    escreve_frase_ram_lcd(point_buff_gps_lat);
                    posicao_cursor_lcd(2,0);
                    escreve_frase_ram_lcd("lo:");
                    posicao_cursor_lcd(2,4);
                    escreve_frase_ram_lcd(point_buff_gps_long);
                }
                last_state = 2;
            }
   
            //HOME
//            if((!PORTEbits.RE1 && state!=0) && !fence_flag[2].point )
//            {
//                if(last_state != 4)
//                {
//                    LIMPA_DISPLAY();
//                    state = 0;
//                    posicao_cursor_lcd(1,0);
//                    escreve_frase_ram_lcd("B1:RPM B2:FENCE");
//                }
//                last_state = 4;
//            }

            if(!state && !fence_flag[2].point)
            {
                if(last_state != 4)
                {
                    LIMPA_DISPLAY();
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("B1:RPM  B2:FENCE");

                }
                last_state = 4;
            }
        break;
        
        default:
           if(fence_flag[2].point || state == 3)
            {
                //ideia para implementacao futura
                if(!time_atualizacao)
                {  
                    posicao_cursor_lcd(2,7);
                    escreve_inteiro_lcd(fence_flag[0].point);                        
                    posicao_cursor_lcd(2,9);
                    escreve_inteiro_lcd(fence_flag[1].point);
                    time_atualizacao = 10000;
                }

                if(last_state != 3)
                {
                    state = 3;
                    LIMPA_DISPLAY();
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("REGIAO NEGADA");
                    posicao_cursor_lcd(2,0);
                    escreve_frase_ram_lcd("LO-LT:");
                    posicao_cursor_lcd(2,7);
                    escreve_inteiro_lcd(fence_flag[0].point);
                    posicao_cursor_lcd(2,8);
                    escreve_frase_ram_lcd("-");
                    posicao_cursor_lcd(2,9);
                    escreve_inteiro_lcd(fence_flag[1].point);
                }
                last_state = 3;
            }
        break;
    }
    

}


void recebe_dados_gps(void)
{
    unsigned char flag_gps = 0;
    PORTBbits.RB3 = 1;
    desliga_SPI();
    inicializa_uart();
    
    
    while(!flag_gps)
    {
        flag_gps = verifica_recep_gps();
    }

    
    desliga_uart();
    inicializa_SPI();
    PORTBbits.RB3 = 0;
}