/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "SDCard.h"
#include "display_lcd.h"
#include "SPI.h"
#include "ff.h"
#include <ctype.h>



//ESTRUTURAS
typedef enum _SD_DataError
{
	SD_DATA_TOKEN_OK          = 0x00,
	SD_DATA_TOKEN_ERROR       = 0x01,
	SD_DATA_TOKEN_CC_ERROR    = 0x02,
	SD_DATA_TOKEN_ECC_FAILURE = 0x04,
	SD_DATA_TOKEN_OUT_OF_RANGE= 0x08,
	SD_DATA_TOKEN_CARD_LOCKED = 0x10,
} SD_DataError;



typedef enum _SD_CMD
{
	SD_CMD_GO_IDLE_STATE		=  0, /*!< CMD0  = 0x40, ARG=0x00000000, CRC=0x95 */
	SD_CMD_SEND_OP_COND			=  1, /*!< CMD1  = 0x41 */
	SD_CMD_SEND_IF_COND			=  8, /*!< CMD8  = 0x48, ARG=0x000001AA, CRC=0x87 */
	SD_CMD_SEND_APP				= 55, /*!< CMD55 = 0x77, ARG=0x00000000, CRC=0x65 */
	SD_CMD_ACTIVATE_INIT		= 41, /*!< ACMD41= 0x69, ARG=0x40000000, CRC=0x77 */
	SD_CMD_READ_OCR				= 58, /*!< CMD58 = 0x7A, ARG=0x00000000, CRC=0xFF */
	SD_CMD_SEND_CSD				=  9, /*!< CMD9  = 0x49 */
	SD_CMD_SEND_CID				= 10, /*!< CMD10 = 0x4A */
	SD_CMD_SEND_SCR				= 51, /*!< ACMD51= 0x73 */
	SD_CMD_STATUS				= 13, /*!< ACMD13= 0x4D */
	SD_CMD_STOP_TRANSMISSION	= 12, /*!< CMD12 = 0x4C */
	SD_CMD_SET_BLOCKLEN			= 16, /*!< CMD16 = 0x50 */
	SD_CMD_READ_SINGLE_BLOCK	= 17, /*!< CMD17 = 0x51 */
	SD_CMD_READ_MULT_BLOCK		= 18, /*!< CMD18 = 0x52 */
	SD_CMD_SET_BLOCK_COUNT		= 23, /*!< CMD23 = 0x57 */
	SD_CMD_WRITE_SINGLE_BLOCK	= 24, /*!< CMD24 = 0x58 */
	SD_CMD_WRITE_MULT_BLOCK		= 25, /*!< CMD25 = 0x59 */
	SD_CMD_ERASE_BLOCK_START	= 32, /*!< CMD32 = 0x60 */
	SD_CMD_ERASE_BLOCK_END		= 33, /*!< CMD33 = 0x61 */
	SD_CMD_ERASE				= 38  /*!< CMD38 = 0x66 */
} SD_CMD;

//=================


/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SD_DUMMY_BYTE		0xFF

BYTE *data_buff;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		stat = sdc_disk_status(pdrv);

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = sdc_disk_status(pdrv);

		// translate the reslut code here

		return stat;

	case DEV_USB :
//		result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
        stat = sdc_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		stat = sdc_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
//		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	BYTE count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = sdc_disk_read(buff, sector, count);
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("coletando data");
//        __delay_ms(2000);
        res = result;
		// translate the reslut code here
		return res;

	case DEV_MMC :
		// translate the arguments here

		result = sdc_disk_read(buff, sector, count);
        posicao_cursor_lcd(1,0);
        escreve_inteiro_lcd(result);
        __delay_ms(2000);
        res = result;
		// translate the reslut code here
        buff = data_buff;
		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	BYTE count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here
        result = sdc_disk_write(buff, sector, count);
//		result = RAM_disk_write(buff, sector, count);
        res = result;
		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = sdc_disk_write(buff, sector, count);
        res = result;
		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}



/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/
void sdc_reset(void)
{
    int i = 0, count = 0, count_nosd = 0;
    unsigned char buff, teste[10];
    DSTATUS stat;
    unsigned int receive;
    
    
    /*SOFTWARE RESET COMMAND*/
    do{
//        SPI_DATA_OUT = 1;
//        CHIP_SELECT = 0; 
//        dummy_clocks(10); 
        command(0X40, 0X00000000, 0X95);    //CMD0
        proceed();
        do{  
//            __delay_ms(50);
            buff = response(); 
            count++;
//            posicao_cursor_lcd(1,0);
//            escreve_inteiro_lcd(buff);
        }while(buff!=0X01 && count<10);
//        CHIP_SELECT = 1;
        count = 0;
        if(buff != 0x01)
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Sem Cartao");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("Insira o Cartao");
        }
        else
        {
           posicao_cursor_lcd(1,0);
           escreve_frase_ram_lcd("Cartao Inserido");
        }

    }while(buff!=0X01);
    
    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
    return; //CHECK FOR A STA_NODISK return
}



/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/
DSTATUS sdc_disk_initialize(void)
{
    BYTE ocr;
    DSTATUS stat;
    int i = 0, count1 = 0, count2 = 0;
    BYTE buff = 0;
    BYTE buff_i;
    int crc_number;
    __delay_ms(1);
    dummy_clocks(100); 
    

    sdc_reset(); //RESET THE SD CARD 
    command(0X48, 0x000001AA, 0x87);    //CMD8
    proceed();
    do{
        buff_i = response(); 
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(buff_i);
        if(buff_i == 1)
        {
            crc_number = buff_i<<8;
        }
        else if(buff_i==0xAA)
        {
            crc_number |= buff_i;
        }
        count1++;
    }while(crc_number != 0x1AA && count1<1000); //MATCH? From fluxogram process
    
    
    
    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
    if (crc_number == 0x1AA)
    {                      
        count1 = 0;      
        do{ 
            count2 = 0;
            buff = 0XFF;
              //ACMD41 - CRC doesnt need be correct
            /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
            
            do{    
//                command(0X41, 0X00000000, 0XFF);    //CMD55
//                buff = 0XFF;
//                /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
//                proceed();
//                do{
//                    buff = response();
//                    count2++;
//                }while((buff!=0X01) && count2<10);
                
                
                dummy_clocks(10);
                command(0X77, 0X00000000, 0X00);    //CMD55
                buff = 0XFF;
                /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
                proceed();
                do{
                    buff = response();
                    count2++;
                }while((buff!=0X01)&&(count2<10));
                count2 = 0;



                dummy_clocks(10);
                command(0X69, /*0X40000000*/1UL << 30, 0x00);        
                proceed();
                buff = response();
                count2++;
//                posicao_cursor_lcd(2,5);
//                escreve_inteiro_lcd(buff);
            }while(buff!=0X00);
            
            buff = 0xFF;
            
            
            dummy_clocks(10);
            command(0X7A, 0X00000000, 0XFF);    //CMD58;
            proceed();
            do{
                buff = response();
                ocr = (buff << 1) & 0b10000000;
                posicao_cursor_lcd(2,0);
                escreve_inteiro_lcd(ocr);
                count2++;
            }while(ocr!=0X00 && ocr != 0x01 && count2<10);
        }while(ocr != 0X00 && ocr !=0x01);
        
//        LIMPA_DISPLAY();
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(ocr);
        count1 = 0;
        //Delay before sending command
        __delay_ms(1);
        stat = 0X00;
        /*SETTING BLOCK LENGTH TO 512 Bytes*/
        if(!ocr)
        {
             
            dummy_clocks(10);
            command(0X50,0X00000200,0XFF);    //CMD16
            proceed();

            buff = 0xFF;
            do{   
                buff = response();
            }while(buff!=0X00);  

        }
        
    }
    else
    {

        count2 = 0;
        buff = 0XFF;
        dummy_clocks(1);
        command(0X69, 0X00000000, 0XFF);    //ACMD41 - CRC doesnt need be correct
        proceed();
        do{
            buff = response();
            count2++;
            if(buff==0x00)
            {
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("SD VER. 1");
            }
            __delay_ms(1);
        }while(buff!=0X00 && count2<10000);

        buff = 0xFF;
        if(count2==10)
        {
            count2 = 0;
            dummy_clocks(10);
            command(0X41, 0X00000000, 0XFF);    //ACMD41 - CRC doesnt need be correct
            proceed();
            do{
                buff = response();
                count2++;
                if(buff==0x00)
                {
                    posicao_cursor_lcd(1,0);
                    escreve_frase_ram_lcd("MMC Ver. 3");
                }

            }while(buff!=0X00 && count2<10000);   
        } 
        
        
        dummy_clocks(10);    
        command(0X50,0X00000200,0XFF);    //CMD16
        proceed();

        buff = 0xFF;
        do{   
            buff = response();
        }while(buff!=0X00); 
       
    }

    
//    __delay_ms(2000);
    return stat;
}






/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/
DRESULT sdc_disk_read(
    BYTE *p_buff,
    DWORD sector,
    BYTE count
)
{
    //O QUE FOI FEITO=================
    DRESULT res;
    unsigned char ptr=0X00, buff;
    unsigned long int start_add;
//    static unsigned char arr[512];
    int length,i, count1;
    dummy_clocks(10);
    start_add = sector*512;
    
//    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("LENDO SD");
    
    
    if(sector != 1)
    {
        dummy_clocks(10);
        command(0X52,start_add ,0X00);    //CMD18
        proceed();
        do{   
            buff = response();
        }while(buff!=0X00);
        proceed();
    }
    else
    {
        dummy_clocks(10);
        command(0X51,sector ,0X00);    //CMD18
        proceed();
        do{   
            buff = response();
        }while(buff!=0X00);
        proceed();
    }


    
    while(buff!=0xFE)
    {
        buff = response();
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(buff);
//        __delay_ms(900);
    }
//    proceed();
    length = 0;
    while ( length < 512 )               
    {
        p_buff[length] = response();
        length++;                   
    }


    length = 0;
    dummy_clocks(10);  
    
    command(0X4C,0X00000000,0X00);    //CMD12
    proceed();
    do{   
        buff = response();
    }while(buff!=0xFF);
    
    length = 0;
//    while(p_buff[length]!='\0')
//    {
//            LIMPA_DISPLAY();
//            posicao_cursor_lcd(2,10);
//            escreve_frase_ram_lcd("saindo");
//            length++;
//    }
//    *p_buff = length; // pointer to the pointer got in
	return RES_OK;  
    
    //tem mais código mas estamos testando sem por enquanto
    
    
}

/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/
DRESULT sdc_disk_write(const BYTE *p_buff, DWORD sector, BYTE count)
{
    DRESULT res;
    unsigned char buff; 
    int i=0,j, count1 = 0;
    SPI_DATA_OUT = 0;
    CHIP_SELECT = 0;
    dummy_clocks(10);    
    command(0X58, sector*512, 0X00); // CMD24
    proceed();
    do{   
        buff = response();
    }while(buff!=0X00);

    CHIP_SELECT = 0;
    SPI_DATA_OUT = 0;
    dummy_clocks(1);
    WriteSPI_(0XFE); //START TOKEN SINGLE BLOCK WRITE
      
    for(j=0;j<512;j++) //DATA BLOCK
    {
       
        

        WriteSPI_(*p_buff);
        p_buff++;

       
    }

    ReadSPI_(); // CRC 2 BYTES
    ReadSPI_();

    dummy_clocks(10);
    command(0X4D,0X00000000,0X00);    //CMD13
    proceed();
    do{ 
        buff = response();      
    }while(buff!=0X00 /*&& count<10*/);
//      
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("OK");
//    __delay_ms(4000);
	return res;
    
}

/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/

DSTATUS sdc_disk_status(BYTE pdrv)
{
    DSTATUS stat = 0;
    unsigned char buff, cont;
    
    if (pdrv != 0) return STA_NOINIT;	/* Supports only single drive */

    return FR_OK; //temos que mudar isso para que tratem os outros erros

}