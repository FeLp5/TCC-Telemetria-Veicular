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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "GPS.h"
#include "SDCard.h"
#include "display_lcd.h"
#include "fence.h"


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

void stringcpy(char *str1, char *str2, int dir, unsigned char size)
{
  uint8_t chr = 0;
//  dir=0;
  do {
    str2[chr + dir] = str1[chr];
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd(str2);
    chr++;
  } while(str1[chr] != '\0');
}



////variáveis
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


//char dt_buffer[6];
  
//
//
volatile unsigned int gga_index;
volatile unsigned int rmc_index;
//
volatile unsigned char is_it_gga_string	= 0;
volatile unsigned char is_it_rmc_string = 0;

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

   
//	unsigned char data_out[34];
//	unsigned long int time;
//    unsigned long int date;
//	float latitude, longitude;
//	char gps_buffer[15];
    

//    //hora
////    time = get_gpstime();            /* Extract Time */
////    convert_time_to_utc(time);       /* convert time to UTC */
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd("Hora:");
////    posicao_cursor_lcd(1,7);
////    escreve_frase_ram_lcd(data_buffer);
////    
////    //data
////    date = get_dt(rmc_pointers[7]);/* Extract Latitude */
//////    unsigned long int dt = "000000";
//////    dt = convert_to_date(date);
////    //sprintf(gps_buffer,"%%",date);		/* convert float value to string */
////    posicao_cursor_lcd(2,0);
////    escreve_frase_ram_lcd("Data:");
////    
////    posicao_cursor_lcd(2,7);
////    escreve_frase_ram_lcd(dado_buffer);
////
////    
////    __delay_ms(2000);
////    LIMPA_DISPLAY();
////    
////    //latidude
////    latitude = get_latitude(gga_pointers[0]); /* Extract Latitude */
////    latitude = convert_to_degrees(latitude);  /* convert raw latitude in degree decimal*/
////    sprintf(gps_buffer,"%.07f",latitude);			/* convert float value to string */
////    // LCD_String(gps_buffer);  
////    
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd("Lat:");/* display latitude in degree */    
////    posicao_cursor_lcd(1,5);
////    escreve_frase_ram_lcd(gps_buffer);
////    memset(gps_buffer,0,15);
////       
////
////    
////    // LCD_String_xy(3,0,"Long: ");
////    longitude = get_longitude(gga_pointers[2]);/* Extract Latitude */
////    longitude = convert_to_degrees(longitude);/* convert raw longitude in degree decimal*/
////    sprintf(gps_buffer,"%.07f",longitude);		/* convert float value to string */
////    // LCD_String(gps_buffer);            				/* display latitude in degree */
////    posicao_cursor_lcd(2,0);
////    escreve_frase_ram_lcd("Lon:");/* display latitude in degree */ 
////    posicao_cursor_lcd(2,5);
////    escreve_frase_ram_lcd(gps_buffer);       
////    memset(gps_buffer,0,15);
////
////    __delay_ms(2000);
////    LIMPA_DISPLAY();
//    time = get_gpstime();            /* Extract Time */
//    convert_time_to_utc(time);       /* convert time to UTC */
//    monta_sd(0, data_buffer, 0);
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd("Hora:");
////    posicao_cursor_lcd(1,7);
////    escreve_frase_ram_lcd(data_buffer);
//    
//    //data
//    date = get_dt(rmc_pointers[7]);/* Extract Latitude */
//    monta_sd(1, data_buffer, 0);
////    unsigned long int dt = "000000";
////    dt = convert_to_date(date);
////    //sprintf(gps_buffer,"%%",date);		/* convert float value to string */
////    posicao_cursor_lcd(2,0);
////    escreve_frase_ram_lcd("Data:");
////    
////    posicao_cursor_lcd(2,7);
////    escreve_frase_ram_lcd(dado_buffer);
//
//    
////    __delay_ms(2000);
////    LIMPA_DISPLAY();
//    
//    //latidude
//    latitude = get_latitude(gga_pointers[0]); /* Extract Latitude */
//    latitude = convert_to_degrees(latitude);  /* convert raw latitude in degree decimal*/
////    sprintf(gps_buffer,"%.07f",latitude);			/* convert float value to string */
//    // LCD_String(gps_buffer);  
//    monta_sd(2, data_buffer, 0);
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd("Lat:");/* display latitude in degree */    
////    posicao_cursor_lcd(1,5);
////    escreve_frase_ram_lcd(gps_buffer);
////    memset(gps_buffer,0,15);
//       
//
//    
//    // LCD_String_xy(3,0,"Long: ");
//    longitude = get_longitude(gga_pointers[2]);/* Extract Latitude */
//    longitude = convert_to_degrees(longitude);/* convert raw longitude in degree decimal*/
//    teste = ftoa(longitude, 7);
//    monta_sd(3, teste, 0);
////    sprintf(gps_buffer,"%.07f",longitude);		/* convert float value to string */
//    // LCD_String(gps_buffer);            				/* display latitude in degree */
////    posicao_cursor_lcd(2,0);
////    escreve_frase_ram_lcd("Lon:");/* display latitude in degree */ 
////    posicao_cursor_lcd(2,5);
////    escreve_frase_ram_lcd(gps_buffer);       
////    memset(gps_buffer,0,15);
//
////    __delay_ms(2000);
////    LIMPA_DISPLAY();
//
// 
}
//
//
//
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
//	unsigned long int _time;
	
	/* parse time in GGA string stored in buffer */
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
//	unsigned long int _time;
	index_lat = 1;
	/* parse time in GGA string stored in buffer */
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


void get_longitude(void)
{
	unsigned char index_long, index;
//	unsigned long int _time;
	index_long = 1;
	/* parse time in GGA string stored in buffer */
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

void get_rawdate(void)
{
	unsigned char index_date, index;
//	unsigned long int _time;
	index_date = 0;
	/* parse time in GGA string stored in buffer */
	for(index = 46; index<52; index++)
	{		
		rawDate[index_date] = rmc_buffer[index];
        index_date++;
	}
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd(rawDate);
}

void get_rawspeed(void)
{
	unsigned char index_speed, index;
//	unsigned long int _time;
	index_speed = 0;
	/* parse time in GGA string stored in buffer */
	for(index = 39; index<44; index++)
	{		
		rawSpeed[index_speed] = rmc_buffer[index];
        index_speed++;
	}
}


void get_fix(void)
{
	unsigned char index_fix, index;
//	unsigned long int _time;
	index_fix = 0;
	/* parse time in GGA string stored in buffer */
	for(index = 37; index<39; index++)
	{		
		rawFix[index_fix] = gga_buffer[index];
        index_fix++;
	}
//    posicao_cursor_lcd(2,0);
//    escreve_caractere_lcd(rawFix[0]);
}


///******************************************************************************
// * Funcao:		void convert_time_to_utc(unsigned long int utc_time)
// * Entrada:		unsigned long int utc_time
// * Saida:		float
// * Descricao:	Converte de float para formato de hora
// *****************************************************************************/
//
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



//
//int GPSRead(unsigned char buff) 
//{
//  uint8_t c = buff;
//
//  switch(c) {
//    case '\r':  // sentence end
//      if(SentenceType == _GPRMC_)
//        GPRMC_ok = 1;
//      if(SentenceType == _GPGGA_)
//        GPGGA_ok = 1;
//      if(GPRMC_ok && GPGGA_ok) {
//        GPRMC_ok = GPGGA_ok = 0;
//        return 1;
//      }
//      break;
//
//    case '$': // sentence start
//      Term = char_number = 0;
//      break;
//
//    case ',':  // term end (new term start)
//      buffer[char_number] = '\0';
//      if(Term == 0) 
//      {
//        stringcpy(buffer, sentence, 0, 5);
//        if(strcmp(sentence, "GPRMC") == 0)
//        {
//           SentenceType = _GPRMC_; 
//        }
//        else if(strcmp(sentence, "GPGGA") == 0)
//        {
//            SentenceType = _GPGGA_;
//        }
//        else
//        {
//            SentenceType = _OTHER_; 
//        }     
//      }
//
//      // Time
//      if(Term == 1 && SentenceType == _GPRMC_) {
//        stringcpy(buffer, rawTime, 0, 6);
//
//        gps_flag[0].flag = 1;
//      }
//
//      // Latitude
//      if((Term == 3) && (SentenceType == _GPRMC_)) {
//        if(rawFix[0] == '1')
//        {
//            stringcpy(buffer, rawLatitude, 1, 11);
//        }
//        gps_flag[1].flag = 1;
//      }
//      // Latitude N/S
//      if((Term == 4) && (SentenceType == _GPRMC_))
//      {
//        if(buffer[0] == 'N')
//          rawLatitude[0] = '0';
//        else
//          rawLatitude[0] = '-';
//      }
//
//      // Longitude
//      if((Term == 5) && (SentenceType == _GPRMC_)) {
//          if(rawFix[0] == '1')
//          {
//               stringcpy(buffer, rawLongitude, 1, 11);
//          }
//        gps_flag[2].flag = 1;
//      }
//      // Longitude E/W
//      if((Term == 6) && (SentenceType == _GPRMC_)) {
//        if(buffer[0] == 'E')
//          rawLongitude[0] = '0';
//        else
//          rawLongitude[0] = '-';
//      }
//
//      // Speed;
//      if((Term == 7) && (SentenceType == _GPRMC_)) {
//        stringcpy(buffer, rawSpeed, 0, 3);
//        gps_flag[5].flag = 1;
//      }
//
//      // Date
//      if(Term == 9 && SentenceType == _GPRMC_) {
//        stringcpy(buffer, rawDate, 0, 6);
//        gps_flag[3].flag = 1;
//      }
//      
//     if((Term == 6) && (SentenceType == _GPGGA_)) {
//        stringcpy(buffer, rawFix, 0, 2);
//        gps_flag[4].flag = 1;
//      }
//      Term++;
//      char_number = 0;
//    break;
//
//    default:
//        if(Term == 1)
//        {
//            if(char_number<6)
//            {
//                buffer[char_number++] = c;
//            }
////           buffer[char_number++] = ' ';
//        }
//        else if(Term != 1)
//        {
//            buffer[char_number++] = c;
//        } 
//       
//      break;
//  }
//
//  return 0;
//}




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
//    stringcpy("-2337.66653", rawLatitude, 0 , 11);
    
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
//    stringcpy("-04639.81479", rawLongitude, 0, 12);
    
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

char posicao_long(void) 
{
//   strcpy(rawLongitude, "-04633.94050");
   return rawLongitude[0];
}

char posicao_lat(void) 
{   
//    strcpy(rawLatitude, "-2336.46653");
    return rawLatitude[0];
}

char *latitude(void) 
{   
    return rawLatitude;
}

char *longitude(void) 
{   
    return rawLongitude;
}

char *rawhora(void) 
{   
    return rawTime;
}

char *rawdata(void) 
{   
    return rawDate;
}


char *fix(void)
{
    return rawFix;
}


char *Speed()
{
  return rawSpeed;
}


void mostra_dados_display(void)
{
    posicao_cursor_lcd(1,4);
    escreve_frase_ram_lcd(rawLatitude);
    posicao_cursor_lcd(2,4);
    escreve_frase_ram_lcd(rawLongitude);
}




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