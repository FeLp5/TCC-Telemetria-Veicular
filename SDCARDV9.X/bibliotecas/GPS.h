/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Souza, Deivide ConceiÃ§Ã£o de
			  Silva, Felipe Alves da
			  Souza, Ricardo de
			  
 *
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial 
 ******************************************************************************/



#ifndef GPS_H
#define	GPS_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/ 
#include <p18f4550.h>

/*****************************************************************************/
 
/******************************************************************************
* Definicoes de pinos do microcontrolador
******************************************************************************/
/*-----------------------------------------------------------------------
/  PFF - Low level disk interface modlue include file    (C)ChaN, 2014
/-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif



/******************************************************************************
     * Defines
     ******************************************************************************/

#define GGA_BUFFER_SIZE 80
#define GGA_POINTERS_SIZE 20

#define RMC_BUFFER_SIZE  70
#define RMC_POINTERS_SIZE 12
    
#define LOCAL -3

/******************************************************************************
* Prototipos das funÃ§Ãµes
******************************************************************************/
//void tratamento_uart(unsigned char received_char); 
//void gps(void);
//unsigned long int get_gpstime();
//float get_latitude(unsigned char);
//float get_longitude(unsigned char);
//unsigned long int get_dt(unsigned char);
//void convert_time_to_utc(unsigned long int);
////void convert_time_to_utc(unsigned char *data);
//void gps_sd(void);
////void convert_time_to_utc(int *hora, int *min, int *seg);
//unsigned char convert_to_date(unsigned char dt[]);
//float convert_to_degrees(float);

    
void stringcpy(char *str1, char *str2, int dir);
int GPSRead(unsigned char buff);
uint8_t GPSSecond();
uint8_t GPSMinute();
uint8_t GPSHour();
uint8_t GPSDay();
uint8_t GPSMonth();
uint8_t GPSyear();
float parse_rawDegree(char *term_);
float Latitude();
float Longitude();
float Altitude();
uint8_t Satellites();
float Speed();
float Course();
void transf_dados_sd(void);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	/* _SDCARD_DEFINED */