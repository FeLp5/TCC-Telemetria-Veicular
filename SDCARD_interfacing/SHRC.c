/******************************************************************************
 * Nome do Arquivo 	: SHRC.h
 *
 * Descricao       	: Implementa a controle do registrador de deslocamento
 *
 * Ambiente			    : MPLAB, XC8 versao 1.45, PIC18F4550
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
#include "SHRC.h"

/*****************************************************************************/


/******************************************************************************
* Variaveis Globais
******************************************************************************/
unsigned char dado;

/*****************************************************************************/


/******************************************************************************
* Prototipos das funções
******************************************************************************/
void envia_dados_shrc(unsigned char  data_to_shrc);
void ativa_latch_shrc ( void );

/*****************************************************************************/
 
 
/******************************************************************************
 * Funcao:		void inicializa_shrc(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Realiza a inicializacao das portas conectadas ao CI 74HC595
 *****************************************************************************/
void inicializa_shrc(void)
{
    SHIFT_LATCH = 0;
	SHIFT_CLOCK = 0;
	SHIFT_DATA  = 0;

}	

/******************************************************************************
 * Funcao:		void envia_dados_shrc(unsigned char data_to_shrc)
 * Entrada:		unsigned char data_to_shrc - dado para ser enviado ao CI 74HC595
 *				
 * Saída:		Nenhuma (void)
 * Descrição:	Envia dados para o CI 74HC595. 
 *****************************************************************************/
void envia_dados_shrc(unsigned char data_to_shrc)
{
    unsigned char contador=0;
    di();
    
	while(contador<=7)
	{
		if((data_to_shrc & 0x80) == 0)
		{
			SHIFT_DATA = 0;
		}
		else
		{
			SHIFT_DATA = 1;
		}
		data_to_shrc = data_to_shrc << 1;
		__delay_us(30);
		SHIFT_CLOCK = 1;
        __delay_us(30);
		SHIFT_DATA  = 0;
		__delay_us(30);
		SHIFT_CLOCK = 0;
        __delay_us(30);
		contador++;
	}
    ei();
}

/******************************************************************************
 * Funcao:		static void ativa_latch_shrc Escreve_Latch ( void )
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Finaliza o processo de envio de dados para o CI 74HC595
 *****************************************************************************/
void ativa_latch_shrc ( void )
{
    //Inicio da funcao.
    SHIFT_LATCH = 0;
    __delay_us(30);
    SHIFT_LATCH = 1;
    __delay_us(60);
    SHIFT_LATCH = 0;
}

/******************************************************************************
 * Funcao:		void controle_shrc (void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Realiza a inicializacao das portas conectadas ao CI 74HC595
 *****************************************************************************/
void controle_shrc (void)
{    

   envia_dados_shrc(dado);
   ativa_latch_shrc();
}

void shrc_seta_bit(unsigned char posicao_bit)
{
	dado |= (1<<posicao_bit);
}

void shrc_apaga_bit(unsigned char posicao_bit)
{
	dado &= ~(1<<posicao_bit);
}
