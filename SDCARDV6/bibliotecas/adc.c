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
* HEADER-FILES (Somente os arquivos necess?rios nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "adc.h"

/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/
typedef struct 
{
    unsigned char canal;
    unsigned int leituras[8];
    unsigned int media;
}canal_ad;

canal_ad valor_leitura_adc[2];
/*****************************************************************************/

/******************************************************************************
* Prototipos das fun??es
******************************************************************************/

/*****************************************************************************/

/******************************************************************************
 * Funcao:		void inicializa_adc(void)
 * Entrada:		Nenhuma (void)
 * Sa?da:		Nenhuma (void)
 * Descri??o:	Inicializa hardware do conversor AD
 *****************************************************************************/
void inicializa_adc(void)
{
    /* Habilita canais 0 a 3 como analogico */
    ADCON1=0x0B;
    ADCON1bits.VCFG0 = 0;  // Refer?ncia em VDD
    ADCON1bits.VCFG1 = 0;  // Refer?ncia em VSS
    
	ADCON1bits.PCFG0 = 1;
	ADCON1bits.PCFG1 = 0;
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG3 = 1;
	
    ADCON2bits.ADFM  = 0;    //Ajusta justifica??o para esquerda
    ADCON2bits.ADCS0 = 1;  //Seleciona divisor FOSC/8
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS2 = 0;
    ADCON2bits.ACQT0 = 0; //Seleciona tempo de aquisi??o (0 us)
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT2 = 0;
    
    ADCON0bits.ADON  = 1; //Ativar m?dulo de ADC
}

/******************************************************************************
 * Funcao:		unsigned int leitura_adc(unsigned char canal)
 * Entrada:		unsigned char canal: canal do conversor AD (0 a 3)
 * Sa?da:		Nenhuma (void)
 * Descri??o:	Realiza a leitura do canal desejado do conversor AD
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

/******************************************************************************
 * Funcao:		void leitura_continua_adc(void)
 * Entrada:		Nenhuma
 * Sa?da:		Nenhuma (void)
 * Descri??o:	Realiza a leitura do conversor AD continuamente
 *****************************************************************************/
void leitura_continua_adc(void)
{
    for(unsigned char cont = 0; cont < 2; cont++)
    {
        for(unsigned char cont_int = 0; cont_int < 8; cont_int++)
        {
            valor_leitura_adc[cont].leituras[cont_int] = leitura_adc(cont);
            valor_leitura_adc[cont].media = valor_leitura_adc[cont].media + valor_leitura_adc[cont].leituras[cont_int];
        }
        valor_leitura_adc[cont].media = valor_leitura_adc[cont].media / 8;
    }
}

/******************************************************************************
 * Funcao:		unsigned int valor_medio_adc(unsigned char canal)
 * Entrada:		unsigned char canal: canal do conversor AD (0 a 1)
 * Sa?da:		unsigned int: retorna valor medio do canal do AD
 * Descri??o:	Retorna valor medio medido pelo ADC
 *****************************************************************************/
unsigned int valor_medio_adc(unsigned char canal)
{
    return(valor_leitura_adc[canal].media);
}