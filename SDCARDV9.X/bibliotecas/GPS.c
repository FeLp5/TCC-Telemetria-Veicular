/*
    GPS Information extraction using PIC18F4550 
    http://www.electronicwings.com
*/

#include <pic18f4550.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPS.h"
#include "display_lcd.h"
#include "uart.h"
#include "integer.h"


// Protótipos de funções
//unsigned long int get_gpstime();
//float get_latitude(unsigned char);
//float get_longitude(unsigned char);
//unsigned long int get_dt(unsigned char);
//void convert_time_to_utc(unsigned long int);
//unsigned char convert_to_date(unsigned char dt[]);
//float convert_to_degrees(float);
//float get_altitude(unsigned char);
//float get_sv(unsigned char date_pointer);


//variáveis
fat_time f_time;

char gga_buffer[GGA_BUFFER_SIZE];              /* to store GGA string */
char rmc_buffer[RMC_BUFFER_SIZE];

char gga_code[3];
char rmc_code[3];

unsigned char n_s, E_W;                        /* for getting direction polarity */

unsigned char gga_pointers[GGA_POINTERS_SIZE]; /* to store instances of ',' */
unsigned char rmc_pointers[RMC_POINTERS_SIZE];

char comma_counter_gga;
char comma_counter_rmc;

char data_buffer[15];
//char dt_buffer[6];
char dado_buffer[7];    


volatile unsigned int gga_index;
volatile unsigned int rmc_index;

volatile unsigned char is_it_gga_string	= 0;
volatile unsigned char is_it_rmc_string = 0;

void gps(void)
{
    inicializa_uart();
	unsigned char data_out[34];
	unsigned long int time;
    unsigned long int date;
	float latitude, longitude;
	char gps_buffer[15];
    

    //hora
    time = get_gpstime();            /* Extract Time */
    convert_time_to_utc(time);       /* convert time to UTC */
    
//    
//    //data
//    date = get_dt(rmc_pointers[7]);/* Extract Latitude */
////    unsigned long int dt = "000000";
////    dt = convert_to_date(date);
//    //sprintf(gps_buffer,"%%",date);		/* convert float value to string */
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("Data:");
//    
//    posicao_cursor_lcd(2,7);
//    escreve_frase_ram_lcd(dado_buffer);
//
//    
//    __delay_ms(2000);
//    LIMPA_DISPLAY();
//    
//    //latidude
//    latitude = get_latitude(gga_pointers[0]); /* Extract Latitude */
//    latitude = convert_to_degrees(latitude);  /* convert raw latitude in degree decimal*/
//    sprintf(gps_buffer,"%.07f",latitude);			/* convert float value to string */
//    // LCD_String(gps_buffer);  
//    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Lat:");/* display latitude in degree */    
//    posicao_cursor_lcd(1,5);
//    escreve_frase_ram_lcd(gps_buffer);
//    memset(gps_buffer,0,15);
//       
//
//    
//    // LCD_String_xy(3,0,"Long: ");
//    longitude = get_longitude(gga_pointers[2]);/* Extract Latitude */
//    longitude = convert_to_degrees(longitude);/* convert raw longitude in degree decimal*/
//    sprintf(gps_buffer,"%.07f",longitude);		/* convert float value to string */
//    // LCD_String(gps_buffer);            				/* display latitude in degree */
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("Lon:");/* display latitude in degree */ 
//    posicao_cursor_lcd(2,5);
//    escreve_frase_ram_lcd(gps_buffer);       
//    memset(gps_buffer,0,15);
//
//    __delay_ms(2000);
//    LIMPA_DISPLAY();
//    time = get_gpstime();            /* Extract Time */
//    convert_time_to_utc(time);       /* convert time to UTC */
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Hora:");
//    posicao_cursor_lcd(1,7);
//    escreve_frase_ram_lcd(data_buffer);
//    
//    //data
//    date = get_dt(rmc_pointers[7]);/* Extract Latitude */
////    unsigned long int dt = "000000";
////    dt = convert_to_date(date);
//    //sprintf(gps_buffer,"%%",date);		/* convert float value to string */
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("Data:");
//    
//    posicao_cursor_lcd(2,7);
//    escreve_frase_ram_lcd(dado_buffer);
//
//    
//    __delay_ms(2000);
//    LIMPA_DISPLAY();
//    
//    //latidude
//    latitude = get_latitude(gga_pointers[0]); /* Extract Latitude */
//    latitude = convert_to_degrees(latitude);  /* convert raw latitude in degree decimal*/
//    sprintf(gps_buffer,"%.07f",latitude);			/* convert float value to string */;
//    // LCD_String(gps_buffer);  
    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Lat:");/* display latitude in degree */    
//    posicao_cursor_lcd(1,5);
//    escreve_frase_ram_lcd(gps_buffer);
//    memset(gps_buffer,0,15);
       

    
    // LCD_String_xy(3,0,"Long: ");
//    longitude = get_longitude(gga_pointers[2]);/* Extract Latitude */
//    longitude = convert_to_degrees(longitude);/* convert raw longitude in degree decimal*/
//    sprintf(gps_buffer,"%.07f",longitude);		/* convert float value to string */
//    // LCD_String(gps_buffer);            				/* display latitude in degree */
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("Lon:");/* display latitude in degree */ 
//    posicao_cursor_lcd(2,5);
//    escreve_frase_ram_lcd(gps_buffer);       
//    memset(gps_buffer,0,15);

//    __delay_ms(2000);
//    LIMPA_DISPLAY();

 
}



/******************************************************************************
 * Funcao:		unsigned long int get_gpstime()
 * Entrada:		Nenhuma (void)
 * Saida:		unsigned long int
 * Descricao:	Coleta a hora atual
 *****************************************************************************/

unsigned long int get_gpstime()
{
	unsigned char index;
	unsigned char time_buffer[15];
	unsigned long int _time;
	
	/* parse time in GGA string stored in buffer */
	for(index = 0; gga_buffer[index]!=','; index++)
	{		
		time_buffer[index] = gga_buffer[index];
	}
	_time= atol(time_buffer);        /* convert string of time to integer */
	return _time;                    /* return integer raw value of time */        
}



/******************************************************************************
 * Funcao:		float get_latitude(char lat_pointer)
 * Entrada:		char lat_pointer
 * Saida:		float
 * Descricao:	Retorna a latitude
 *****************************************************************************/
float get_latitude(char lat_pointer)
{
	unsigned char lat_index = lat_pointer+1;	/* index pointing to the latitude */
	unsigned char index = 0;
	char lat_buffer[15];
	float _latitude;

	/* parse Latitude in GGA string stored in buffer */
	for(;gga_buffer[lat_index]!=',';lat_index++){
		lat_buffer[index]= gga_buffer[lat_index];
		index++;
	}
    lat_index++;
    n_s = gga_buffer[lat_index];
	_latitude = atof(lat_buffer);     /* convert string of latitude to float */
	return _latitude;                 /* return float raw value of Latitude */
}


/******************************************************************************
 * Funcao:		float get_longitude(unsigned char long_pointer)
 * Entrada:		long_pointer
 * Saida:		float
 * Descricao:	Retorna a longitude
 *****************************************************************************/

float get_longitude(unsigned char long_pointer)
{
	unsigned char long_index;
	unsigned char index = long_pointer+1;		/* index pointing to the longitude */
	char long_buffer[15];
	float _longitude;
	long_index=0;
	
	/* parse longitude in GGA string stored in buffer */
	for( ; gga_buffer[index]!=','; index++){
		long_buffer[long_index]= gga_buffer[index];
		long_index++;
	}
    long_index++;
    E_W = gga_buffer[long_index];
	_longitude = atof(long_buffer);    /* convert string of longitude to float */
	return _longitude;                 /* return float raw value of longitude */
}


/******************************************************************************
 * Funcao:		unsigned long int get_date(unsigned char date_pointer)
 * Entrada:		unsigned long int get_date
 * Saida:		long int
 * Descricao:	Retorna a data do GPS 
 *****************************************************************************/
unsigned long int get_dt(unsigned char dt_pointer)
{
    unsigned char dt_index = dt_pointer + 1; //dt_pointer+1;
	unsigned long int _dt;
    unsigned char index;
    unsigned char count;
    unsigned char dia = "|"; 
    unsigned char mes = "|"; 
    unsigned char ano = "|";
	index = 0;
	/* parse time in GGA string stored in buffer */

    for(;rmc_buffer[dt_index]!=',';dt_index++){
		dado_buffer[index]= rmc_buffer[dt_index];
		index++;
	}
    
    dado_buffer[strlen(dado_buffer) - 0] = '\0';

    for(count = 0;count<=7; count++)
    {
        if(count < 2)
        {
            strcat(dia, dado_buffer[count]); 
        }
        else if(count>=2 && count<4)
        {
            strcat(mes, dado_buffer[count]); 
        }
        else
        {
            strcat(ano, dado_buffer[count]); 
        }
    }
    
    //armazena valores em uma estrutura para o sistema de arquivos
    
    f_time.dia = dia;
    f_time.mes = mes;      
    f_time.ano = ano;
    
}


/******************************************************************************
 * Funcao:		void convert_time_to_utc(unsigned long int utc_time)
 * Entrada:		unsigned long int utc_time
 * Saida:		float
 * Descricao:	Converte de float para formato de hora
 *****************************************************************************/

void convert_time_to_utc(unsigned long int utc_time)
{
    unsigned int hour, min, sec;
    DWORD data_time; //new for return to the get_fattime
	hour = (utc_time / 10000) + LOCAL;                  /* extract hour from integer */
	min = (utc_time % 10000) / 100;             /* extract minute from integer */
	sec = (utc_time % 10000) % 100;             /* extract second from integer*/
//	sprintf(data_buffer, "%d:%d:%d", hour,min,sec); /* store UTC time in buffer */
//    
    f_time.hora = hour;
    f_time.min = min;      
    f_time.seg = sec;
}

unsigned char convert_to_date(unsigned char dt[])
{
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd(dt_buffer[1]);
//    
//    __delay_ms(2000);
//    int tamanho = strlen(dt);
//     posicao_cursor_lcd(2,0);
//    escreve_inteiro_lcd(tamanho);
//    strncat (dt, "\0", 5);
    
//  data_buffer = dt;
    
//    posicao_cursor_lcd(2,6);
//    escreve_frase_ram_lcd(dt);
//	unsigned int dia, mes, ano;
//	dia = dt/10000; 
//    unsigned long int temp = dia * 10000;   /* extract hour from integer */
//	mes = (dt - temp) / 100 ; 
//    temp = dt * 100; /* extract minute from integer */
//	ano = dt - temp;             /* extract second from integer*/
//	sprintf(dt_buffer, "%d/%d/%d", dia,mes,ano); /* store UTC time in buffer */
   
//    return dt;
}



float convert_to_degrees(float nmea_lat_long){
	float minutes, dec_deg, decimal;
	int degrees;
	float position;
    degrees = (int)(nmea_lat_long/100.00);
    minutes = nmea_lat_long - degrees*100.00;
    dec_deg = minutes / 60.00;
    decimal = degrees + dec_deg;
    if (n_s == 'S' || E_W == 'W') { // return negative
        decimal *= -1;
    }	
	/* convert raw latitude/longitude into degree format */
	return decimal;
}



/******************************************************************************
 * Funcao:		void convert_time_to_utc(unsigned long int utc_time)
 * Entrada:		unsigned long int utc_time
 * Saida:		float
 * Descricao:	Converte de float para formato de hora
 *****************************************************************************/

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