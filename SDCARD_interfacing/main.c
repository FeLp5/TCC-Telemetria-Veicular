/*
 * File:   main.c
 * Author: Etiq Lab Desktop
 *
 * Created on January 27, 2017, 4:44 PM
 */



#include "pff.h"
#include "SDCard.h"
#include "pic_18f4550.h"
#include "SPI.h"


#define FOSC 20000000
#define Baud 9600
#define spbrg ((FOSC/Baud)/64)-1                                 /*
                                      ((FOSC/Desired Baud Rate)/64) ? 1
                                       = ((16000000/9600)/64) ? 1        */
                                       /*void Open1USART ( unsigned char config,  unsigned int spbrg);*/
BYTE sector_buffer[512];
unsigned char *rd;
unsigned char Result;
static unsigned char str[512];
BYTE s;
DWORD fileSize;
BYTE txt[20];
BYTE data_buffer_32[32];

void soft_hard_init()
{
    inicializa_SPI(modo_zero);
}





// File to read 
BYTE fileName[10] = {"check.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
BYTE folder[48] = {""}; 

void main(void) 
{
    FRESULT FResult;
    FATFS fs;
    WORD br;
    soft_hard_init(); // SOFTWARE AND HARWARE INITIALISE FUNCTION

    /*READ FUNCTION*/
    
        prossiga();
	if( (FResult = pf_mount(&fs)) == FR_OK )
	{

		// open file
		FResult = pf_open("check.txt"/*fileName*/);
        
		if( FResult == FR_OK )
		{

				// Read 31 bytes from the file                        
            if( (FResult = pf_write("SD_Card", 5, &br)) == FR_OK )
				{
				} 
				else
				{

					
					while( 1 );
				}

				
			// read file and print it until it ends
			do
			{
				// Read 31 bytes from the file
				if( (FResult = pf_read(data_buffer_32, 31, &br)) == FR_OK )
				{
					// putsUSART needs a C-string (NULL terminated)
					data_buffer_32[br] = 0;
                    break;
					//putsUART(data_buffer_32);
				}
				else
				{

					while( 1 );
				}
			} while( br == 31 );	// if the pf_Read reads less then 31 bytes the file has ended
		}
		else 
		{

			
			while( 1 );
		}
	}
	else
	{

		
		while( 1 );
	}
	

  	// do a directory listing and list all files on the SD-card
	//scan_files(folder);
		
	// unmount drive
	
	//FResult = pf_mount(NULL);
	
	if( FResult != FR_OK )
	{
	
		
		while( 1 );
	}	

	while(1);
/*WRITE FUNCTION*/
}		
