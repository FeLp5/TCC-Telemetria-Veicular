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
#include "display_lcd.h"

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
    PORT_CHIP_SELECT = 0;
    PORT_CLOCK = 0;
    PORT_DATA_IN = 1;
    PORT_DATA_OUT = 0;
    
    CHIP_SELECT = 1;
	SHIFT_CLOCK = 0;
	SPI_DATA_OUT  = 0;
}	

/******************************************************************************
 * Funcao:		void envia_dados_shrc(unsigned char data_to_shrc)
 * Entrada:		unsigned char data_to_shrc - dado para ser enviado ao CI 74HC595
 *				
 * Saída:		Nenhuma (void)
 * Descrição:	Envia dados para o CI 74HC595. 
 *****************************************************************************/
void escreve_dado_SPI(unsigned char data_to_shrc)
{
    SPI_DATA_OUT = 0;
    unsigned char contador=0;
    di();
    
	while(contador<=7)
	{
		if((data_to_shrc & 0x80) == 0)
		{
			SPI_DATA_OUT = 0;
		}
		else
		{
			SPI_DATA_OUT = 1;
		}
		data_to_shrc = data_to_shrc << 1;
		__delay_us(0.005);
		SHIFT_CLOCK = 1;
        __delay_us(0.005);
		SPI_DATA_OUT  = 0;
		__delay_us(0.005);
		SHIFT_CLOCK = 0;
        __delay_us(0.005);
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
//void ativa_latch_shrc ( void )
//{
//    //Inicio da funcao.
//    SHIFT_LATCH = 0;
//    __delay_us(0.005);
//    SHIFT_LATCH = 1;
//    __delay_us(60);
//    SHIFT_LATCH = 0;
//}

/******************************************************************************
 * Funcao:		void controle_shrc (void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Realiza a inicializacao das portas conectadas ao CI 74HC595
 *****************************************************************************/
//void controle_shrc (void)
//{    
//
//   envia_dados_shrc(dado);
//   ativa_latch_shrc();
//}
//
//void shrc_seta_bit(unsigned char posicao_bit)
//{
//	dado |= (1<<posicao_bit);
//}
//
//void shrc_apaga_bit(unsigned char posicao_bit)
//{
//	dado &= ~(1<<posicao_bit);
//}



/******************************************************************************
 * Funcao:		void controle_shrc (void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Realiza a inicializacao das portas conectadas ao CI 74HC595
 *****************************************************************************/
unsigned char recebe_dado_SPI()
{
    dado = 0;
    unsigned char contador=0;
    
    di();
//    SPI_DATA_OUT = 0;
//    CHIP_SELECT = 0;
	while(contador<=7)
	{     
        dado |= (SPI_DATA_IN << (7-contador));

		__delay_us(0.005);
		SHIFT_CLOCK = 1;
        __delay_us(0.005);
		__delay_us(0.005);
		SHIFT_CLOCK = 0;
        __delay_us(0.005);
		contador++;
	}
    ei();
	return dado;
}
