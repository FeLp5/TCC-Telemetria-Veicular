/******************************************************************************
 * Nome do Arquivo 	: main.c
 *
 * Descricao       	: 
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
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
#include <p18f4550.h>

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
#include "stdlib.h"
#include "string.h"
//=-============================



/******************************************************************************
* Variaveis Globais
******************************************************************************/
//estrutura de dados para o fence
fence_ext_struct poligono_ext[4];
bit_field flag[4];


  


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



/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void mensagem_inicial(void);

void verifica_fence_externo(void);

void verifica_dados_operacionais(void);

void disparo_gravacao(void);



/*****************************************************************************/
/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao	Implementa a rotina de interrupcao
 *****************************************************************************/
void interrupt isr(void)
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
            if(time_sd > 0) time_sd--;
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
 * Descricao:	Inicializa o ponteiro de funï¿½ï¿½o e as temporizaï¿½ï¿½es de cada umas
				das tarefas.
 *****************************************************************************/ 
void inicializa_tarefas(void)
{

    p_tarefas[0] = verifica_dados_operacionais; //executada a cada 5 segundos
    p_tarefas[1] = disparo_gravacao; // executada a cada 100ms
    p_tarefas[2] = verifica_fence_externo; // executada a cada 100ms
    
    
	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_5000_MS; 
    tempo_backup[1] = TIME_1000_MS;
    tempo_backup[2] = TIME_10000_MS;
	
	/*init recent temporization values of each task. 
	Theyï¿½re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_5000_MS;
    tempo_tarefa[1] = TIME_1000_MS;
    tempo_tarefa[2] = TIME_10000_MS;

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
        dados_gps_to_sd();
       
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
    poligono_ext[0].fence_diff_lat[11]  = "-2365.84991";
    poligono_ext[0].fence_diff_long[11] = "-4653.16622";
    poligono_ext[1].fence_diff_lat[11]  = "-2365.84991";
    poligono_ext[1].fence_diff_long[11] = "-4653.00475";
    poligono_ext[2].fence_diff_lat[11]  = "-2365.99682";
    poligono_ext[2].fence_diff_long[11] = "-4653.00475";
    poligono_ext[3].fence_diff_lat[11]  = "-2365.99731";
    poligono_ext[3].fence_diff_long[11] = "-4653.16622";      
    
    
    unsigned char *point_buff, i, j, count, count_index;
    unsigned char *point_buff_gps_lat, *point_buff_gps_long;
    unsigned char point;
    
    unsigned char temp_buff_lat[11] = "-4653.16622";
    unsigned char temp_buff_long[11] = "-4653.16622";
    count = 0;
    point = 0;
    j= 0;
//    point_buff = leitura_sdcard(0);
    
    point_buff_gps_lat = Latitude();
    point_buff_gps_long = Longitude();
    
    for(i=0; i<11; i++)
    {
        temp_buff_lat[i]  = *point_buff_gps_lat;
        temp_buff_long[i] = *point_buff_gps_long;
        point_buff_gps_lat++;
        point_buff_gps_long++;
    }
    
    
    while(j<4)
    {
        for(i=0; i<5; i++)
        {
            flag[j].point = 1;
        }
        j++;
    }

    point = 0;
    
    while(point<4)
    {
        for(i=6; i<11; i++)
        {
            if(poligono_ext[point].fence_diff_lat[i] != temp_buff_lat[i])
            {
                for(j= 0; i < 11; j++)
                {
                    poligono_ext[point].diff_lat = (atoi(poligono_ext[point].fence_diff_lat)) - (atoi(poligono_ext[point].latitude));
                }
                
            }
            
            if(poligono_ext[point].fence_diff_long[i] != temp_buff_long[i])
            {
                for(j= 0; i < 11; j++)
                {
                    poligono_ext[point].diff_long = (atoi(poligono_ext[point].fence_diff_long)) - (atoi(poligono_ext[point].longitude));
                }
                
            }
        } 
    }

    
    switch(poligono_ext[point].longitude[0])
    {
        case '-':
            if(poligono_ext[0].diff_long > 0  &&
        //       poligono_ext[1].diff_long > 0  &&    
               poligono_ext[2].diff_long < 0  //&&
        //       poligono_ext[3].diff_long < 0   &&     
             )
            {
                flag[0].point = 1;
            }
            else
            {
                flag[0].point = 0;
            }
        break;
        
        default:
            if(poligono_ext[0].diff_long < 0  &&
        //       poligono_ext[1].diff_long > 0  &&    
               poligono_ext[2].diff_long > 0  //&&
        //       poligono_ext[3].diff_long < 0   &&     
             )
            {
                flag[0].point = 1;
            }
            else
            {
                flag[0].point = 0;
            }
        break;
                
    }
    
    
    switch(poligono_ext[point].latitude[0])
    {
        case '-':
            if(poligono_ext[0].diff_lat < 0  &&
               poligono_ext[1].diff_lat < 0  &&    
               poligono_ext[2].diff_lat > 0  &&
               poligono_ext[3].diff_lat > 0      
             )
            {
                flag[1].point = 1;
            }
            else
            {
                flag[1].point = 0;
            }
        break;
        
        default:
            if(poligono_ext[0].diff_lat > 0  &&
               poligono_ext[1].diff_lat > 0  &&    
               poligono_ext[2].diff_lat < 0  &&
               poligono_ext[3].diff_lat < 0       
             )
            {
                flag[1].point = 1;
            }
            else
            {
                flag[1].point = 0;
            }
        break;
                
    }

    
    if(flag[0].point && flag[1].point)
    {
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("REGIAO NEGADA");
    }
 
    
    
//    for(i=0; i<11; i++)
//    {
//        temp_buff_lat[i] = *point_buff_gps_lat;
//        temp_buff_long[i] = *point_buff_gps_long;
//        point_buff_gps_lat++;  
//        point_buff_gps_long++;
//    }
    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd(point_buff);
//    __delay_ms(1000);
//
//    for(i=0; caractere_gps[i] != ';'; i++)
//    {
//        posicao_cursor_lcd(1,0);
//        escreve_caractere_lcd(caractere_gps[i]);
//        if(caractere_gps[i-1] == '|' || flag_long)
//        {
//            flag_long = 1;
//            flag_lat = 0;
//            if(caractere_gps[i] != temp_buff_long[count])
//            {
//                for(count_index = 0; caractere_gps[i] != '$'; count_index++)
//                {
//                    poligono_ext[point].fence_diff_long[count_index] = caractere_gps[i];
//                    poligono_ext[point].point_diff_long[count_index] = temp_buff_long[count];
//                    i++;
//                    count++;
//                    
//                }
//                count = 0;
//            }
//        }
//        
//        
//        //11 22 33 44
//        
//        if(caractere_gps[i-1] == '$' || flag_lat)
//        {
//            
//            flag_long = 0;
//            flag_lat = 1;
//            if(caractere_gps[i] != temp_buff_lat[count])
//            {
//                for(count_index = 0; caractere_gps[i] != '|'; count_index++)
//                {
//                    poligono_ext[point].fence_diff_lat[count_index] = caractere_gps[i];
//                    poligono_ext[point].point_diff_lat[count_index] = temp_buff_lat[count];
//                    posicao_cursor_lcd(1,count_index);
//                    escreve_caractere_lcd(poligono_ext[point].fence_diff_lat[count_index]);
////                    __delay_ms(100);
//                    i++;
//                    count++;
//                }
//            
//            }
//            count = 0;
//        }
//        count++;
//        if(count == 11)
//        {
//            count = 0;
//        }
//            
//
//            if(i==(j+22))
//            {
//                j += 22;
//                point += 1;
//            }
//
//
//            if(i==size)
//            {
//                i = (j+22);
//            }     
    
    
//    LIMPA_DISPLAY();
//    posicao_cursor_lcd(1,5);
//    escreve_frase_ram_lcd(poligono_ext[1].point_diff_long);
//    posicao_cursor_lcd(2,5);
//    escreve_frase_ram_lcd(poligono_ext[1].fence_diff_long);
//    
//    escrita_sdcard(poligono_ext[0].point_diff_long, poligono_ext[0].fence_diff_long);
    
}

void verifica_dados_operacionais(void)
{
   
//    if(!PORTEbits.RE1)
//    {
//        escrita_sdcard();
//    }
    
    
    
}




void disparo_gravacao(void)
{
    
    if(!time_sd)
    {
        escrita_sdcard();
        time_sd = 30000;
    } 
}