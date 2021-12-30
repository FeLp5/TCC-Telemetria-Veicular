/******************************************************************************
 * Nome do Arquivo 	: chaves.c
 *
 * Descrição       	: Implementa a leitura de chaves do projeto
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsável		: Weslley Torres
 *
 * Versão/Data		: 01.00 - 16/08/2021 - Versao inicial 
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/
#include "hardware.h"
#include "main.h"
#include "biblioteca/display_lcd.h"
/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/




unsigned char info_chaves[7];


/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
 * Funcao:		void leitura_chaves_sistema(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Função para leitura das chaves conectadas ao uC
 *****************************************************************************/
void leitura_chaves_sistema(void)
{
    info_chaves[0] = PORTCbits.RC0;
    info_chaves[1] = PORTCbits.RC1;
    info_chaves[2] = PORTCbits.RC2;
    info_chaves[3] = PORTAbits.RA2;
    info_chaves[4] = PORTAbits.RA5;
    info_chaves[5] = PORTEbits.RE0;
    info_chaves[6] = PORTEbits.RE1;
    
}

/******************************************************************************
 * Funcao:		void leitura_chave(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura da chave em RC0
 *****************************************************************************/
void leitura_chave(void)
{
    unsigned char chave = 0;
    unsigned char chave_dois = 0;
    
    const unsigned char pos_ignicao_off[NUM_CARACTERES]    = " Ign Off";
    const unsigned char pos_ignicao_crank[NUM_CARACTERES]    = " Ign Crank";
    const unsigned char pos_ignicao_on[NUM_CARACTERES]    = " Ign On";
    
    chave = PORTCbits.RC0;
    chave_dois = PORTCbits.RC1;
    
    if((chave == 0 && chave_dois == 0) || (chave == 1 && chave_dois == 0))
    {
        LIMPA_DISPLAY();
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd(pos_ignicao_off);
    }
    else if (chave == 0 && chave_dois == 1)
    {
        LIMPA_DISPLAY();
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd(pos_ignicao_crank);
    }
    else
    {
        LIMPA_DISPLAY();
        posicao_cursor_lcd(1,0);
        escreve_frase_ram_lcd(pos_ignicao_on);        
    }
}


/******************************************************************************
 * Funcao:		unsigned char chave_tres(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char: 0 - chave desligada/ 1 - chave ligada
 * Descrição:	Retorna estado da chave tres (RC2)
 *****************************************************************************/
unsigned char chave_tres(void)
{
    
    return (info_chaves[2]);
 
}

/******************************************************************************
 * Funcao:		unsigned char chave_quatro(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char: 0 - chave desligada/ 1 - chave ligada
 * Descrição:	Retorna estado da chave tres (RA2)
 *****************************************************************************/
unsigned char chave_quatro(void)
{
    
    return (info_chaves[3]);
 
}

/******************************************************************************
 * Funcao:		unsigned char chave_quatro(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char: 0 - chave desligada/ 1 - chave ligada
 * Descrição:	Retorna estado da chave tres (RA2)
 *****************************************************************************/
unsigned char chave_zero(void)
{
    
    return (info_chaves[0]);
 
}
/*Final do Arquivo modelo.c **************************************************/