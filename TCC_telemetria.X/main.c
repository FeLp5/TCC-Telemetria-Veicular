/******************************************************************************
 * Nome do Arquivo 	: main.c
 *
 * Descricao       	: 
 *
 * Ambiente			    : MPLAB v3.51, XC8 v2.32, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Conceiï¿½ao de
                      Silva, Felipe Alves da
                      Souza, Ricardo de
			  
 * Contribuicao: Os codigos para comunicacao com o SDCard
 * foram retiradas do site OPENLAB (https://openlabpro.com/guide/raw-sd-readwrite-using-pic-18f4550/)
 * E os demais codigos para inicializacao do SDCard
 * Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014 retirado em
 * (https://github.com/etiq/OpenLab-PIC18F4550-SDCard-Examples)
 * 
 * O sistema FAT é opensource disponibilizado por (C) ChaN, 2008
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
//estrutura de flag para o fence
bit_field fence_flag[3];

unsigned char data_uart_recebe;

/*Store function pointers of Task.*/
void (*p_tarefas[NUMBER_OF_TASKS])(void);  

/*Store taskï¿½s times (time period to execute)*/
unsigned int tempo_backup[NUMBER_OF_TASKS];       
 
/*Stores recent taskï¿½s times ("time to execute" each task) */
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

volatile unsigned char *point_buff_gps_lat;

volatile unsigned char *point_buff_gps_long;

/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void mensagem_inicial(void);

void verifica_fence(void);

void verifica_dados_operacionais(void);

void disparo_gravacao(void);

void troca_de_tela(void);

void grava_sd(void);

void recebe_dado_gps(void);
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
 * Descricao:	Inicializa o ponteiro de funï¿½ï¿½o e as temporizaï¿½ï¿½es de cada umas
				das tarefas.
 *****************************************************************************/ 
void inicializa_tarefas(void)
{

    p_tarefas[0] = verifica_dados_operacionais; //executada a cada 5 segundos
    p_tarefas[1] = disparo_gravacao; // executada a cada 100ms
    p_tarefas[2] = troca_de_tela;
    
	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_500_MS; 
    tempo_backup[1] = TIME_1000_MS;
    tempo_backup[2] = TIME_5000_MS;
	/*init recent temporization values of each task. 
	Theyï¿½re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_500_MS;
    tempo_tarefa[1] = TIME_1000_MS;
    tempo_tarefa[2] = TIME_5000_MS;
	//It indicates that thereï¿½s no task executing
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

    T0CONbits.TMR0ON = 1;
    while(1)
    {
        //Verification: check if thereï¿½s a task to be executed
        if ((sinaliza_int_timer == YES)  && (NUMBER_OF_TASKS)) 
        {
            sinaliza_int_timer = NO;  
            escalonador();			
        }    
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
    posicao_cursor_lcd(1,0);
	escreve_frase_ram_lcd("AGUARDE...");
    
}


/******************************************************************************
 * Funcao:		void verifica_dados(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Verifica se o veiculo esta dentro do fence 
 *****************************************************************************/

void verifica_fence(void)
{
    
    recebe_dado_gps();
    longitude_to_convert();
    latitude_to_convert();
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
    unsigned char flag_gps = 0;
    unsigned char vel = 50;
    unsigned char i = 0;
    static unsigned char contador_atual =0;
    static unsigned char contador_anterior =0;
    static unsigned char contador_gravacoes = 0;
    static unsigned long int velocidade_media;
    static unsigned long int velocidade_atual_int = 0;
    
    static unsigned char dados_vel[3][6] = {"000000", "000000", "000000"};
    static unsigned char dados_lat[3][11] = {"0000000000", "0000000000", "0000000000"};
    static unsigned char dados_long[3][12] = {"000000000000", "000000000000", "000000000000"};
    static unsigned char dados_hora[3][6] = {"000000", "000000", "000000"};
    static unsigned char dados_data[3][6] = {"000000", "000000", "000000"};
    
    static unsigned char *velocidade_atual;
    velocidade_atual = Speed();
//    posicao_cursor_lcd(1,0);
//    escreve_inteiro_lcd(atoi(velocidade_atual)*1.852);
//    posicao_cursor_lcd(2,0);
//    escreve_inteiro_lcd(atoi(dados_vel[contador])*1.852);
    verifica_fence();

    
    if(contador_atual >=3)
    {
        time_sd = 0; 
        monta_sd(0, dados_hora[contador_gravacoes]);
        monta_sd(1, dados_data[contador_gravacoes]); 
        monta_sd(2, dados_lat[contador_gravacoes]);
        monta_sd(3, dados_long[contador_gravacoes]);
        monta_sd(7, dados_vel[contador_gravacoes]);

        contador_gravacoes++;
        if(contador_gravacoes >=3)
        {
           contador_gravacoes = 0;
           contador_atual = 0; 
           contador_anterior  = contador_atual;
        }
    }
    
    
    velocidade_atual_int = ((atoi(velocidade_atual)*1.852*100));
    if((velocidade_atual_int >  atoi(dados_vel[2])) &&      
        contador_atual<3)
    {
//        ltoa(velocidade_atual,velocidade_atual_int/1.852 , 10);
//        posicao_cursor_lcd(2,0);
//        escreve_frase_ram_lcd(velocidade_atual);
        if(velocidade_atual_int > atoi(dados_vel[contador_anterior]))
        {
            strcpy(dados_vel[contador_atual], velocidade_atual);
            strcpy(dados_lat[contador_atual], latitude());
            strcpy(dados_long[contador_atual], longitude());
            strcpy(dados_hora[contador_atual], rawhora());
            strcpy(dados_data[contador_atual], rawdata());
        }

        
        
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd(dados_lat[contador_atual]);
//        
        contador_anterior  = contador_atual;
        contador_atual++; 
    }


//    if(!PORTEbits.RE2)
//    {
//        posicao_cursor_lcd(2,0);
//        escreve_inteiro_lcd(velocidade_media);
//        velocidade_media++;
//    }
//    else
//    {
//        velocidade_media = 0;
//    }
    
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
    
    if(!time_sd)
    {
        fix_gps = fix();
        if(fix_gps[0] == '0')
        {     
//            verifica_fence();
            recebe_dado_gps();
            dados_gps_to_sd();
            grava_sd();
        }
        time_sd = 1000;
    } 
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
    static unsigned char last_state = 0;
    
    if(!PORTEbits.RE0 || (state == 1 ))
    {

        if(!time_atualizacao)
        {
            if(!fence_flag[2].point)
            {
                posicao_cursor_lcd(1,8);
                escreve_frase_ram_lcd("DENTRO");
            }
            else
            {
                posicao_cursor_lcd(1,8);
                escreve_frase_ram_lcd("FORA");
                posicao_cursor_lcd(1,13);
                escreve_inteiro_lcd(fence_flag[0].point);
                posicao_cursor_lcd(1,14);
                escreve_frase_ram_lcd("|");
                posicao_cursor_lcd(1,15);
                escreve_inteiro_lcd(fence_flag[1].point);
            }
            
            posicao_cursor_lcd(2,8);
            escreve_frase_ram_lcd(fix()); 
            time_atualizacao = 10000;
        }

        if(last_state != 1)
        {
            state = 1;
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("FENCE:");

            if(!fence_flag[2].point)
            {
                posicao_cursor_lcd(1,8);
                escreve_frase_ram_lcd("DENTRO");
            }
            else
            {
                posicao_cursor_lcd(1,8);
                escreve_frase_ram_lcd("FORA");
                posicao_cursor_lcd(1,13);
                escreve_inteiro_lcd(fence_flag[0].point);
                posicao_cursor_lcd(1,14);
                escreve_frase_ram_lcd("|");
                posicao_cursor_lcd(1,15);
                escreve_inteiro_lcd(fence_flag[1].point);
            }

            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("FIX:"); 
            posicao_cursor_lcd(2,8);
            escreve_frase_ram_lcd(fix()); 
        }
        last_state = 1;

    } 

    
    
    
    
    
    
        //segunda tela

    if(!PORTEbits.RE1 || (state == 2))
    {
        if(!time_atualizacao)
        {
            recebe_dado_gps();
            mostra_dados_display();
            time_atualizacao = 2000;
        }


        if(last_state != 2)
        {
            state = 2;
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("lt:");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("lo:");
            recebe_dado_gps();
            mostra_dados_display();
        }
        last_state = 2;
    }
    
    
    
    //HOME
    if(!state)
    {
        if(last_state != 4)
        {
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("B1:FENCE");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("B2:LTLO");

        }
        last_state = 4;
    }
}


/******************************************************************************
 * Funcao:		void disparo_gravacao(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Grava os dados no SD Card
 *****************************************************************************/

void grava_sd(void)
{
    PORTBbits.RB3 = 0; // DESATIVA A PORTA AND
    desliga_uart();
    inicializa_SPI();
    escrita_sdcard();
    desliga_SPI();
    inicializa_uart();
    PORTBbits.RB3 = 1;
}


/******************************************************************************
 * Funcao:		void recebe_dado_gps(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Garante a recepção dos dados do GPS evitando a flutuacao dos dados
 *****************************************************************************/

void recebe_dado_gps(void)
{
    unsigned char flag_recebe = 0;
    unsigned char timeout = 10;
    while(!flag_recebe && timeout>0)
    {
        flag_recebe = verifica_recep_gps();
        timeout--;
    }
}