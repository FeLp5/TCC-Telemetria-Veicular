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
#include "biblioteca/display_lcd.h"
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
 * Funcao:		void main(void)
 * Entrada:		config_SPI SPI, essa estrutura cont�m o modo de transmiss�o e o identificador se � escravo ou n�o
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a parametriza��o do m�dulo SPI
 *****************************************************************************/
void inicializa_SPI(unsigned char SPI_mode)
{
    switch(SPI_mode)
    {
        case 0:
            SSPCON1bits.CKP   = 0;              // Define o n�vel logico baixo como iddle (polaridade do clock)
            SSPSTATbits.CKE   = 0;                //Define se ir� transmitir no n�vel logico zero ou um
            break;
           
        case 1:
            SSPCON1bits.CKP   = 1;              // Define o n�vel logico baixo como iddle (polaridade do clock)
            SSPSTATbits.CKE   = 0;                //Define se ir� transmitir no n�vel logico zero ou um
            break;
        
        case 2:
            SSPCON1bits.CKP   = 0;              // Define o n�vel logico baixo como iddle (polaridade do clock)
            SSPSTATbits.CKE   = 1;                //Define se ir� transmitir no n�vel logico zero ou um
            break;
        
        case 3:
            SSPCON1bits.CKP   = 1;              // Define o n�vel logico baixo como iddle (polaridade do clock)
            SSPSTATbits.CKE   = 1;                //Define se ir� transmitir no n�vel logico zero ou um
            
         default:
            SSPCON1bits.CKP   = 0;              // Define o n�vel logico baixo como iddle (polaridade do clock)
            SSPSTATbits.CKE   = 0;                //Define se ir� transmitir no n�vel logico zero ou um
            break;
    }
    
    
    //SSPCON
    SSPCON1bits.WCOL  = 0;              //Define que n�o haver� colis�o
    //SSPCON1bits.SSPOV = 0;              // n�o � setado desde que seja lido o buffer
    SSPCON1bits.SSPEN = 1;              //Define como mestre 
    
    SSPCON1bits.SSPM0 = 1;              //Configura para usar o Timer 2
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;
    
    //STATUS
    SSPSTATbits.SMP = 1;             //define o momento de amostragem, se ocorre no meio ou no final, nesse caso no final
    
    
    posicao_cursor_lcd(1,1);
    escreve_frase_ram_lcd("SPI configurada!");
}



/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		Nenhuma
 * Saida:		unsigned char
 * Descricao:	Realiza a leitura do buffer do SPI
 *****************************************************************************/
unsigned char recebe_dado_SPI(void)
{
    PIR1bits.SSPIF = 0;
    SSPBUF = 0x00;
    while(!PIR1bits.SSPIF);
    return (SSPBUF);
}

/******************************************************************************
 * Funcao:		void main(void)
 * Entrada:		unsigned char dado
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a transmiss�o do dado (quando o buff � alterado o m�dulo 
 * SPI envia automaticamente o dado (Mestre)
 *****************************************************************************/
char WriteSPI_(unsigned char dado)
{
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPCON1bits.WCOL = 0;
    SSPBUF = dado;
    if (SSPCON1 & 0x80 )        // test if write collision occurred;
    {
        return (-1);              // if WCOL bit is set return negative #
    }
    else
    {
        // while( !SSPSTATbits.BF );  // wait until bus cycle complete 
        while(!PIR1bits.SSPIF ); // wait until bus cycle complete   
        return (0);                // if WCOL bit is not set return non-negative#
    }
}



 






