/******************************************************************************
 * Nome do Arquivo 	: SPI.c
 *
 * Descricao       	: Implementa o controle protocolo SPI
 *
 * Ambiente			    : MPLAB v3.51, XC8 v2.32, PIC18F4550
 *
 * Responsavel		: Souza, Deivide Conceição de
			  Silva, Felipe Alves da
			  Souza, Ricardo de
			  
 *
 * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "display_lcd.h"
#include "SPI.h"
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
 * Funcao:		void inicializa_SPI(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a parametrizacao do modulo SPI 
 *****************************************************************************/
void inicializa_SPI(void)
{
    TRIS_SCK  = 0;
    TRIS_CS  =  0; 
    TRIS_SDI =  1;               // define SDI input
    TRIS_SDO =  0;               //SD0 as outpu
     
    CHIP_SELECT = 1; // deselect sdcard
    SSPSTAT &= 0x3F;			// power on state 
	SSPCON1 = 0x00;
    
    SSPSTATbits.SMP = 0;
    SSPSTATbits.CKE = 1;
    SSPCON1bits.CKP = 0;
    
    SSPCON1 = 0b00000011; // CLOCK defined as TIMER2/2
    
    SSPCON1bits.SSPEN = 1;
}

void desliga_SPI(void)
{
    SSPSTAT = 0x00;			// power on state 
	SSPCON1 = 0x00;
    SSPCON1bits.SSPEN = 0;
}


/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		Nenhuma
 * Saida:		unsigned char
 * Descricao:	Realiza a leitura do buffer do SPI
 *****************************************************************************/
unsigned char leitura_SPI(void)
{
    SSPBUF = 0xFF;
    SSPSTATbits.BF = 0;
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
void escreve_SPI(unsigned char dado)
{
    unsigned char TempVar;
    TempVar = SSPBUF; //Clear de BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
//    SSPSTATbits.BF =0;
    SSPBUF = dado;
    while(!PIR1bits.SSPIF);
//    while(!SSPSTATbits.BF);
}



 






