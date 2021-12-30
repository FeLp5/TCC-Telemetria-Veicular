/******************************************************************************
 * Nome do Arquivo 	: main.c
 *
 * Descricao       	: Implementa a controle do conversor AD
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 20/08/2020 - versao inicial 
 *
 *****************************************************************************/
 /******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "adc.h"

/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/

/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

/*****************************************************************************/

/******************************************************************************
 * Funcao:		void inicializa_adc(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Inicializa hardware do conversor AD
 *****************************************************************************/
void inicializa_adc(void)
{
    /* Habilita canais 0 a 3 como analogico */
    ADCON1=0x0B;
    ADCON1bits.VCFG0 = 0;  // Referência em VDD
    ADCON1bits.VCFG1 = 0;  // Referência em VSS
    
	ADCON1bits.PCFG0 = 1;
	ADCON1bits.PCFG1 = 0;
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG3 = 1;
	
    ADCON2bits.ADFM = 1;    //Ajusta justificação para direita
    ADCON2bits.ADCS0 = 0;  //Seleciona divisor FOSC/64
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS2 = 1;
    ADCON2bits.ACQT0 = 1; //Seleciona tempo de aquisição (2,66 us)
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT2 = 0;
    ADCON0bits.ADON  = 1; //Ativar módulo de ADC
}

/******************************************************************************
 * Funcao:		unsigned int leitura_adc(unsigned char canal)
 * Entrada:		unsigned char canal: canal do conversor AD (0 a 3)
 * Saída:		Nenhuma (void)
 * Descrição:	Realiza a leitura do canal desejado do conversor AD
 *****************************************************************************/
unsigned int leitura_adc(unsigned char canal)
{
    switch(canal)
    {
        case 0:             /* Canal 0 */
            ADCON0 = 0x01;
            break;

        case 1:             /* Canal 1 */
            ADCON0 = 0x05;
            break;

        case 2:             /* Canal 2 */
            ADCON0 = 0x09;
            break;
        
        case 3:             /* Canal 3 */
            ADCON0 = 0x0D;
            break;

    }
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

    return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}
