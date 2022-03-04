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
#include "tff.h"
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
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
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
        res = result;
		// translate the reslut code here
		return res;

	case DEV_MMC :
		// translate the arguments here
		result = sdc_disk_read(buff, sector, count);
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
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa a rotina de interrupcao
 *****************************************************************************/
void disk_reset(void)
{
   
}



/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa a rotina de interrupcao
 *****************************************************************************/
DSTATUS sdc_disk_initialize(void)
{
    
    int i = 0, count = 0, count_nosd = 0;
    unsigned char buff, teste[10];
    DSTATUS stat;
    unsigned int receive;
    /*SOFTWARE RESET COMMAND*/
    
    __delay_ms(10);
//    dummy_clocks(10); 
    
    buff = 0;
    SPI_DATA_OUT = 1;
    CHIP_SELECT = 0; 
//    dummy_clocks(1); 
    WriteSPI_("0xFF");
    command(GO_IDLE_STATE, 0X00000000, 0X95);    //CMD0
    dummy_clocks(8);
//    proceed();
    CHIP_SELECT = 1;
    do{
        buff = response(); 
        count++;
        if(buff != 0x01)
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("Sem Cartao");
            posicao_cursor_lcd(1, 11);
            escreve_inteiro_lcd(buff);
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("Ins. Cartao");
            posicao_cursor_lcd(2, 11);
            escreve_inteiro_lcd(count);
            count++;
        }
        else
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("                ");        
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("                ");
        }
    }while((buff!=0X01) && count<100);
    count = 0;
          
        // verificando a vers�o do cart�o
        buff = 0xFF;        
        CHIP_SELECT = 0; 
        SPI_DATA_OUT = 1;
        dummy_clocks(10);
        command(CHECK_VERSION, 0x000001AA, 0x87);    //CMD8
        proceed();
        CHIP_SELECT = 1;        
        do{  
            buff = response(); 
            posicao_cursor_lcd(1,0);
            escreve_inteiro_lcd(buff);
        }while((buff!=0X01));
        __delay_ms(10);
        
         // verificando o status do cart�o, precisa estar em 0x00 idle state
        
        buff = 0xFF;
        CHIP_SELECT = 0; 
        SPI_DATA_OUT = 1;
        dummy_clocks(10);
        command(COMMAND_55, 0x00000000, 0x65);    //CMD55
        proceed();
        CHIP_SELECT = 1; 
        
        __delay_ms(10);
        
        buff = 0xFF;
        count = 0;

//        __delay_ms(10);
         
        CHIP_SELECT = 0; 
        SPI_DATA_OUT = 1;
        dummy_clocks(10);
        command(COMMAND_41, 0x00000000, 0x00);    //CMD41
        proceed();
        CHIP_SELECT = 1; 

        __delay_ms(10);
        CHIP_SELECT = 0; 
        SPI_DATA_OUT = 1;
        dummy_clocks(10);
        command(SEND_OP_COND, 0x00000000, 0x00);    //CMD1
        proceed();
        CHIP_SELECT = 1; 

        do
        {
            buff = response();
            count++;

            if(buff != 0x00) // mostrando erro
            {
                posicao_cursor_lcd(2,0);
                escreve_frase_ram_lcd("Erro Idle St    ");
                escreve_inteiro_lcd(buff);
                posicao_cursor_lcd(2,13);
                escreve_inteiro_lcd(count);
            }

        }while(buff != 0x00);

        __delay_ms(10);
       
        // grava��o / leitura - enviando comando 16 - setando o bloco para 512 e
        // esperando 0x00 como resposta
        
        __delay_ms(10);

         CHIP_SELECT = 0; 
         SPI_DATA_OUT = 1;
         dummy_clocks(10);
         command(SET_BLOCK_SIZE, BLCK_SIZE, 0xFF);    //CMD1
         proceed();
         CHIP_SELECT = 1; 

         count = 0;
         do
         {
             buff = response();
             count++;
             if(buff != 0x00)
             {
                posicao_cursor_lcd(2,0);
                escreve_frase_ram_lcd("Set. 512b       ");
                posicao_cursor_lcd(2,0);
                escreve_inteiro_lcd(buff);
                posicao_cursor_lcd(2,10);
                escreve_inteiro_lcd(count);
             }
         }while(buff != 0x00);
     posicao_cursor_lcd(2,10);
     escreve_inteiro_lcd(stat);
        
    return stat; //CHECK FOR A STA_NODISK return
    
    
    
}


/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa a rotina de interrupcao
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
    int length,i, count1, offset = 0; // atribuindo 0 a offset
//  
    // assumimos que o offset = 0? 
    
//    proceed();
////    while(response() == 0xFF)
////    {
////        posicao_cursor_lcd(1,0);
////        escreve_frase_ram_lcd("EROOOOOO");
////        
////    }
////    
    if(offset == 0)
    {
        start_add = ((sector*512));
        __delay_ms(1);
        CHIP_SELECT = 0;
        SPI_DATA_OUT = 1;
        
        dummy_clocks(10);
        command(GO_READ_MBLK,start_add,0X00);    //CMD18
        
        CHIP_SELECT = 1;
    //    
//        __delay_ms(1); // por conta pr�pria
        proceed();
        do
        {   
            buff = response();
            if(buff != 0x00)
            {
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("buffer leit.   ");
                posicao_cursor_lcd(1,13);
                escreve_inteiro_lcd(buff);
            }

        }while(buff!=0X00);

        proceed();
        do
        {
            buff = response();
            posicao_cursor_lcd(2,13);
            escreve_inteiro_lcd(buff);
            if(buff != 0xFE)
            {
                posicao_cursor_lcd(2,0);
                escreve_frase_ram_lcd("Aguard 0XFE     ");
                posicao_cursor_lcd(2,13);
                escreve_inteiro_lcd(buff);
            }
           
        }while(buff!=0xFE);
        length = 0;
        __delay_ms(1);// era 200ms
        while (length < 512)       // estava 512 alterei para count        
        {
            p_buff[length] = response(); // alterei de p_buff para arr
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd(p_buff[length]);
            length++;                   
        }
        CHIP_SELECT = 0;
        length = 0;    
        dummy_clocks(10);    
        command(0X4C,0X00000000,0X00);    //CMD12
        CHIP_SELECT = 1;

        proceed();
        do{   //aguardando a resposta 0xFF
            buff = response();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("aguard 0xFF       ");
            posicao_cursor_lcd(1,13);
            escreve_inteiro_lcd(buff);


        }while(buff!=0xFF);
    //    
        __delay_ms(500); //era 1000ms
        length = 0;
        while(p_buff[length]!='\0') 
        {
//                LIMPA_DISPLAY();
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("\0 ->         ");
                posicao_cursor_lcd(1,14);
                escreve_inteiro_lcd(p_buff[length]);
                length++;
        }
        *p_buff = length; // pointer to the pointer got in
                          // n�o alterei *p_buff para *rd

         return RES_OK;
    }
    else
    {
        start_add = (sector*512);

        length = 0;
        while(length<512)           //CLEAR ARRAY
        {
            p_buff[length] = 0;                
            length++;
        }
        __delay_ms(1000);
        dummy_clocks(10);    
        //SPI_cmd(sd_cmd17);
        command(0X51,start_add,0X00);
        proceed();
        do{   
            buff = response();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("aguard 0x00    ");
            posicao_cursor_lcd(1,13);
            escreve_inteiro_lcd(buff);
          }while(buff!=0x00);
          do{   
            buff = response();
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("aguard 0xFE    ");
            posicao_cursor_lcd(1,13);
            escreve_inteiro_lcd(buff);
          }while(buff!=0xFE);

              length = 0;
                while ( length < 512 )               
            {
                while(offset--)             //RUN DOWN TILL THE OFFSET VALUE
                {
                p_buff[length] = response();
                length++;
                }
                while(count--)              //RUN DOWN TILL THE COUNT VALUE
                {
                *p_buff = response();
                p_buff[length] = *p_buff;
                p_buff++;
                length++;
                }
                while(length<512)           //FOR THE TRAILING BYTES
                {
                    p_buff[length] = response();                
                    length++;
                }
            }
            __delay_ms(1000);
//            dummy_clocks(10);    
            //SPI_cmd(sd_cmd12);
            command(0X4C,0X00000000,0X00); //COMMAND12 MANDATORY
            proceed();
            do{   
                buff = response();
                posicao_cursor_lcd(1,0);
                escreve_frase_ram_lcd("aguard 0xFF       ");
                posicao_cursor_lcd(1,13);
                escreve_inteiro_lcd(buff);
              }while(buff!=0xFF);
              
              length = 0;



              
        __delay_ms(2000);
         return RES_OK; 
    }
        __delay_ms(2000);
    
    
    
}

/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Sa�daa:		Nenhuma (void)
 * Descricao:	Implementa a rotina de interrupcao
 *****************************************************************************/
DRESULT sdc_disk_write(const BYTE *p_buff, DWORD sector, BYTE count)
{
    DRESULT res;
    unsigned char buff; 
    int i=0,j, count1 = 0;
    
    SPI_DATA_OUT = 1;
    CHIP_SELECT = 0;
    dummy_clocks(10);
    if(count < 512)
    {
        command(0X58, 0X00000000, 0X00); // CMD24 - single block
        CHIP_SELECT = 1;
        do
        {
            buff = response();        
            // aguardando a resposta
        }
        while(buff != 0x00);
        
        __delay_ms(1); // dando uma pausa
        SPI_DATA_OUT = 1;
        CHIP_SELECT = 0;
        WriteSPI_(0XFE); //START TOKEN SINGLE BLOCK WRITE
        CHIP_SELECT = 1; 
        
        
        __delay_ms(1); // dando uma pausa
        // verificar se � gravado o bloco todo ou precisamos mandar cada byte separado
        SPI_DATA_OUT = 1;
        CHIP_SELECT = 0;
        WriteSPI_(*p_buff);
        p_buff++;
        CHIP_SELECT = 1; 
    }
    else
    {
        command(0X59, 0X00000000, 0X00); // CMD25 - multi block
        CHIP_SELECT = 1;
        
        do
        {
            buff = response();        
            // aguardando a resposta
        }
        while(buff != 0x00);
        

        
        for(char i = 0; i < count / 512; i++)
        {
            
            SPI_DATA_OUT = 1;
            CHIP_SELECT = 0;    
            dummy_clocks(2); // mandando dummy clocks para enviar o start token
            WriteSPI_(0XFC); //START TOKEN Multi BLOCK WRITE
            CHIP_SELECT = 1; 
            
            SPI_DATA_OUT = 1;
            CHIP_SELECT = 0; 
            WriteSPI_(*p_buff);
            p_buff++;
            CHIP_SELECT = 1; 
            
            SPI_DATA_OUT = 1;
            CHIP_SELECT = 0;   
            WriteSPI_(0XFD); //STOP TOKEN 
            CHIP_SELECT = 1; 
            
        }
                
    }

    __delay_ms(1); // dando uma pausa
    CHIP_SELECT = 0;
    SPI_DATA_OUT = 1;
    dummy_clocks(2);
    command(0X4D,0X00000000,0XFF);    //CMD13 // terminando o bloco
    CHIP_SELECT = 1;
    
    
    do{ 
        buff = response(); 
//        count++;
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(buff);
//        __delay_us(30);
        
    }while(buff!=0X00 /*&& count<10*/);
//      
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd(res);
    __delay_ms(2000);
    
    
    
	return res;
    
}

/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa a rotina de interrupcao
 *****************************************************************************/

DSTATUS sdc_disk_status(BYTE pdrv)
{
    DSTATUS stat = 0;
    unsigned char buff, cont;
    
    if (pdrv != 0) return STA_NOINIT;	/* Supports only single drive */

    return FR_OK; //temos que mudar isso para que tratem os outros erros

}

/////   ////////////////// C�digo GIT Adaptado
//// https://github.com/etiq/OpenLab-PIC18F4550-SDCard-Examples/blob/master/OpenLab_SDC_PetitFS.X/diskio.c


//
//
//int flag;
//
//void readover(int a)    /*Set Global Variable, Flag when reading SD Card content*/
//{
//    flag = a;
//}
//
///*-----------------------------------------------------------------------*/
///* Initialize Disk Drive                                                 */
///*-----------------------------------------------------------------------*/
//
//    void reset()
//    {
//    int i = 0, count = 0;
//    unsigned char buff;
//    /*SOFTWARE RESET COMMAND*/
//    do{
//    dummy_clocks(10);    
//    command(0X40, 0X00000000, 0X95);    //CMD0
//    proceed();
//    do{   
//        buff = response();
//        count++;
//      }while((buff!=0X01) && (count<10) );
//      count = 0;
//    }while(buff!=0X01);
//
//    return; //CHECK FOR A STA_NODISK return
//}
//
////DSTATUS disk_initialize (void)
//{
//	DSTATUS stat;
//    int i = 0, count1 = 0, count2 = 0;
//    unsigned char buff;
//    reset(); //RESET THE SD CARD
//    __delay_ms(500);
//    dummy_clocks(10);    
//    command(0X41, 0X00000000, 0XFF);    //CMD1
//    proceed();
//    do{
//        buff = response();
//      }while(buff!=0x01);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//      __delay_ms(1000);
//      proceed();
//if (buff == 0x01)
//{                    
//    //SEND CMD55 + ACMD41     
//    __delay_ms(1);
//    count1 = 0;        
//      do{
//        count2 = 0;
//        dummy_clocks(10);
//        command(0X77, 0X00000000, 0X95);    //CMD55
//        buff = 0XFF;
//        /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
//        do{
//        buff = response();
//        count2++;
//        }while((buff!=0X01)&&(count2<10));
//        
//        __delay_ms(1);
//        count2 = 0;
//        dummy_clocks(10);
//
//        command(0X69,0X40000000,0X95);    //CMD41
//        buff = 0XFF;
//        /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
//        proceed();
//        do{
//        buff = response();
//        count2++;
//        }while((buff!=0X00)&&(count2<10));
//     }while(buff != 0X00);
//
//      count1 = 0;
//  //Delay before sending command
//      __delay_ms(1);
//      stat = 0X00;
//      }
//
//      else if(buff == 0x05)
//      {
//        stat = STA_NOINIT;
// 
//      }
//      __delay_ms(1);
//      
///*SETTING BLOCK LENGTH TO 512 Bytes*/
//    dummy_clocks(10);    
//    command(0X50,0X00000200,0XFF);    //CMD16
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//	return stat;
//}


/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

//DRESULT disk_readp (
//	BYTE* rd,		/* Pointer to the destination object */
//	DWORD sector,	/* Sector number (LBA) */
//	UINT offset,	/* Offset in the sector */
//	UINT count		/* Byte count (bit15:destination) */
//)
//  {
//	DRESULT res;
//    unsigned char ptr=0X00,buff;
//    unsigned long int start_add;
//    static unsigned char arr[512];
//    int length,i;
//    /*Read Sector*/
//    if(offset == 0){
//    start_add = ((sector*512));
//    
//    __delay_ms(1000);
//    dummy_clocks(10);    
//    command(0X52,start_add,0X00);    //CMD18
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//      __delay_ms(1000);
//      proceed();
//      do{
//        buff = response();
//      }while(buff!=0xFE);
//      length = 0;
//      while ( length < count )               
//        {
//            arr[length] = response();
//            length++;                   
//        }
//      
//    __delay_ms(1000);
//    dummy_clocks(10);    
//    command(0X4C,0X00000000,0X00);    //CMD12
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0xFF);
// 
//      length = 0;
//      
//      while(arr[length]!='\0')
//      {
//          //WriteUART(arr[length]);
//          length++;
//      }
//      
//      *rd = length;
//	return RES_OK;
//  }
//    else
//    {
//    start_add = (sector*512);
//    
//    length = 0;
//    while(length<512)           //CLEAR ARRAY
//    {
//        arr[length] = 0;                
//        length++;
//    }
//
//    __delay_ms(1000);
//    dummy_clocks(10);    
//    //SPI_cmd(sd_cmd17);
//    command(0X51,start_add,0X00);
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//      do{   
//        buff = response();
//      }while(buff!=0xFE);
//
//          length = 0;
//            while ( length < 512 )               
//            {
//                while(offset--)             //RUN DOWN TILL THE OFFSET VALUE
//                {
//                arr[length] = response();
//                length++;
//                }
//                while(count--)              //RUN DOWN TILL THE COUNT VALUE
//                {
//                *rd = response();
//                arr[length] = *rd;
//                rd++;
//                length++;
//                }
//                while(length<512)           //FOR THE TRAILING BYTES
//                {
//                    arr[length] = response();                
//                    length++;
//                }
//            }
//            __delay_ms(1000);
////            dummy_clocks(10);    
//            //SPI_cmd(sd_cmd12);
//            command(0X4C,0X00000000,0X00); //COMMAND12 MANDATORY
//            proceed();
//            do{   
//                buff = response();
//              }while(buff!=0xFF);
//              
//              length = 0;
//
//              if(flag == 1){
//              while(arr[length]!='\0')
//              {
////                WriteUART(arr[length]);
//                length++;
//              }
//              }
//              else
////                putsUART("..");
//              
//      __delay_ms(2000);
//
//return RES_OK;
//}

//}
/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

//DRESULT disk_writep (
//    const BYTE* wr,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
//	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
//)
//{
//	DRESULT res;
//    
//    unsigned char buff; 
//    int i=0,j;
//    
//    __delay_ms(1000);
//    
//    dummy_clocks(10);    
//    command(0X58, sc*512, 0X00); // CMD24
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//
//      __delay_ms(2000);
//
//      dummy_clocks(10);
//
//      WriteSPI_(0XFE); //START TOKEN
//      
//      for(j=0;j<512;j++) //DATA BLOCK
//      {
//          if(*wr!='\0')
//          {
//             WriteSPI_(*wr);
//              wr++;
//          }
//          else
//          WriteSPI_(0x00);
//      }
//      WriteSPI_(0XFF); // CRC 2 BYTES
//      WriteSPI_(0XFF);
//
//      proceed();
//
//      __delay_ms(1);
//      dummy_clocks(10);    
//
//      command(0X4D,0X00000000,0X00);    //CMD13
//      proceed();
//      do{   
//        buff = response();
//        }while(buff!=0x00);
//      
//	return RES_OK;
//}