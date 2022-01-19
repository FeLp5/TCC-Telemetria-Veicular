///******************************************************************************
// * Nome do Arquivo 	: SPI.c
// *
// * Descricao       	: Implementa o controle protocolo SPI
// *
// * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceição de
//			  Silva, Felipe Alves da
//			  Souza, Ricardo de
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/
//
///******************************************************************************
//* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
//******************************************************************************/
//
//#include "hardware.h"
////#include "biblioteca/display_lcd.h"
////#include "SHRC.h"
//#include "SPI.h"
//#include "SDCard.h"
///*****************************************************************************/
//
//
//
//
//
//
//
//
//
//
///******************************************************************************
//* Variaveis Globais
//******************************************************************************/
//
///*****************************************************************************/
//
//
///******************************************************************************
//* Prototipos das funções
//******************************************************************************/
//
//
///*****************************************************************************/
// 
//
///******************************************************************************
// * Funcao:		void main(void)
// * Entrada:		config_SPI SPI, essa estrutura cont�m o modo de transmiss�o e o identificador se � escravo ou n�o
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a parametriza��o do m�dulo SPI
// *****************************************************************************/
//void init_SDcard()
//{
//    unsigned char versao = 1;
//    envia_CMD0();
//    envia_CMD1();
//    do
//    {
//        versao = envia_CMD8;
//    }while(versao); 
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd("Cart�o inserido");
//    __delay_ms(1000);
//}
//
//
///******************************************************************************
// * Funcao:		void escreve_no_SD()
// * Entrada:		config_SPI SPI, essa estrutura cont�m o modo de transmiss�o e o identificador se � escravo ou n�o
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a escrita no SDC
// *****************************************************************************/
//
////void escreve_no_SD()
////{
////    
////}
//
///******************************************************************************
// * Funcao:		void command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
// * Entrada:		unsigned char CMD, unsigned long int arg, unsigned char CRC
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a parametriza��o do m�dulo SPI
// *****************************************************************************/
//
//void comando(unsigned char CMD, unsigned long int arg, unsigned char CRC)
//{
//    unsigned char argument = arg>>24;
//    transmite_dado_SPI(0xFF);
//    transmite_dado_SPI(CMD);
//    transmite_dado_SPI(argument);
//    argument = arg>>16;
//    transmite_dado_SPI(argument);
//    argument = arg>>8;
//    transmite_dado_SPI(argument);
//    argument = arg;
//    transmite_dado_SPI(argument);
//    transmite_dado_SPI(CRC);
//}
//
///******************************************************************************
// * Funcao:		clock_simulado(unsighed char n)
// * Entrada:		unsigned char n (n�mero de execu��esw
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza simula��es de clock para manter o SDCard em sincronismo
// *****************************************************************************/
//
//void clock_simulado(unsigned char n)
//{
//    unsigned int i;
//    for(i=0;i<n;i++)
//    {
//        chip_select = 1;
//        transmite_dado_SPI(0XFF);
//        chip_select = 0;
//    }
//}
///******************************************************************************
// * Funcao:		void prossiga(void)
// * Entrada:		Nenhuma (void)
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza uma transmiss�o de 8 clocks no barramento 
// * para manter a comunica��o
// *****************************************************************************/
//void prossiga(void)
//{
//    chip_select = 0; //CS Low
//    transmite_dado_SPI(0XFF); // Give Time For SD_CARD To Proceed
//    chip_select = 1; //CS High
//}
//
//
///******************************************************************************
// * Funcao:		unsigned char resposta(void)
// * Entrada:		Nenhuma (void)
// * Saida:		unsigned char 
// * Descricao:	Realiza a leitura da resposta do SDCard
// *****************************************************************************/
//unsigned char resposta(void)
//{
//    unsigned char buff;
//    chip_select = 0;//CS low   
//    buff= recebe_dado_SPI();   //read buffer (R1) should be 0x01 = idle mode   
//    chip_select = 1;//CS high
//    return buff;
//}
//
///******************************************************************************
// * Funcao:		void send_CMD0(void)
// * Entrada:		Nenhuma (void)
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza simula��es de clock para manter o SDCard em sincronismo
// *****************************************************************************/
//void envia_CMD0(void)
//{
//    unsigned char cont; 
//    unsigned char buff;
//    while(buff != 0x01)
//    { 
//        clock_simulado(8);    // Initial Clock Pulse
//        comando(GO_IDLE_STATE, 0X00000000, 0X95);
//        prossiga();    // Generate Bus Clock
//        while((buff != 0x01) && (cont<10))
//        {
//            buff = resposta();
//            cont++;
//        }
//        cont = 0;
//    }
//}
//
//// /******************************************************************************
////  * Funcao:		void send_CMD1(void)
////  * Entrada:		Nenhuma (void)
////  * Saida:		Nenhuma (void)
////  * Descricao:	Realiza a transmiss�od o cimando CMD1
////  *****************************************************************************/
//// void envia_CMD41(void)
//// {
////     unsigned char cont; 
////     unsigned char buff;
////     while(buff != 0x00)
////     { 
////         clock_simulado(8);    // Initial Clock Pulse
////         comando(SEND_OP_COND, 0X00000000, 0X00);
////         prossiga();    // Generate Bus Clock
////         while((buff != 0x00) && (cont<10))
////         {
////             buff = resposta();
////             cont++;
////         }
////         cont = 0;
////     }   
//// }
//
///******************************************************************************
// * Funcao:		void send_CMD1(void)
// * Entrada:		Nenhuma (void)
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a transmiss�od o cimando CMD1
// *****************************************************************************/
//void envia_CMD1(void)
//{
//    unsigned char cont; 
//    unsigned char buff;
//    while(buff != 0x00)
//    { 
//        clock_simulado(8);    // Initial Clock Pulse
//        comando(SEND_OP_COND, 0X00000000, 0X00);
//        prossiga();    // Generate Bus Clock
//        while((buff != 0x00) && (cont<10))
//        {
//            buff = resposta();
//            cont++;
//        }
//        cont = 0;
//    }   
//}
//
//
//
//
//
///******************************************************************************
// * Funcao:		void send_CMD1(void)
// * Entrada:		Nenhuma (void)
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a transmiss�od o cimando CMD1
// *****************************************************************************/
//
//unsigned char envia_CMD8(void)
//{
//    unsigned char cont; 
//    unsigned char buff;
//    while(buff != 0x01 || buff != 0x05)
//    { 
//        clock_simulado(8);    // Initial Clock Pulse
//        comando(CHECK_VERSION, 0x000001AA, 0x87);
//        prossiga();    // Generate Bus Clock
//        while((buff != 0x01 || buff != 0x05) && (cont<10))
//        {
//            buff = resposta();
//            cont++;
//        }
//        cont = 0;
//    }   
//    
//    if(buff == 0x01)
//    {
////        posicao_cursor_lcd(1,0);
////        escreve_frase_ram_lcd("Versao 2");
//        return (0);
//    }
//    else if(buff == 0x05)
//    {
////        posicao_cursor_lcd(1,0);
////        escreve_frase_ram_lcd("Versao 1 ou MMC");
//        return (1);
//    }
//}
//
///******************************************************************************
// * Funcao:		void send_CMD1(void)
// * Entrada:		Nenhuma (void)
// * Saida:		Nenhuma (void)
// * Descricao:	Realiza a transmiss�od o cimando CMD16
// *****************************************************************************/
//void envia_CMD16(void)
//{
//    unsigned char cont; 
//    unsigned char buff;
//    while(buff != 0x00)
//    { 
//        clock_simulado(8);    // Initial Clock Pulse
//        comando(SET_BLOCK_SIZE, BLCK_SIZE, 0xFF);
//        prossiga();    // Generate Bus Clock
//        while((buff != 0x00) && (cont<10))
//        {
//            buff = resposta();
//            cont++;
//        }
//        cont = 0;
//    }   
//}









/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "SDCard.h"
#include "SPI.h"
#include "hardware.h"
#include "../integer.h"
#include "display/display_lcd.h"
//#include "sw_uart.h"

BYTE response(void)
{
    
    unsigned char buff;
    chip_select = 1;//CS low   
    buff= recebe_dado_SPI();   //read buffer (R1) should be 0x01 = idle mode   
    chip_select = 0;//CS high
    return buff;
}


void dummy_clocks(unsigned char n)
{
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("AAAAAAAAAA");
    unsigned int i;
    for(i=0;i<n;i++)
    {
        chip_select = 0;
        WriteSPI_(0XFF);
        chip_select = 1;
    }
}


void proceed(void)
{
    chip_select = 1; //CS Low
    WriteSPI_(0xFF); // Give Time For SD_CARD To Proceed
    chip_select = 0; //CS High
}

//extern  void check();

void command(unsigned char CMD, unsigned long int arg, unsigned char CRC)
{
    posicao_cursor_lcd(1,0);
    escreve_frase_ram_lcd("OOOOOOOOO");
    unsigned char argument = arg>>24;
    WriteSPI_(0xFF);
    WriteSPI_(CMD);
    WriteSPI_(argument);
    argument = arg>>16;
    WriteSPI_(argument);
    argument = arg>>8;
    WriteSPI_(argument);
    argument = arg;
    WriteSPI_(argument);
    WriteSPI_(CRC);
}

int flag;
//
//void readover(int a)    /*Set Global Variable, Flag when reading SD Card content*/
//{
//    flag = a;
//}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

//    void reset()
//    {
//    int i = 0, count = 0;
//    unsigned char buff;
//    /*SOFTWARE RESET COMMAND*/
//    do{
//    dummy_clocks(8);    
//    command(0X40, 0X00000000, 0X95);    //CMD0
//    proceed();
//    do{   
//        buff = response();
//        count++;
//      }while((buff!=0X01) && (count<10) );
//      count = 0;
//    }while(buff!=0X01);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//    return; //CHECK FOR A STA_NODISK return
//}
//
//DSTATUS disk_initialize (void)
//{
//	DSTATUS stat;
//    int i = 0, count1 = 0, count2 = 0;
//    unsigned char buff;
//    reset(); //RESET THE SD CARD
//    dummy_clocks(8);    
//    command(0X41, 0X00000000, 0XFF);    //CMD1
//    proceed();
//    do{
//        buff = response();
//      }while(buff!=0x01);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//      proceed();
//if (buff == 0x01)
//{                    
//    //SEND CMD55 + ACMD41     
//    count1 = 0;        
//      do{
//        count2 = 0;
//        dummy_clocks(8);
//        command(0X77, 0X00000000, 0X95);    //CMD55
//        buff = 0XFF;
//        /*CHECK THE BUFFER FOR A 0X00 RESPONSE BIT*/
//        do{
//        buff = response();
//        count2++;
//        }while((buff!=0X01)&&(count2<10));
//        
//        count2 = 0;
//        dummy_clocks(8);
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
//      stat = 0X00;
//
//      }
//
//      else if(buff == 0x05)
//      {
//        stat = STA_NOINIT;
//      }
///*SETTING BLOCK LENGTH TO 512 Bytes*/
//    dummy_clocks(8);    
//    command(0X50,0X00000200,0XFF);    //CMD16
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//	return stat;
//}
//
//
///*-----------------------------------------------------------------------*/
///* Read Partial Sector                                                   */
///*-----------------------------------------------------------------------*/
//
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
//    dummy_clocks(8);    
//    command(0X52,start_add,0X00);    //CMD18
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
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
//    dummy_clocks(8);    
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
//    dummy_clocks(8);    
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
////            dummy_clocks(8);    
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
//                length++;
//              }
//              }
//              else
//              
//
//    return RES_OK;
//    }
//
//}
///*-----------------------------------------------------------------------*/
///* Write Partial Sector                                                  */
///*-----------------------------------------------------------------------*/
//
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
//    
//    dummy_clocks(8);    
//    command(0X58, sc*512, 0X00); // CMD24
//    proceed();
//    do{   
//        buff = response();
//      }while(buff!=0x00);
//    /*SOFTWARE RESET RESPONSE BIT OBTAINED (0X01)*/
//
//
//      dummy_clocks(8);
//
//      WriteSPI_(0XFE); //START TOKEN
//      
//      for(j=0;j<512;j++) //DATA BLOCK
//      {
//          if(*wr!='\0')
//          {
//              WriteSPI_(*wr);
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
//      dummy_clocks(8);    
//
//      command(0X4D,0X00000000,0X00);    //CMD13
//      proceed();
//      do{   
//        buff = response();
//        }while(buff!=0x00);
//      
//	return RES_OK;
//}
//
