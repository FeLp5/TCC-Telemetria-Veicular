/*
    GPS Information extraction using PIC18F4550 
    http://www.electronicwings.com
*/

#include <pic18f4550.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPS.h"

unsigned long int get_gpstime();
float get_latitude(unsigned char);
float get_longitude(unsigned char);
float get_altitude(unsigned char);
void convert_time_to_UTC(unsigned long int);
float convert_to_degrees(float);

char GGA_Buffer[GGA_Buffer_Size];              /* to store GGA string */
char GGA_CODE[3];

unsigned char N_S, E_W;                        /* for getting direction polarity */
unsigned char GGA_Pointers[GGA_Pointers_Size]; /* to store instances of ',' */
char CommaCounter_GGA;
char Data_Buffer[15];
volatile unsigned int GGA_Index;
volatile unsigned char	IsItGGAString	= 0;


char RCM_CODE[3];
char RCM_Buffer[RCM_Buffer_Size];
unsigned char RCM_Pointers[RCM_Pointers_Size];
volatile unsigned int RCM_Index;
volatile unsigned char IsItRCMString = 0;
char CommaCounter_RCM;

void GPS(void)
{

	unsigned char data_out[34];
	unsigned long int Time;
	float Latitude, Longitude, Altitude;
	char GPS_Buffer[15];
    
    // OSCCON = 0x72;      /* use internal osc. of 8MHz Freq. */

    // PIE1bits.RCIE=1;    /* enable Receive Interrupt */

	// USART_Init(9600);
	
//    while(1){
		memset(GPS_Buffer,0,15);
		Time = get_gpstime();            /* Extract Time */
		convert_time_to_UTC(Time);       /* convert time to UTC */
		// LCD_String(Data_Buffer);
		// LCD_String("  ");
			
		// LCD_String_xy(2,0,"Lat: ");
		Latitude = get_latitude(GGA_Pointers[0]); /* Extract Latitude */
		Latitude = convert_to_degrees(Latitude);  /* convert raw latitude in degree decimal*/
		sprintf(GPS_Buffer,"%.05f",Latitude);			/* convert float value to string */
		// LCD_String(GPS_Buffer);            				/* display latitude in degree */
		memset(GPS_Buffer,0,15);
			
		// LCD_String_xy(3,0,"Long: ");
		Longitude = get_longitude(GGA_Pointers[2]);/* Extract Latitude */
		Longitude = convert_to_degrees(Longitude);/* convert raw longitude in degree decimal*/
		sprintf(GPS_Buffer,"%.05f",Longitude);		/* convert float value to string */
		// LCD_String(GPS_Buffer);            				/* display latitude in degree */
		memset(GPS_Buffer,0,15);
				
		// LCD_String_xy(4,0,"Alt: ");
		// Altitude = get_altitude(GGA_Pointers[7]); /* Extract Latitude */
		// sprintf(GPS_Buffer,"%.2f",Altitude);			/* convert float value to string */
		// LCD_String(GPS_Buffer);            				/* display latitude in degree */


//		sprintf(Data_out,"",Longitude, Latitude, Time); //Uma ideia sobre a conversão dos dados e retorno
//		return Data_out; 
//	}
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
	unsigned char Time_Buffer[15];
	unsigned long int _Time;
	
	/* parse Time in GGA string stored in buffer */
	for(index = 0; GGA_Buffer[index]!=','; index++)
	{		
		Time_Buffer[index] = GGA_Buffer[index];
	}
	_Time= atol(Time_Buffer);        /* convert string of Time to integer */
	return _Time;                    /* return integer raw value of Time */        
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
	char Lat_Buffer[15];
	float _latitude;

	/* parse Latitude in GGA string stored in buffer */
	for(;GGA_Buffer[lat_index]!=',';lat_index++){
		Lat_Buffer[index]= GGA_Buffer[lat_index];
		index++;
	}
    lat_index++;
    N_S = GGA_Buffer[lat_index];
	_latitude = atof(Lat_Buffer);     /* convert string of latitude to float */
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
	char Long_Buffer[15];
	float _longitude;
	long_index=0;
	
	/* parse Longitude in GGA string stored in buffer */
	for( ; GGA_Buffer[index]!=','; index++){
		Long_Buffer[long_index]= GGA_Buffer[index];
		long_index++;
	}
    long_index++;
    E_W = GGA_Buffer[long_index];
	_longitude = atof(Long_Buffer);    /* convert string of longitude to float */
	return _longitude;                 /* return float raw value of Longitude */
}


/******************************************************************************
 * Funcao:		float get_altitude(unsigned char alt_pointer)
 * Entrada:		alt_pointer
 * Saida:		float
 * Descricao:	Retorna a altitude
 *****************************************************************************/


float get_altitude(unsigned char alt_pointer)
{
	unsigned char alt_index;
	unsigned char index = alt_pointer+1;	/* index pointing to the altitude */
	char Alt_Buffer[12];
	float _Altitude;
	alt_index=0;
	
	/* parse Altitude in GGA string stored in buffer */
	for( ; GGA_Buffer[index]!=','; index++){
		Alt_Buffer[alt_index]= GGA_Buffer[index];
		alt_index++;
	}
		_Altitude = atof(Alt_Buffer);   /* convert string of altitude to float */ 
	return _Altitude;                 /* return float raw value of Altitude */
}

/******************************************************************************
 * Funcao:		void convert_time_to_UTC(unsigned long int UTC_Time)
 * Entrada:		unsigned long int UTC_Time
 * Saida:		float
 * Descricao:	Converte de float para formato de hora
 *****************************************************************************/

void convert_time_to_UTC(unsigned long int UTC_Time)
{
	unsigned int hour, min, sec;
		
	hour = (UTC_Time / 10000);                  /* extract hour from integer */
	min = (UTC_Time % 10000) / 100;             /* extract minute from integer */
	sec = (UTC_Time % 10000) % 100;             /* extract second from integer*/

	sprintf(Data_Buffer, "%d:%d:%d", hour,min,sec); /* store UTC time in buffer */
	
}

float convert_to_degrees(float NMEA_lat_long){
	
	
	float minutes, dec_deg, decimal;
	
	int degrees;
	
	float position;

    degrees = (int)(NMEA_lat_long/100.00);
    minutes = NMEA_lat_long - degrees*100.00;
    dec_deg = minutes / 60.00;
    decimal = degrees + dec_deg;
    if (N_S == 'S' || E_W == 'W') { // return negative
        decimal *= -1;
    }	
	/* convert raw latitude/longitude into degree format */
	return decimal;
}

/******************************************************************************
 * Funcao:		void convert_time_to_UTC(unsigned long int UTC_Time)
 * Entrada:		unsigned long int UTC_Time
 * Saida:		float
 * Descricao:	Converte de float para formato de hora
 *****************************************************************************/

float get_date(unsigned char date_pointer)
{
	unsigned char date_index;
	unsigned char index = date_pointer + 1;		/* index pointing to the longitude */
	char date_buffer[15];
	unsigned long int _date;
	date_index=0;
	
	/* parse Longitude in GGA string stored in buffer */
	for( ; RCM_Buffer[index]!=','; index++){
		date_buffer[date_index]= RCM_Buffer[index];
		date_index++;
	}
	return _date;                 /* return integer raw value of Time */  
}


/******************************************************************************
 * Funcao:		void convert_time_to_UTC(unsigned long int UTC_Time)
 * Entrada:		unsigned long int UTC_Time
 * Saida:		float
 * Descricao:	Converte de float para formato de hora
 *****************************************************************************/

void tratamento_uart(unsigned char received_char)   
{
		if(received_char =='$'){                                                    /* check for '$' */
			GGA_Index = 0;
			RCM_Index = 0;
			IsItGGAString = 0;
			IsItRCMString = 0;
			CommaCounter_GGA = 0;
			CommaCounter_RCM = 0;
		}


		if(IsItGGAString == 1)
		{                                             /* if true save GGA info. into buffer */
			if(received_char == ',' )
			{
			 	GGA_Pointers[CommaCounter_GGA++] = GGA_Index;    /* store instances of ',' in buffer */
			}
			GGA_Buffer[GGA_Index++] = received_char;
        }
		else if(GGA_CODE[0] == 'G' && GGA_CODE[1] == 'G' && GGA_CODE[2] == 'A'){ /* check for GGA string */
			IsItGGAString = 1;
			GGA_CODE[0] = 0; 
			GGA_CODE[1] = 0; 
			GGA_CODE[2] = 0;	
		}
		else{
			GGA_CODE[0] = GGA_CODE[1];
			GGA_CODE[1] = GGA_CODE[2]; 
			GGA_CODE[2] = received_char; 
		}	



		if(IsItRCMString == 1)
		{                                             /* if true save GGA info. into buffer */
			if(received_char == ',' )
			{
			 	RCM_Pointers[CommaCounter_RCM++] = RCM_Index;    /* store instances of ',' in buffer */
			}
			RCM_Buffer[RCM_Index++] = received_char;
        }
		else if(RCM_CODE[0] == 'R' && RCM_CODE[1] == 'C' && RCM_CODE[2] == 'M'){ /* check for GGA string */
			IsItRCMString = 1;
			RCM_CODE[0] = 0; 
			RCM_CODE[1] = 0; 
			RCM_CODE[2] = 0;	
		}
		else{
			RCM_CODE[0] = RCM_CODE[1];
			RCM_CODE[1] = RCM_CODE[2]; 
			RCM_CODE[2] = received_char; 
		}	

}