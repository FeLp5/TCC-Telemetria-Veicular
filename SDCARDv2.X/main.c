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

//#include <pic18f4550.h>

//Bibliotecas do OPENLAB================
#include "bibliotecas/tff.h"
#include "bibliotecas/diskio.h"
//======================================

//Bibliotecas MCP2515 
#include "./bibliotecas/MCP2515.h"

//=====================================

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
fence_ext_struct poligono_ext[2];
bit_field flag[3];


  


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

unsigned int time_atualizacao;

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

//void stringcpy(char *str1, char *str2, int dir);

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
            
        }
        if(time_sd > 0) time_sd--;
        if(time_atualizacao > 0) time_atualizacao--;    
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
    p_tarefas[2] = verifica_fence_externo; // executada a cada 100ms
    p_tarefas[3] = troca_de_tela;
    
	/*init temporization values of each task. 
	These values do no change during execution*/
	tempo_backup[0] = TIME_5000_MS; 
    tempo_backup[1] = TIME_1000_MS;
    tempo_backup[2] = TIME_1000_MS;
	tempo_backup[3] = TIME_100_MS;
	/*init recent temporization values of each task. 
	Theyï¿½re used to decide which task must be executed*/
	tempo_tarefa[0] = TIME_5000_MS;
    tempo_tarefa[1] = TIME_1000_MS;
    tempo_tarefa[2] = TIME_1000_MS;
    tempo_tarefa[3] = TIME_1000_MS;
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
    unsigned char fence_diff_lat[4][11]  = {"-2342.07112", "-2342.07112", "-2342.11784", "-2342.11784"};
    unsigned char fence_diff_long[4][12] = {"-04629.73477", "-04629.68601", "-04629.68601", "-04629.73477"};
//    unsigned char fence_diff_lat[11]  = "-2365.84991";
//    unsigned char fence_diff_long[11] = "-4653.00475";
//    unsigned char fence_diff_lat[11]  = "-2365.99682";
//    unsigned char fence_diff_long[11] = "-4653.00475";
//    unsigned char fence_diff_lat[11]  = "-2365.99731";
//    unsigned char fence_diff_long[11] = "-4653.16622";      
    
    
    
    unsigned char *point_buff, i, j, count, count_index;
    unsigned char point, point_dif_lat ,point_dif_long;
    unsigned long int temp_var_deg;
    unsigned char temp_buff_lat[11] =  "-2342.07112";
    unsigned char temp_buff_long[12] = "-04629.68677";
    count = 0;
    point = 0;
    point_dif_lat = 0;
    point_dif_long = 0;
    j= 0;
//    point_buff = leitura_sdcard(0);
    
    point_buff_gps_lat = Latitude();
    point_buff_gps_long = Longitude();
    
    
    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("AQUI");
    for(i=0; i<11; i++)
    {
        temp_buff_lat[i]  = *point_buff_gps_lat;
        temp_buff_long[i] = *point_buff_gps_long;
        point_buff_gps_lat++;
        point_buff_gps_long++;
    }   
    
//GRAUS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
    while(point<4)
    {

        for(i=1; i<6; i++)
        {
            if(point == 0 || point == 2)
            {      
                if(point == 2 && !point_dif_long)
                {
                    point_dif_long++;
                }
                for(j= 0; i < 6; j++)
                {
                    poligono_ext[point_dif_long].point_long_deg[j] = temp_buff_long[i];  
                    poligono_ext[point_dif_long].fence_long_deg[j] = fence_diff_long[point][i]; 

                    i++;
                }
                
            } 
            
        }
        
        for(i=1; i<5; i++)
        {
            if(point == 0 || point == 2)
            {
                     
                if(point == 2 && !point_dif_lat)
                {
                    point_dif_lat++;
                }
                
                
                for(j= 0; i < 5; j++)
                {
                    poligono_ext[point_dif_lat].point_lat_deg[j] = temp_buff_lat[i];  
                    poligono_ext[point_dif_lat].fence_lat_deg[j] = fence_diff_lat[point][i]; 
                    i++;
                }
                
            } 
        }
        
        
        
        if(point == 0 || point == 2)
        {
            temp_var_deg = atoi(poligono_ext[count].point_lat_deg);
            poligono_ext[count].diff_lat_deg = atoi(poligono_ext[count].fence_lat_deg);
            poligono_ext[count].diff_lat_deg -= temp_var_deg;            
            temp_var_deg = atoi(poligono_ext[count].point_long_deg);
            poligono_ext[count].diff_long_deg =  atoi(poligono_ext[count].fence_long_deg);
            poligono_ext[count].diff_long_deg -= temp_var_deg; 
            
            count++;
        }
        point++;
    }
    
    
    
    
    
    point = 0;
    count = 0;
    point_dif_lat = 0;
    point_dif_long = 0;
    
    
//MINUTOS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    while(point<4)
    {
        for(i=6; i<11; i++)
        {
            if(point == 0 || point == 2)
            {
                if(point == 2 && !point_dif_lat)
                {
                    point_dif_lat++;
                }
                if(fence_diff_lat[point][i] != temp_buff_lat[i])
                {
                    for(j= 0; i < 11; j++)
                    {
                        poligono_ext[point_dif_lat].fence_lat_min[j] = fence_diff_lat[point][i];
                        poligono_ext[point_dif_lat].point_lat_min[j] = temp_buff_lat[i];
                        i++;
                    }

                }
                
            }
            else
            {
               i = 11; 
            }    
        } 
        
        for(i=7; i<12; i++)
        {
            if(point == 0 || point == 2)
            {
                if(point == 2 && !point_dif_long)
                {
                    point_dif_long++;
                }
                
                
                if(fence_diff_long[point][i] != temp_buff_long[i])
                {
                    for(j= 0; i < 12; j++)
                    {
                        poligono_ext[point_dif_long].fence_long_min[j] = fence_diff_long[point][i];
                        poligono_ext[point_dif_long].point_long_min[j] = temp_buff_long[i];
                        i++;
                    }
                }
            }
            else
            {
                i = 11;
            }
            
        }
        
        
        if(point == 0 || point == 2)
        {
            poligono_ext[count].diff_lat_min = (atoi(poligono_ext[count].fence_lat_min)) - (atoi(poligono_ext[count].point_lat_min));
            poligono_ext[count].diff_long_min = (atoi(poligono_ext[count].fence_long_min)) - (atoi(poligono_ext[count].point_long_min));
            count++;
        }
       
        point++;
    }
    count = 0;
    point = 0;

    
    
    
//VERIFICACAO +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
    switch(temp_buff_long[0])
    {
        case '-':
            if(
                ((poligono_ext[0].diff_long_min >= 0  &&
               poligono_ext[1].diff_long_min <= 0)  &&
                    
               (poligono_ext[0].diff_long_deg >= 0  &&
               poligono_ext[1].diff_long_deg <= 0)))
            {
                flag[0].point = 0;
            }
            else if((poligono_ext[0].diff_long_min == 0 ||
                    poligono_ext[1].diff_long_min == 0 ) &&
                    
                    
                    (poligono_ext[0].diff_long_deg == 0 ||
                    poligono_ext[1].diff_long_deg == 0))
            {
                flag[0].point = 0;
            }
            else
            {
                flag[0].point = 1;
            }
        break;
        
        default:
            if(
               (poligono_ext[0].diff_long_min <= 0  &&
               poligono_ext[1].diff_long_min >= 0 )  &&
              (poligono_ext[0].diff_lat_deg <= 0  &&  
               poligono_ext[1].diff_lat_deg >= 0))
            {
                flag[0].point = 0;
            }
            else if((poligono_ext[0].diff_long_min == 0 ||
                    poligono_ext[1].diff_long_min == 0) &&
                    (poligono_ext[0].diff_long_deg == 0 ||
                    poligono_ext[1].diff_long_deg == 0))
            {
                flag[0].point = 0;
            }
            else
            {
                flag[0].point = 1;
            }
        break;
                
    }
    
    switch(temp_buff_lat[0])
    {
        case '-':
            if(
              (
               poligono_ext[0].diff_lat_min <= 0  &&
               poligono_ext[1].diff_lat_min >= 0)  &&

               (poligono_ext[0].diff_lat_deg <= 0  &&  
               poligono_ext[1].diff_lat_deg >= 0)) 
                
            {
                flag[1].point = 0;
            }
            else if(
                    (poligono_ext[0].diff_lat_min == 0 ||
                    poligono_ext[1].diff_lat_min == 0) &&
                    (poligono_ext[0].diff_lat_deg == 0 ||
                    poligono_ext[1].diff_lat_deg == 0))
            {
                
                flag[1].point = 0;
            }
            else
            {
                flag[1].point = 1;
            }
        break;
        
        default:
            if((poligono_ext[0].diff_lat_min >= 0  &&
               poligono_ext[1].diff_lat_min <= 0 ) &&
                    
                    
               (poligono_ext[0].diff_lat_deg >= 0   &&
               poligono_ext[1].diff_lat_deg <= 0)     
             )
            {
                flag[1].point = 0;
            }
            else if((poligono_ext[0].diff_lat_min == 0 ||
                    poligono_ext[1].diff_lat_min == 0) &&
                    
                    (poligono_ext[0].diff_lat_deg == 0 ||
                    poligono_ext[1].diff_lat_deg == 0))
            {
                flag[1].point = 0;
            }
            else
            {
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("SWITCH3");
                flag[1].point = 1;
            }
        break;
                
    }

//VALIDACAO +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
    if(flag[0].point || flag[1].point)
    {
        flag[2].point = 1;
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("REGIAO NEGADA");
        posicao_cursor_lcd(2,14);
        escreve_inteiro_lcd(flag[0].point);
        posicao_cursor_lcd(2,15);
        escreve_inteiro_lcd(flag[1].point);
    }
    else
    {
        flag[2].point = 0;
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
//   MCP2515_RECEBE(0x32, )
//    if(!PORTEbits.RE1)
//    {
//        escrita_sdcard();
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
    if(!time_sd)
    {
//        if(fix)
//        {
            escrita_sdcard();
            time_sd = 30000;
//        }

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
    static unsigned char last_state = 0;;
   
    
    //primeira tela
    if(!PORTEbits.RE0 && !flag[2].point || (state == 1 ))
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
    
    if(!PORTEbits.RE1 && !flag[2].point || (state == 2))
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
    
    
    //FORA DO FENCE
    if(flag[2].point || state == 3)
    {
        //ideia para implementacao futura
        if(!time_atualizacao)
        {  
            time_atualizacao = 10000;
        }
        
        if(last_state != 3)
        {
            state = 3;
            LIMPA_DISPLAY();
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("D_Fence:");
        }
        last_state = 3;
    }

    
    
    
    
    //HOME
    
    if((!PORTEbits.RE1 && state!=0) && !flag[2].point )
    {
        if(last_state != 4)
        {
            LIMPA_DISPLAY();
            state = 0;
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("B1:RPM B2:FENCE");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("B2:HOME");
        }
        last_state = 4;
    }
    
    if(!state && !flag[2].point)
    {
        if(last_state != 4)
        {
            LIMPA_DISPLAY();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("B1:RPM  B2:FENCE");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("B2:HOME");
        }
        last_state = 4;
    }
   
    
}


//
//void stringcpy(char *str1, char *str2, int dir)
//{
//  uint8_t chr = 0;
////  dir=0;
//  do {
//    str2[chr + dir] = str1[chr];
//  } while(str1[chr++] != '\0');
//}