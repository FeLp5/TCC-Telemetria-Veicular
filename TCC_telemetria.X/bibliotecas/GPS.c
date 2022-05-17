///******************************************************************************
// * Nome do Arquivo 	: GPS.c
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB v3.51, XC8 versao 2.32, PIC18F4550
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
#include "display_lcd.h"
#include "fence.h"


bit_field_gps gps_flag[6];


int GPRMC_ok = 0, GPGGA_ok = 0;

uint8_t char_number = 0, SentenceType = 0, Term;

char sentence[6];
char rawTime[11]; 
char rawDate[7];
char rawSpeed[6];
char rawCourse[6];
char rawSatellites[3];
char rawAltitude[7];
char buffer[12];
char rawFix[2];

char rawLatitude[11];
char rawLongitude[12];

void stringcpy(char *str1, char *str2, int dir, unsigned char size)
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
        stringcpy(buffer, sentence, 0, 5);
        if(strcmp(sentence, "GPRMC") == 0)
          SentenceType = _GPRMC_;
        else if(strcmp(sentence, "GPGGA") == 0)
               SentenceType = _GPGGA_;
             else
               SentenceType = _OTHER_;
      }

      // Time
      if(Term == 1 && SentenceType == _GPRMC_) {
        stringcpy(buffer, rawTime, 0, 6);

        gps_flag[0].flag = 1;
      }

      // Latitude
      if((Term == 3) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawLatitude, 1, 10);
        gps_flag[1].flag = 1;
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
        stringcpy(buffer, rawLongitude, 1, 11);
        gps_flag[2].flag = 1;
      }
      // Longitude E/W
      if((Term == 6) && (SentenceType == _GPRMC_)) {
        if(buffer[0] == 'E')
          rawLongitude[0] = '0';
        else
          rawLongitude[0] = '-';
      }

      // Speed;
      if((Term == 7) && (SentenceType == _GPRMC_)) {
        stringcpy(buffer, rawSpeed, 0, 3);
        gps_flag[5].flag = 1;
      }

      // Course
//      if((Term == 8) && (SentenceType == _GPRMC_)) {
//        stringcpy(buffer, rawCourse, 0);
//      }

      // Date
      if(Term == 9 && SentenceType == _GPRMC_) {
        stringcpy(buffer, rawDate, 0, 6);
        gps_flag[3].flag = 1;
      }

      // Satellites
//      if((Term == 7) && (SentenceType == _GPGGA_)) {
//        stringcpy(buffer, rawSatellites, 0);
//      }

      // Altitude
//      if((Term == 9) && (SentenceType == _GPGGA_)) {
//        stringcpy(buffer, rawAltitude, 0);
//      }

      
     if((Term == 6) && (SentenceType == _GPGGA_)) {
        stringcpy(buffer, rawFix, 0, 2);
        gps_flag[4].flag = 1;
//        rawFix = buffer;
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
unsigned int GPSSecond() 
{
  return ((rawTime[4] - '0') * 10 + (rawTime[5] - '0'));
}
unsigned int GPSMinute()
{
  return ((rawTime[2] - '0') * 10 + (rawTime[3] - '0'));
}
unsigned int GPSHour()
{
  return (((rawTime[0] - '0') * 10 + (rawTime[1] - '0')));
}
//
unsigned int GPSDay() 
{
  return ((rawDate[0] - '0') * 10 + (rawDate[1] - '0'));
}
unsigned int GPSMonth() 
{
  return ((rawDate[2] - '0') * 10 + (rawDate[3] - '0'));
}
unsigned int GPSyear()
{
  return ((rawDate[4] - '0') * 10 + (rawDate[5] - '0'));
}


void latitude_to_convert(unsigned char index) 
{   
    unsigned char i,j;
    unsigned char latitude[9];//, teste[11] = "-2340.59642";
    strcpy(rawLatitude, "-2337.66653");
    
    j = 0;
    for(i=0; i<12; i++)
    {
        if(rawLatitude[i] != '\0' && rawLatitude[i] != '-' && rawLatitude[i]!= '.' && j <9)
        {
            latitude[j] = rawLatitude[i];
            j++;
        }   
    }
    armazena_ltlo(latitude, index);
}

void longitude_to_convert(unsigned char index) 
{
    unsigned char i,j;
    unsigned char longitude[11];
    strcpy(rawLongitude, "-04639.81479");
    
    j = 0;
    for(i=0; i<12; i++)
    {
        if(rawLongitude[i] != '\0' && rawLongitude[i] != '-' && rawLongitude[i]!= '.' && j <10)
        {
        
            longitude[j] = rawLongitude[i];
            j++;
        }   
    }
    armazena_ltlo(longitude, index);
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


char *latitude_to_display(void) 
{   
//    strcpy(rawLatitude, "-2336.46653");
    return rawLatitude;
}

char *longitude_to_display(void) 
{   
//    strcpy(rawLatitude, "-2336.46653");
    return rawLongitude;
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
//    gps_flag[0].flag = 1;
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