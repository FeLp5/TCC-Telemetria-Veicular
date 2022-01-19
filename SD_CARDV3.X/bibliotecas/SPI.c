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
#include "display/display_lcd.h"
#include "SPI.h"
#include "SHRC.h"
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
    SSPCON1bits.SSPEN = 0;
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
    SSPCON1bits.SSPOV = 0;             

    
    
    SSPCON1bits.SSPM0 = 1;              //Configura para usar o Timer 2
    SSPCON1bits.SSPM1 = 1;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;
     
    
    //STATUS
    SSPSTATbits.SMP = 0;             //define o momento de amostragem, se ocorre no meio ou no final, nesse caso no final
    
    
    SSPCON1bits.SSPEN = 1;              //Define como mestre 
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
unsigned char recebe_dado_SPI(void)
{
    PIR1bits.SSPIF = 0;
    unsigned char temp_var;
    temp_var = SSPBUF;

//    SSPSTATbits.BF = 0;
    SSPBUF = 0x00;
//    while(!SSPSTATbits.BF);
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
    unsigned char TempVar;
    
    TempVar = SSPBUF; //Clear de BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPCON1bits.WCOL = 0;
//    SSPSTATbits.BF = 0;
    SSPBUF = dado;
    if (SSPCON1 & 0x80 )        // test if write collision occurred;
    {
        return (-1);              // if WCOL bit is set return negative #
    }
    else
    {
//         while( !SSPSTATbits.BF );  // wait until bus cycle complete 
        while(!PIR1bits.SSPIF){
           
            shrc_seta_bit(2);   
            controle_shrc();
            __delay_ms(1000);
//            PIR1bits.SSPIF = 1;
        }
//        shrc_seta_bit(0);
//        controle_shrc();
        // wait until bus cycle complete
        return (0);                // if WCOL bit is not set return non-negative#
    }

}



 






