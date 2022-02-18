///******************************************************************************
// * Nome do Arquivo 	: SPI.c
// *
// * Descricao       	: Implementa o controle protocolo SPI
// *
// * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
// *
// * Responsavel		: Souza, Deivide ConceiÃ§Ã£o de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "SDCard.h"
//#include "SPI.h"
#include "hardware.h"
#include "integer.h"
#include "display_lcd.h"
#include "ff.h"
#include "diskio.h"
#include "SHRC.h"
//#include "sw_uart.h"

//global variables

unsigned char *rd;
unsigned char Result;
BYTE s;
DWORD fileSize;
BYTE txt[20];
BYTE data_buffer_32[32];


// File to read================================================================= 
BYTE fileName[15] = {"check.txt"}; //USE SMALLER ARRAY SIZE /testmapp/testtext 
BYTE folder[48] = {""}; 
//==============================================================================

extern void readover(int);

BYTE response(void)
{
    
    unsigned char buff;
    CHIP_SELECT = 0;//CS low   
    buff= recebe_dado_SPI();   //read buffer (R1) should be 0x01 = idle mode   
//    CHIP_SELECT = 1;//CS high
    return buff;
}

/******************************************************************************
 * Funcao:		dummy_clocks(unsigned char n)
 * Entrada:		unsigned char n
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para que o SDCard e o 
 * Microcontrolador estejam em sincronia
 *****************************************************************************/

void dummy_clocks(unsigned char n)
{
    unsigned int i;
    __delay_ms(1);
//    LATB5 = 1;
    for(i=0;i<n;i++)
    {
        CHIP_SELECT = 1;
        escreve_dado_SPI(0XFF);
        CHIP_SELECT = 0;
    }
}


/******************************************************************************
 * Funcao:		proceed(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Envia pulsos de clock no barramento para manter o SDCard ativo
 * ou para ativar
 *****************************************************************************/
void proceed(void)
{
    CHIP_SELECT = 0; //CS Low
    escreve_dado_SPI(0xFF); // Give Time For SD_CARD To Proceed
    CHIP_SELECT = 1; //CS High
}

//extern  void check();


/******************************************************************************
 * Funcao:		command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
 * Entrada:		unsigned char CMD, unsigned long int arg, unsigned char CRC
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza o envio do comando para o SDCard
 *****************************************************************************/

void command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
{
    unsigned char argument;
    escreve_dado_SPI(0xFF);
    escreve_dado_SPI(CMD);
    argument = arg>>24;
    escreve_dado_SPI(argument);
    argument = arg>>16;
    escreve_dado_SPI(argument);
    argument = arg>>8;
    escreve_dado_SPI(argument);
    argument = arg;
    escreve_dado_SPI(argument);
    escreve_dado_SPI(CRC);
}



/******************************************************************************
 * Funcao:		SDCard(void) 
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a montagem do sistema de arquivos, cria o arquivo, escreve no arquivo
 * e fecha o arquivo
 *****************************************************************************/
FATFS fs;
WORD br;
void SDCard(void) 
{
    T0CONbits.TMR0ON = 0;
//    inicializa_SPI(0,3,1);
    FRESULT FResult;
    FIL fil;
    
    
    
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Attempting to");
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd("mount file sys");
//    SPI_DATA_OUT = 0;
//    __delay_ms(2000);
//    SPI_DATA_OUT = 1;
//    __delay_ms(2000);
    LIMPA_DISPLAY();
    
    /*READ FUNCTION=============================================================*/
    proceed();
    
    
    if((FResult = f_mount(1, &fs)) == FR_OK)
    {
        if((FResult = f_write(&fil,"Hello World", 11,  br)) == FR_OK) 
        {
            f_close(&fil);
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("File wrote!");
        }
        else
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("ERROR open file");
        }
        
        
    }
    else
    {
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("ERROR mount");
    }
    
    
    
    
    
    
    
    
//	if((FResult = pf_mount(&fs)) == FR_OK )
//	{   
//        posicao_cursor_lcd(2,0);
//        escreve_frase_ram_lcd("montado");
//        if(FResult == FR_OK)
//        {
//            posicao_cursor_lcd(2,0);
//            escreve_frase_ram_lcd("tentando escrever");
//            if( (FResult = pf_write("SD_Card", 7, &br)) == FR_OK )
//            {
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("escrito");
//                while(1);
//    //            putsUART("\r\nSuccessfully written");
//            } 
//            else
//            {
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("ERRO!");
////                putsUART("\r\nError trying to write to file; ");
////                WriteUART(FResult + 0x30);
////                putsUART(".\r\n");
//
//                while( 1 );
//            }
//            
//        }
        
//    }
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("syst initialized");
//        
//		//open file=======================================
//		FResult = pf_open(fileName/*fileName*/);
//        posicao_cursor_lcd(1,0);
//        escreve_inteiro_lcd(FResult);
//        __delay_ms(2000);
//        //=================================================
//		if( FResult == FR_OK )
//		{
//            FResult = pf_read(data_buffer_32, 31, &br);
//           
//            __delay_ms(1000);
//			// Read 31 bytes from the file                        
//            if( (FResult) == FR_OK )
//			{
//                posicao_cursor_lcd(1,0);
//                escreve_frase_ram_lcd("Lendo");
//                __delay_ms(2000);
//                if(FResult = pf_write("FELIPE", 7, &br) == FR_OK)
//                {
//                    posicao_cursor_lcd(2,0);
//                    escreve_frase_ram_lcd("escrito");
//                }
//			} 
//			else
//			{
//                LIMPA_DISPLAY();
//                posicao_cursor_lcd(1,0);
//                escreve_frase_ram_lcd("SDCard error");
//				while( 1 );
//			}
//            LIMPA_DISPLAY();
//            
//            //Starts to read the file for writing
//            posicao_cursor_lcd(1,0);
//            escreve_frase_ram_lcd("lendo arquivo");
//			// read file and print it until it ends
//			do
//			{
//                readover(1); //Set Flag=1 in diskio.c before reading Card Data
//                posicao_cursor_lcd(1,0);
//                escreve_frase_ram_lcd("To aqui");
//				// Read 31 bytes from the file
//				if( (FResult = pf_read(data_buffer_32, 31, &br)) == FR_OK )
//				{
//					// putsUSART needs a C-string (NULL terminated)
//                    posicao_cursor_lcd(2,0);
//                    escreve_frase_ram_lcd("Sistema OK");
//					data_buffer_32[br] = 0;
//                    break;
//					//putsUART(data_buffer_32);
//				}
//				else
//				{
//                    //write on the screen if it got an error
//                    LIMPA_DISPLAY();
//                    posicao_cursor_lcd(1,0);
//                    escreve_frase_ram_lcd("Erro tentando");
//                    posicao_cursor_lcd(2,0);
//                    escreve_frase_ram_lcd("ler o arquivo");
//					
//					while( 1 );
//				}
//                posicao_cursor_lcd(1,0);
//                escreve_frase_ram_lcd("Preso");
//			} while( br == 31 );	// if the pf_Read reads less then 31 bytes the file has ended
//		}
//		else 
//		{
//            LIMPA_DISPLAY();
//            posicao_cursor_lcd(1,0);
//            escreve_frase_ram_lcd("Erro tentando");
//            posicao_cursor_lcd(2,0);
//            escreve_frase_ram_lcd("abrir o arquivo");
//			while( 1 );
//		}
//	}
//	else
//	{
//    //Write a error message if the file wasnt oppened 
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("ERRO!");
//		while( 1 );
//        T0CONbits.TMR0ON = 1;
//	}
//
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Não to Preso");
//    
//  	// do a directory listing and list all files on the SD-card ========
//
//    
//    
////============================================================================
//	//scan_files(folder);????????????????? function depreciated ?
//		
//	// unmount drive
////	putsUART("\r\nAttempting to UNmount file system.\r\n");
//	
//	//FResult = pf_mount(NULL); ??????????????????????????????
//	
////============================================================================  
//    
//	if( FResult != FR_OK )
//	{
//    //write on the screen it wanst possible to create   
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("Erro filesyst");
////		putsUART("\r\nError trying to UNmount filesystem; ");
////		WriteUART(FResult + 0x30);
////		putsUART(".\r\n");
//		while( 1 );
//	}	
//
////	putsUART("\rFile system succesfully unmounted.\r\n");
//	while(1){
//        posicao_cursor_lcd(1,0);
//        escreve_frase_ram_lcd("to Preso aq");
//    }
/*WRITE FUNCTION*/
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
    unsigned char buff;
    /*SOFTWARE RESET COMMAND*/
    do{
        dummy_clocks(8);    
        command(0X40, 0X00000000, 0X95);    //CMD0
        proceed();
        do{  
            buff = response(); 
            count++;
        }while((buff!=0X01) && (count<10));
        count = 0;
        count_nosd++;
        if(count_nosd<=3)
        {
            posicao_cursor_lcd(1,0);
            escreve_frase_ram_lcd("SEM SDCARD!!");
            posicao_cursor_lcd(2,0);
            escreve_frase_ram_lcd("Insira o Cartao");
            count_nosd = 0;
        }
    }while(buff!=0X01);
    
    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//      putsUART("\n\rSD Card Inserted\n\r");
    __delay_ms(2000);
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
    DSTATUS stat;
    int i = 0, count1 = 0, count2 = 0;
    unsigned char buff;
    
    sdc_reset(); //RESET THE SD CARD 
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("Cartao Aceito");
    __delay_ms(500);
    
    dummy_clocks(8);    
    command(0X41, 0X00000000, 0XFF);    //CMD1
    proceed();
    buff =0XFF; //enche o buffer para receber a resposta 0x01
    do{
        buff = response();
    }while(buff!=0x01);
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Iniciando");
    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
    __delay_ms(1000);
    proceed();
    
    
    
    if (buff == 0x01)
    {                    
        //SEND CMD55 + ACMD41     
        __delay_ms(1);
        count1 = 0;        
        do{
            count2 = 0;
            dummy_clocks(8);
            command(0X77, 0X00000000, 0X95);    //CMD55
            buff = 0XFF;
            /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
            proceed();
            do{
                buff = response();
                count2++;
            }while((buff!=0X01)&&(count2<10));
            
//            __delay_ms(1);
            count2 = 0;
            
            dummy_clocks(8);
            command(0X69,0X40000000,0X95);    //CMD41
            buff = 0XFF;
            /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
//            proceed();
            do{
                buff = response();
                count2++;
            }while((buff!=0X00) && count2<10);
        }while(buff != 0X00);
        count1 = 0;
        //Delay before sending command
        __delay_ms(1);
        stat = 0X00;

        __delay_ms(2000);     
    }
    else if(buff == 0x05)
    {
        stat = STA_NOINIT;
        //DISPLAY MESSAGE=================================================        
        LIMPA_DISPLAY();
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd("Error!!!");
        //DISPLAY MESSAGE=================================================
        __delay_ms(3000);
    }
    __delay_ms(1000);
    
    
/*SETTING BLOCK LENGTH TO 512 Bytes*/
    dummy_clocks(8);    
    command(0X50,0X00000200,0XFF);    //CMD16
    proceed();
    do{   
        buff = response();
    }while(buff!=0X00); //OPENLAB has shown that it needs to be 0x00 as response
    
//=============================================    
    LIMPA_DISPLAY();
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("Bloco de 512");
    posicao_cursor_lcd(2,0);
    escreve_frase_ram_lcd("Bytes criado!");
//=============================================    
    
    __delay_ms(2000);
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
    static unsigned char arr[512];
    int length,i;
    
    start_add = (sector*512);
    
    __delay_ms(1000);
    
    dummy_clocks(8); 
    command(0X52,start_add,0X00);    //CMD18
    proceed();
    do{   
        buff = response();
    }while(buff!=0XFF);
    __delay_ms(1000);  
    proceed();
    do{
       buff = response();
    }while(buff!=0xFE);
    length = 0;
    while ( length < count )               
    {
        arr[length] = response();
        length++;                   
    }
    __delay_ms(1000);
    dummy_clocks(8);    
    command(0X4C,0X00000000,0X00);    //CMD12
    proceed();
    do{   
        buff = response();
    }while(buff!=0xFF);
    length = 0;
    while(arr[length]!='\0')
    {
              //WriteUART(arr[length]);
              length++;
    }
    *p_buff = length; // pointer to the pointer got in
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
    int i=0,j;
    
//    Delay_s(1);
    
    dummy_clocks(8);    
    command(0X58, sector*512, 0X00); // CMD24
    proceed();
    do{   
        buff = response();
    }while(buff!=0X00);

    __delay_ms(2000);
//      Delay_s(2);

    dummy_clocks(8);

    escreve_dado_SPI(0XFE); //START TOKEN SINGLE BLOCK WRITE
      
    for(j=0;j<512;j++) //DATA BLOCK
    {
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd(*p_buff);
        if(*p_buff!='\0')
        {
            escreve_dado_SPI(*p_buff);
            p_buff++;
        }
        else
        escreve_dado_SPI(0x00);
    }
    escreve_dado_SPI(0XFF); // CRC 2 BYTES
    escreve_dado_SPI(0XFF);

    proceed();

    __delay_ms(1000);
//      Delay_s(1);
    dummy_clocks(8);    

    command(0X4D,0X00000000,0XFF);    //CMD13
    proceed();
    do{   
        buff = response();
    }while(buff!=0X00);
      
	return RES_OK;
    
}

/******************************************************************************
 * Funcao:		void interrupt isr(void)
 * Entrada:		Nenhuma (void)
 * Saï¿½da:		Nenhuma (void)
 * Descriï¿½ï¿½o:	Implementa a rotina de interrupcao
 *****************************************************************************/

DSTATUS sdc_disk_status(void)
{
    DSTATUS stat;
    unsigned char buff, cont;
    dummy_clocks(8);
    command(0x40, 0x00000000, 0x95);
    proceed;
    do{
        buff= response();
    }while(cont<10);
    if(buff==0x00)
    {
        return stat;
    }
    else if(0x01)
    {
        return stat;
    }
    else
    {
        return STA_NOINIT; //temos que mudar isso para que tratem os outros erros
    }
}