///*
//    GPS Information extraction using PIC18F4550 
//    http://www.electronicwings.com
//*/
//
//#include <pic18f4550.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include "GPS.h"
//#include "display_lcd.h"
//#include "uart.h"
//#include "SDCard.h"
//
//
//// Protótipos de funções
//unsigned long int get_gpstime();
//float get_latitude(unsigned char);
//float get_longitude(unsigned char);
//unsigned long int get_dt(unsigned char);
//void convert_time_to_utc(unsigned long int);
//unsigned char convert_to_date(unsigned char dt[]);
//float convert_to_degrees(float);
////float get_altitude(unsigned char);
////float get_sv(unsigned char date_pointer);
//
//
////variáveis
//char *teste;
//char gga_buffer[GGA_BUFFER_SIZE];              /* to store GGA string */
//char rmc_buffer[RMC_BUFFER_SIZE];
//
//char gga_code[3];
//char rmc_code[3];
//
//unsigned char n_s, E_W;                        /* for getting direction polarity */
//
//unsigned char gga_pointers[GGA_POINTERS_SIZE]; /* to store instances of ',' */
//unsigned char rmc_pointers[RMC_POINTERS_SIZE];
//
//char comma_counter_gga;
//char comma_counter_rmc;
//
//char data_buffer[15];
////char dt_buffer[6];
//char dado_buffer[7];    
//
//
//volatile unsigned int gga_index;
//volatile unsigned int rmc_index;
//
//volatile unsigned char is_it_gga_string	= 0;
//volatile unsigned char is_it_rmc_string = 0;
//
//void gps(void)
//{
//    inicializa_uart();
//	unsigned char data_out[34];
//	unsigned long int time;
//    unsigned long int date;
//	float latitude, longitude;
//	char gps_buffer[15];
//    
//
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
//}
//
//
//
///******************************************************************************
// * Funcao:		unsigned long int get_gpstime()
// * Entrada:		Nenhuma (void)
// * Saida:		unsigned long int
// * Descricao:	Coleta a hora atual
// *****************************************************************************/
//
//unsigned long int get_gpstime()
//{
//	unsigned char index;
//	unsigned char time_buffer[15];
//	unsigned long int _time;
//	
//	/* parse time in GGA string stored in buffer */
//	for(index = 0; gga_buffer[index]!=','; index++)
//	{		
//		time_buffer[index] = gga_buffer[index];
//	}
//	_time= atol(time_buffer);        /* convert string of time to integer */
//	return _time;                    /* return integer raw value of time */        
//}
//
//
//
///******************************************************************************
// * Funcao:		float get_latitude(char lat_pointer)
// * Entrada:		char lat_pointer
// * Saida:		float
// * Descricao:	Retorna a latitude
// *****************************************************************************/
//float get_latitude(char lat_pointer)
//{
//	unsigned char lat_index = lat_pointer+1;	/* index pointing to the latitude */
//	unsigned char index = 0;
//	char lat_buffer[15];
//	float _latitude;
//
//	/* parse Latitude in GGA string stored in buffer */
//	for(;gga_buffer[lat_index]!=',';lat_index++){
//		lat_buffer[index]= gga_buffer[lat_index];
//		index++;
//	}
//    lat_index++;
//    n_s = gga_buffer[lat_index];
//	_latitude = atof(lat_buffer);     /* convert string of latitude to float */
//	return _latitude;                 /* return float raw value of Latitude */
//}
//
//
///******************************************************************************
// * Funcao:		float get_longitude(unsigned char long_pointer)
// * Entrada:		long_pointer
// * Saida:		float
// * Descricao:	Retorna a longitude
// *****************************************************************************/
//
//float get_longitude(unsigned char long_pointer)
//{
//	unsigned char long_index;
//	unsigned char index = long_pointer+1;		/* index pointing to the longitude */
//	char long_buffer[15];
//	float _longitude;
//	long_index=0;
//	
//	/* parse longitude in GGA string stored in buffer */
//	for( ; gga_buffer[index]!=','; index++){
//		long_buffer[long_index]= gga_buffer[index];
//		long_index++;
//	}
//    long_index++;
//    E_W = gga_buffer[long_index];
//	_longitude = atof(long_buffer);    /* convert string of longitude to float */
//	return _longitude;                 /* return float raw value of longitude */
//}
//
//
///******************************************************************************
// * Funcao:		unsigned long int get_date(unsigned char date_pointer)
// * Entrada:		unsigned long int get_date
// * Saida:		long int
// * Descricao:	Retorna a data do GPS 
// *****************************************************************************/
//unsigned long int get_dt(unsigned char dt_pointer)
//{
//    unsigned char dt_index = dt_pointer + 1; //dt_pointer+1;
//	unsigned long int _dt;
//    unsigned char index;
//	index = 0;
//	/* parse time in GGA string stored in buffer */
//
//    for(;rmc_buffer[dt_index]!=',';dt_index++){
//		dado_buffer[index]= rmc_buffer[dt_index];
//		index++;
//	}
//    
//    dado_buffer[strlen(dado_buffer) - 0] = '\0';
//
//    
//  
//    
//    // testando
////    LIMPA_DISPLAY();
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd(rmc_buffer);
////    __delay_ms(1000);
////      LIMPA_DISPLAY();    
//      
////      
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd(dado_buffer);
////    __delay_ms(1000);
//      //    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd(dado_buffer);
////    __delay_ms(1000);
//      
////      int tamanho = strlen(dado_buffer);
////      posicao_cursor_lcd(1,0);
////    escreve_inteiro_lcd(tamanho);
////    __delay_ms(1000);
//
////    _dt = atol(dado_buffer);        /* convert string of time to integer */
////	return _dt;   
//    /* return integer raw value of time */    
////    return dado_buffer;
//}
//
//
///******************************************************************************
// * Funcao:		void convert_time_to_utc(unsigned long int utc_time)
// * Entrada:		unsigned long int utc_time
// * Saida:		float
// * Descricao:	Converte de float para formato de hora
// *****************************************************************************/
//
//void convert_time_to_utc(unsigned long int utc_time)
//{
//    unsigned int hour, min, sec;
//	hour = (utc_time / 10000) + LOCAL;                  /* extract hour from integer */
//	min = (utc_time % 10000) / 100;             /* extract minute from integer */
//	sec = (utc_time % 10000) % 100;             /* extract second from integer*/
//	sprintf(data_buffer, "%d:%d:%d", hour,min,sec); /* store UTC time in buffer */
//}
//
//unsigned char convert_to_date(unsigned char dt[])
//{
////    posicao_cursor_lcd(2,0);
////    escreve_frase_ram_lcd(dt_buffer[1]);
////    
////    __delay_ms(2000);
////    int tamanho = strlen(dt);
////     posicao_cursor_lcd(2,0);
////    escreve_inteiro_lcd(tamanho);
////    strncat (dt, "\0", 5);
//    
////  data_buffer = dt;
//    
////    posicao_cursor_lcd(2,6);
////    escreve_frase_ram_lcd(dt);
////	unsigned int dia, mes, ano;
////	dia = dt/10000; 
////    unsigned long int temp = dia * 10000;   /* extract hour from integer */
////	mes = (dt - temp) / 100 ; 
////    temp = dt * 100; /* extract minute from integer */
////	ano = dt - temp;             /* extract second from integer*/
////	sprintf(dt_buffer, "%d/%d/%d", dia,mes,ano); /* store UTC time in buffer */
//   
////    return dt;
//}
//
//
//
//float convert_to_degrees(float nmea_lat_long){
//	float minutes, dec_deg, decimal;
//	int degrees;
//	float position;
//    degrees = (int)(nmea_lat_long/100.00);
//    minutes = nmea_lat_long - degrees*100.00;
//    dec_deg = minutes / 60.00;
//    decimal = degrees + dec_deg;
//    if (n_s == 'S' || E_W == 'W') { // return negative
//        decimal *= -1;
//    }	
//	/* convert raw latitude/longitude into degree format */
//	return decimal;
//}
//
//
//
///******************************************************************************
// * Funcao:		void convert_time_to_utc(unsigned long int utc_time)
// * Entrada:		unsigned long int utc_time
// * Saida:		float
// * Descricao:	Converte de float para formato de hora
// *****************************************************************************/
//
//void tratamento_uart(unsigned char received_char)   
//{
//		if(received_char =='$'){                                                    /* check for '$' */
//			gga_index = 0;
//			rmc_index = 0;
//			is_it_gga_string = 0;
//			is_it_rmc_string = 0;
//			comma_counter_gga = 0;
//			comma_counter_rmc = 0;
//		}
//
//		if(is_it_gga_string == 1)
//		{                                             /* if true save GGA info. into buffer */
//			if(received_char == ',' )
//			{
//			 	gga_pointers[comma_counter_gga++] = gga_index;    /* store instances of ',' in buffer */
//			}
//			gga_buffer[gga_index++] = received_char;
//        }
//		else if(gga_code[0] == 'G' && gga_code[1] == 'G' && gga_code[2] == 'A'){ /* check for GGA string */
//			is_it_gga_string = 1;
//			gga_code[0] = 0; 
//			gga_code[1] = 0; 
//			gga_code[2] = 0;	
//		}
//		else{
//			gga_code[0] = gga_code[1];
//			gga_code[1] = gga_code[2]; 
//			gga_code[2] = received_char; 
//		}	
//
//		if(is_it_rmc_string == 1)
//		{                                             /* if true save GGA info. into buffer */
//			if(received_char == ',' )
//			{
//			 	rmc_pointers[comma_counter_rmc++] = rmc_index;    /* store instances of ',' in buffer */
//			}
//			rmc_buffer[rmc_index++] = received_char;
//        }
//		else if(rmc_code[0] == 'R' && rmc_code[1] == 'M' && rmc_code[2] == 'C'){ /* check for GGA string */
//			is_it_rmc_string = 1;
//			rmc_code[0] = 0; 
//			rmc_code[1] = 0; 
//			rmc_code[2] = 0;	
//		}
//		else{
//			rmc_code[0] = rmc_code[1];
//			rmc_code[1] = rmc_code[2]; 
//			rmc_code[2] = received_char;
//        }	
//}




///******************************************************************************
// * Nome do Arquivo 	: GPS.c
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
//// GPS Library for CCS PIC C compiler
//// http://simple-circuit.com/
// *****************************************************************************/





#define _GPRMC_  1
#define _GPGGA_  2
#define _OTHER_  3

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "GPS.h"
#include "SDCard.h"
int GPRMC_ok = 0, GPGGA_ok = 0;
uint8_t char_number = 0, SentenceType = 0, Term;
char sentence[6], rawTime[11], rawDate[7], rawSpeed[6], rawCourse[6], rawSatellites[3],
     rawLatitude[13], rawLongitude[13], rawAltitude[7], buffer[12];

void stringcpy(char *str1, char *str2, int dir)
{
  uint8_t chr = 0;
//  dir=0;
  do {
    str2[chr + dir] = str1[chr];
  } while(str1[chr++] != '\0');
}

int GPSRead(unsigned char buff) 
{
  uint8_t c = buff;

  switch(c) {
    case '\r':  // sentence end
      if(SentenceType == _GPRMC_)
        GPRMC_ok = 1;
      if(SentenceType == _GPGGA_)
        GPGGA_ok = 1;
      if(GPRMC_ok && GPGGA_ok) {
        GPRMC_ok = GPGGA_ok = 0;
        return 1;
      }
      break;

    case '$': // sentence start
      Term = char_number = 0;
      break;

    case ',':  // term end (new term start)
      buffer[char_number] = '\0';
      if(Term == 0) {
        stringcpy(buffer, sentence, 0);
        if(strcmp(sentence, "GPRMC") == 0)
          SentenceType = _GPRMC_;
        else if(strcmp(sentence, "GPGGA") == 0)
               SentenceType = _GPGGA_;
             else
               SentenceType = _OTHER_;
      }

      // Time
      if(Term == 1 && SentenceType == _GPRMC_) {
        stringcpy(buffer, rawTime, 0);
      }

      // Latitude
      if((Term == 3) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawLatitude, 1);
      }
      // Latitude N/S
      if((Term == 4) && (SentenceType == _GPRMC_)) {
        if(buffer[0] == 'N')
          rawLatitude[0] = '0';
        else
          rawLatitude[0] = '-';
      }

      // Longitude
      if((Term == 5) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawLongitude, 1);
      }
      // Longitude E/W
      if((Term == 6) && (SentenceType == _GPRMC_)) {
        if(buffer[0] == 'E')
          rawLongitude[0] = '0';
        else
          rawLongitude[0] = '-';
      }

      // Speed
      if((Term == 7) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawSpeed, 0);
      }

      // Course
      if((Term == 8) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawCourse, 0);
      }

      // Date
      if(Term == 9 && SentenceType == _GPRMC_) {
        stringcpy(buffer, rawDate, 0);
      }

      // Satellites
      if((Term == 7) && (SentenceType == _GPGGA_)) {
        stringcpy(buffer, rawSatellites, 0);
      }

      // Altitude
      if((Term == 9) && (SentenceType == _GPGGA_)) {
        stringcpy(buffer, rawAltitude, 0);
      }
      Term++;
      char_number = 0;
      break;

    default:
      buffer[char_number++] = c;
      break;
  }

  return 0;
}
//
//uint8_t GPSSecond() 
//{
//  return ((rawTime[4] - '0') * 10 + (rawTime[5] - '0'));
//}
//uint8_t GPSMinute()
//{
//  return ((rawTime[2] - '0') * 10 + (rawTime[3] - '0'));
//}
//uint8_t GPSHour()
//{
//  return ((rawTime[0] - '0') * 10 + (rawTime[1] - '0'));
//}
//
//uint8_t GPSDay() 
//{
//  return ((rawDate[0] - '0') * 10 + (rawDate[1] - '0'));
//}
//uint8_t GPSMonth() 
//{
//  return ((rawDate[2] - '0') * 10 + (rawDate[3] - '0'));
//}
//uint8_t GPSyear()
//{
//  return ((rawDate[4] - '0') * 10 + (rawDate[5] - '0'));
//}
//
//float parse_rawDegree(char *term_) 
//{
//  float term_value = atof(term_)/100;
//  int16_t term_dec = term_value;
//  term_value -= term_dec;
//  term_value  = term_value * 5/3 + term_dec;
//  return term_value;
//}
//
char  *Latitude(void) 
{
  return rawLatitude;
}
//
char *Longitude(void) 
{
  return rawLongitude;
}
//
//float Altitude()
//{
//  return atof(rawAltitude);
//}
//
//uint8_t Satellites()
//{
//  return atoi(rawSatellites);
//}
//
//float Speed()
//{
//  return (atof(rawSpeed) * 1.852);
//}
//
//float Course()
//{
//  return atof(rawCourse);
//}



void dados_gps_to_sd(void)
{
    monta_sd(0, rawTime);
    monta_sd(1, rawDate);
    monta_sd(2, rawLatitude);
    monta_sd(3, rawLongitude);
    return;
}