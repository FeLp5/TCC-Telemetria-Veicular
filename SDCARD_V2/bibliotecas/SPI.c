/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Conceição de
			  Silva, Felipe Alves da
			  Souza, Ricardo de
			  
 *
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "display_lcd.h"
#include "SPI.h"
//#include "SHRC.h"
/*****************************************************************************/


/******************************************************************************
* Variaveis Globais
******************************************************************************/
unsigned char estado = 0;
/*****************************************************************************/


/******************************************************************************
* Prototipos das funções
******************************************************************************/


/*****************************************************************************/
 

/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		config_SPI SPI, essa estrutura cont�m o modo de transmiss�o e o identificador se � escravo ou n�o
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a parametriza��o do m�dulo SPI
 *****************************************************************************/
void inicializa_SPI(unsigned char sync_mode, unsigned char bus_mode, unsigned char smp_phase)
{
    TRIS_SCK  = 0;
    TRIS_CS  =  0; 
    TRIS_SDI =  1;               // define SDI input
    TRIS_SDO =  0;               //SD0 as outpu
     
    CHIP_SELECT = 1;
    SSPSTAT &= 0x3F;			// power on state 
	SSPCON1 = 0x00;
//    SSPSTAT &= 0x3F;                // power on state
//    SSPCON1 = 0x00;                 // power on state
//    SSPCON1 |= sync_mode;           // select serial mode       (0b00000010 - SPI Master mode, clock = Fosc/64)
//    SSPSTAT |= smp_phase;           // select data input sample phase
//    switch(bus_mode)
//    {
//        case 0:                       // SPI bus mode 0,0
//          SSPSTATbits.CKE = 1;       // data transmitted on rising edge
//          break;    
//        case 2:                       // SPI bus mode 1,0
//          SSPSTATbits.CKE = 1;       // data transmitted on falling edge
//          SSPCON1bits.CKP = 1;       // clock idle state high
//          break;
//        case 3:                       // SPI bus mode 1,1
//          SSPCON1bits.CKP = 1;       // clock idle state high
//          break;
//        default:                      // default SPI bus mode 0,1
//          break;
//    }
//    
//    switch( sync_mode )
//    {
//    case 4:                         // slave mode w /SS enable
//          TRIS_SCK  = 1;            // define clock pin as input    
//          TRIS_CS  = 1;        // define /SS1 pin as input
//        break;
//
//    case 5:                   // slave mode w/o /SS enable
//        TRIS_SCK  = 1;        // define clock pin as input    
//        break;
//
//    default:                 // master mode, define clock pin as output
//        TRIS_SCK  = 0;       // define clock pin as output    
//         break;
//    }

    SSPSTATbits.SMP = 0;
    SSPSTATbits.CKE = 1;
    SSPCON1bits.CKP = 0;
    
    SSPCON1 = 0b00000011;
 
    
    CHIP_SELECT = 1;
    
//    SSPCON1 |= 0x20;          // enable synchronous serial port,  0b00100000  Enable serial port and configures SCK, SDO, SDI
    SSPCON1bits.SSPEN = 1;
    posicao_cursor_lcd(1,1);
    escreve_frase_ram_lcd("SPI configurada!");
    LIMPA_DISPLAY();
}



/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		Nenhuma
 * Saida:		unsigned char
 * Descricao:	Realiza a leitura do buffer do SPI
 *****************************************************************************/
unsigned char ReadSPI_(void)
{
//    unsigned char temp_var;
//    temp_var = SSPBUF;
//    PIR1bits.SSPIF = 0;
//    SSPSTATbits.BF = 0;
//    SSPBUF = 0x00;
    SSPBUF = 0xFF;
//    __delay_us(100);
    while(!SSPSTATbits.BF);
    return (SSPBUF);
}

/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		unsigned char dado
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a transmiss�o do dado (quando o buff � alterado o m�dulo 
 * SPI envia automaticamente o dado (Mestre)
 *****************************************************************************/
void WriteSPI_(unsigned char dado)
{
    unsigned char TempVar;
    TempVar = SSPBUF; //Clear de BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
//    SSPCON1bits.WCOL = 0;
//    SSPSTATbits.BF = 0;
//    posicao_cursor_lcd(1,15);
//    escreve_inteiro_lcd(PIR1bits.SSPIF);
//    SSPBUF = 0x00
//    __delay_us(100);;
    SSPBUF = dado;
    while(!PIR1bits.SSPIF);
}



 






