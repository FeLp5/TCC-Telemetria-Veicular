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

    
typedef struct
{
    unsigned char flag:1;
}bit_field_gps;
/******************************************************************************
* Prototipos de funcoes
******************************************************************************/

void stringcpy(char *str1, char *str2, int dir, unsigned char size);
int GPSRead(unsigned char buff);

unsigned int GPSSecond();
unsigned int GPSMinute();
unsigned int GPSHour();
unsigned int GPSyear();
unsigned int GPSMonth();
unsigned int GPSDay();
void latitude_to_convert(unsigned char index);
void longitude_to_convert(unsigned char index);
char posicao_lat(void);
char posicao_long(void);
char *latitude_to_display(void);
char *longitude_to_display(void);
char *fix(void);
char *Speed(void);
void dados_gps_to_sd(void);
char verifica_recep_gps(void);
void mostra_dados_display(void);
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	