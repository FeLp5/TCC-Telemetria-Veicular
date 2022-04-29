/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB v3.51, XC8 versao 2.32, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Conceicao
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
    
    
#define _GPRMC_  1
#define _GPGGA_  2
#define _OTHER_  3

/******************************************************************************
* Prototipos de funcoes
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
//uint8_t GPSSecond();
//uint8_t GPSMinute();
//uint8_t GPSHour();
//uint8_t GPSDay();
//uint8_t GPSMonth();
//uint8_t GPSyear();
//float parse_rawDegree(char *term_);
//float Latitude();
//float Longitude();
//float Altitude();
//uint8_t Satellites();
//float Speed();
//float Course();

unsigned int GPSSecond();
unsigned int GPSMinute();
unsigned int GPSHour();
unsigned int GPSyear();
unsigned int GPSMonth();
unsigned int GPSDay();
char *Latitude(void);
char *Longitude(void);
char *fix(void);
void dados_gps_to_sd(void);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	