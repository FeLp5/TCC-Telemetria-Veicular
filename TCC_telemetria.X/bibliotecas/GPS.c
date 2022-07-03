///******************************************************************************
// * Nome do Arquivo 	: GPS.c
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB v3.51, XC8 versao 2.30, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceicao de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
//// GPS Library for CCS PIC C compiler
//// http://simple-circuit.com/
////
//// GPS Information extraction using PIC18F4550 
//    http://www.electronicwings.com
//
// *****************************************************************************/

#define _GPRMC_  1
#define _GPGGA_  2
#define _OTHER_  3

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "GPS.h"
#include "SDCard.h"
#include "display_lcd.h"
#include "fence.h"

/******************************************************************************
* Variaveis Globais
******************************************************************************/
bit_field_gps gps_flag[6];
int GPRMC_ok = 0, GPGGA_ok = 0;
uint8_t char_number = 0, SentenceType = 0, Term;
char sentence[6];
char rawTime[7]; 
char rawDate[6];
char rawSpeed[20];
char rawCourse[6];
char rawSatellites[3];
char rawAltitude[7];
char buffer[12];
char rawFix[2];
static char rawLatitude[12];
static char rawLongitude[13];

char *teste;
char gga_buffer[GGA_BUFFER_SIZE];              /* to store GGA string */
char rmc_buffer[RMC_BUFFER_SIZE];

char gga_code[3];
char rmc_code[3];

unsigned char n_s, E_W;                        /* for getting direction polarity */

unsigned char gga_pointers[GGA_POINTERS_SIZE]; /* to store instances of ',' */
unsigned char rmc_pointers[RMC_POINTERS_SIZE];

char comma_counter_gga;
char comma_counter_rmc;

volatile unsigned int gga_index;
volatile unsigned int rmc_index;

volatile unsigned char is_it_gga_string	= 0;
volatile unsigned char is_it_rmc_string = 0;

/******************************************************************************
 * Funcao:		void gps(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Chama as funcoes de armazenamento dos dados
 *****************************************************************************/
void gps(void)
{
    unsigned char *flag_fix;
    get_fix();
    flag_fix = fix();
    if(flag_fix[0] == '1')
    {
        get_gpstime();
        get_latitude();
        get_longitude();
        get_rawdate();
        get_rawspeed(); 
    }
}

///******************************************************************************
// * Funcao:		void get_gpstime(void)
// * Entrada:		Nenhuma (void)
// * Saida:		    Nenhuma (void)
// * Descricao:	Coleta a hora atual
// *****************************************************************************/
void get_gpstime(void)
{
	unsigned char index;
	unsigned char time_buffer[15];


	for(index = 0; index<6; index++)
	{		
		rawTime[index] = gga_buffer[index];
	}
        
}
///******************************************************************************
// * Funcao:		void get_latitude(void)
// * Entrada:		Nenhuma (void)
// * Saida:         Nenhuma (void)
// * Descricao:	Coleta a a latitude
// *****************************************************************************/

void get_latitude(void)
{
	unsigned char index_lat, index;
	unsigned char time_buffer[15];
	index_lat = 1;
	for(index = 10; index<20; index++)
	{		
		rawLatitude[index_lat] = gga_buffer[index];
        index_lat++;
	}
    if(gga_buffer[21] == 'N')
    {
        rawLatitude[0] = '0';
    }
    else
    {
        rawLatitude[0] = '-';
    }
        
}
///******************************************************************************
// * Funcao:		void get_longitude(void)
// * Entrada:		Nenhuma (void)
// * Saida:		    Nenhuma (void)
// * Descricao:	Coleta a longitude
// *****************************************************************************/

void get_longitude(void)
{
	unsigned char index_long, index;
	index_long = 1;
	for(index = 23; index<34; index++)
	{		
		rawLongitude[index_long] = gga_buffer[index];
        index_long++;
	}
    if(gga_buffer[32] == 'E')
    {
        rawLongitude[0] = '0';
    }
    else
    {
        rawLongitude[0] = '-';
    }
        
}
///******************************************************************************
// * Funcao:		void get_rawdate(void)
// * Entrada:		Nenhuma (void)
// * Saida:		    Nenhuma (void)
// * Descricao:	Coleta a data atual
// *****************************************************************************/
void get_rawdate(void)
{
	unsigned char index_date, index;
	index_date = 0;
	for(index = 46; index<52; index++)
	{		
		rawDate[index_date] = rmc_buffer[index];
        index_date++;
	}
}


///******************************************************************************
// * Funcao:		void get_rawspeed(void)
// * Entrada:		Nenhuma (void)
// * Saida:		    Nenhuma (void)
// * Descricao:	Coleta a velocidade 
// *****************************************************************************/
void get_rawspeed(void)
{
	unsigned char index_speed, index;
	index_speed = 0;
	for(index = 39; index<44; index++)
	{		
		rawSpeed[index_speed] = rmc_buffer[index];
        index_speed++;
	}
}

///******************************************************************************
// * Funcao:		void get_fix(void)
// * Entrada:		Nenhuma (void)
// * Saida:		    Nenhuma (void)
// * Descricao:	Retorna o fix
// *****************************************************************************/
void get_fix(void)
{
	unsigned char index_fix, index;
	index_fix = 0;
	for(index = 37; index<39; index++)
	{		
		rawFix[index_fix] = gga_buffer[index];
        index_fix++;
	}
}


///******************************************************************************
// * Funcao:		void tratamento_uart(unsigned char received_char)  
// * Entrada:		unsigned char received_char
// * Saida:         Nenhuma (void)
// * Descricao:	Armazena as informacoes do buffer em variaveis
// *****************************************************************************/
void tratamento_uart(unsigned char received_char)   
{
		if(received_char =='$'){                                                    /* check for '$' */
			gga_index = 0;
			rmc_index = 0;
			is_it_gga_string = 0;
			is_it_rmc_string = 0;
			comma_counter_gga = 0;
			comma_counter_rmc = 0;
		}

		if(is_it_gga_string == 1)
		{                                             /* if true save GGA info. into buffer */
			if(received_char == ',' )
			{
			 	gga_pointers[comma_counter_gga++] = gga_index;    /* store instances of ',' in buffer */
			}
			gga_buffer[gga_index++] = received_char;
        }
		else if(gga_code[0] == 'G' && gga_code[1] == 'G' && gga_code[2] == 'A'){ /* check for GGA string */
			is_it_gga_string = 1;
			gga_code[0] = 0; 
			gga_code[1] = 0; 
			gga_code[2] = 0;	
		}
		else{
			gga_code[0] = gga_code[1];
			gga_code[1] = gga_code[2]; 
			gga_code[2] = received_char; 
		}	

		if(is_it_rmc_string == 1)
		{                                             /* if true save GGA info. into buffer */
			if(received_char == ',' )
			{
			 	rmc_pointers[comma_counter_rmc++] = rmc_index;    /* store instances of ',' in buffer */
			}
			rmc_buffer[rmc_index++] = received_char;
        }
		else if(rmc_code[0] == 'R' && rmc_code[1] == 'M' && rmc_code[2] == 'C'){ /* check for GGA string */
			is_it_rmc_string = 1;
			rmc_code[0] = 0; 
			rmc_code[1] = 0; 
			rmc_code[2] = 0;	
		}
		else{
			rmc_code[0] = rmc_code[1];
			rmc_code[1] = rmc_code[2]; 
			rmc_code[2] = received_char;
        }	
}



/******************************************************************************
 * Funcao:		unsigned int gps_segundos(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna os segundos atuais
 *****************************************************************************/
unsigned int gps_segundos()
{
  return ((rawTime[4] - '0') * 10 + (rawTime[5] - '0'));
}

/******************************************************************************
 * Funcao:		unsigned int gps_minutos(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna os minutos atuais
 *****************************************************************************/
unsigned int gps_minutos()
{
  return ((rawTime[2] - '0') * 10 + (rawTime[3] - '0'));
}

/******************************************************************************
 * Funcao:		unsigned int gps_hora(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna o hora atual
 *****************************************************************************/

unsigned int gps_hora()
{
  return (((rawTime[0] - '0') * 10 + (rawTime[1] - '0')));
}

/******************************************************************************
 * Funcao:		unsigned int gps_dia(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna o dia atual
 *****************************************************************************/

unsigned int gps_dia(void) 
{
  return ((rawDate[0] - '0') * 10 + (rawDate[1] - '0'));
}


/******************************************************************************
 * Funcao:		unsigned int gps_mes(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna o mes atual
 *****************************************************************************/

unsigned int gps_mes(void) 
{
  return ((rawDate[2] - '0') * 10 + (rawDate[3] - '0'));
}

/******************************************************************************
 * Funcao:		unsigned int gps_ano(void)
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned int
 * Descricao:	retorna o ano tual
 *****************************************************************************/

unsigned int gps_ano(void)
{
  return ((rawDate[4] - '0') * 10 + (rawDate[5] - '0'));
}

/******************************************************************************
 * Funcao:		void latitude_to_convert(unsigned char index)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia latitude para a funcao do fence para conversao
 *****************************************************************************/

void latitude_to_convert(void) 
{   
    unsigned char i,j;
    unsigned char latitude[9];//, teste[11] = "-2340.59642";
    
    j = 0;
    for(i=0; i<12; i++)
    {
        if(rawLatitude[i] != '\0' && rawLatitude[i] != '-' && rawLatitude[i]!= '.' && j <9)
        {
            latitude[j] = rawLatitude[i];
            j++;
        }   
    }
    armazena_ltlo(latitude, 1);
}

/******************************************************************************
 * Funcao:		void latitude_to_convert(unsigned char index)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia longitude para a funcao do fence para conversao
 *****************************************************************************/

void longitude_to_convert(void) 
{
    unsigned char i,j;
    unsigned char longitude[11];
    
    j = 0;
    for(i=0; i<12; i++)
    {
        if(rawLongitude[i] != '\0' && rawLongitude[i] != '-' && rawLongitude[i]!= '.' && j <10)
        {
        
            longitude[j] = rawLongitude[i];
            j++;
        }   
    }
    armazena_ltlo(longitude, 0);
}


/******************************************************************************
 * Funcao:		char posicao_long(void)
 * Entrada:		Nenhuma (void)
 * Saida:		char
 * Descricao:	Retorna a posicao geografica, por exemplo se o veiculo se contra 
 * no oriente ou ocidente
 *****************************************************************************/

char posicao_long(void) 
{
   return rawLongitude[0];
}

/******************************************************************************
 * Funcao:		char posicao_lat(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Retorna a posicao geografica, por exemplo se o veiculo se contra 
 * no hemisfério norte ou sul
 *****************************************************************************/

char posicao_lat(void) 
{   
    return rawLatitude[0];
}

/******************************************************************************
 * Funcao:		char *latitude(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Char
 * Descricao:	Retorna o dado bruto da latitude 
 *****************************************************************************/
char *latitude(void) 
{   
    return rawLatitude;
}

/******************************************************************************
 * Funcao:		char *longitude(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Char
 * Descricao:	Retorna o dado bruto da longitude 
 *****************************************************************************/
char *longitude(void) 
{   
    return rawLongitude;
}
/******************************************************************************
 * Funcao:		char *rawhora(void)  
 * Entrada:		Nenhuma (void)
 * Saida:		Char
 * Descricao:	Retorna o dado bruto da hora atual
 *****************************************************************************/

char *rawhora(void) 
{   
    return rawTime;
}

/******************************************************************************
 * Funcao:		char *rawdata(void)   
 * Entrada:		Nenhuma (void)
 * Saida:		Char
 * Descricao:	Retorna o dado bruto da data atual
 *****************************************************************************/
char *rawdata(void) 
{   
    return rawDate;
}

/******************************************************************************
 * Funcao:		char *fix(void)
 * Entrada:		Nenhuma (void)
 * Saida:		char
 * Descricao:	Retorna o fix
 *****************************************************************************/

char *fix(void)
{
    return rawFix;
}

/******************************************************************************
 * Funcao:		char *fix(void)
 * Entrada:		Nenhuma (void)
 * Saida:		char
 * Descricao:	Retorna a  velocidade
 *****************************************************************************/
char *Speed()
{
  return rawSpeed;
}

/******************************************************************************
 * Funcao:		void mostra_dados_display(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Mostra no display os dados brutos de latitude e longitude
 * a funcao e chamada pelo arquivo main.c
 *****************************************************************************/
void mostra_dados_display(void)
{
    posicao_cursor_lcd(1,4);
    escreve_frase_ram_lcd(rawLatitude);
    posicao_cursor_lcd(2,4);
    escreve_frase_ram_lcd(rawLongitude);
}


/******************************************************************************
 * Funcao:		void dados_gps_to_sd(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia os dados para o SDCard.c para efetuar a gravacao no SDCard
 *****************************************************************************/

void dados_gps_to_sd(void)
{

    monta_sd(0, rawTime);
    monta_sd(1, rawDate);
    monta_sd(2, rawLatitude);
    monta_sd(3, rawLongitude);
    monta_sd(5, rawTime);
    monta_sd(6, rawDate);
    monta_sd(7, rawSpeed);
    return;
}

/******************************************************************************
 * Funcao:		char verifica_recep_gps(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Char
 * Descricao:	Verifica se todas as informacoes necessarias foram recebidas para
 * evitar perda de dados
 *****************************************************************************/


char verifica_recep_gps(void)
{
    if(gps_flag[0].flag && gps_flag[1].flag && gps_flag[2].flag && gps_flag[3].flag && gps_flag[4].flag && gps_flag[5].flag)
    {        
        gps_flag[0].flag = 0;
        gps_flag[1].flag = 0;
        gps_flag[2].flag = 0;
        gps_flag[3].flag = 0;
        gps_flag[4].flag = 0;
        gps_flag[5].flag = 0;
        return 1;
    }
    else
    {

        return 0;
    }
    
}

/*Final do Arquivo modelo.c **************************************************/