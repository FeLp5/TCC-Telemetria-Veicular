/******************************************************************************
 * Nome do Arquivo 	: int_eeprom.c
 *
 * Descricao       	: Implementa a escrita e leitura da memoria EEPROM interna
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 01/08/2021 - versao inicial 
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "int_eeprom.h"

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
 * Funcao:		void escreve_eeprom_imterna 	(	unsigned int address, 
													unsigned char data)
 * Entrada:		unsigned int address: endereço na EEPROM interna
				unsigned char data: dado para ser escrito na EEPROM interna
 * Saída:		Nenhuma (void)
 * Descrição:	Função principal
 *****************************************************************************/
void escreve_eeprom_imterna (unsigned int address, unsigned char data)
{
    /* Write Operation*/
    EEADR=address;			
    EEDATA=data;			
    EECON1bits.EEPGD=0;		
    EECON1bits.CFGS=0;		
    EECON1bits.WREN=1;		
    INTCONbits.GIE=0;		
    
    EECON2=0x55;
    EECON2=0xaa;
    
    EECON1bits.WR=1;		
    INTCONbits.GIE=1;		
    
    while(PIR2bits.EEIF==0);	
    PIR2bits.EEIF=0;		
}

/******************************************************************************
 * Funcao:		unsigned char leitura_eeprom_imterna(unsigned int address)
 * Entrada:		unsigned int address: endereço na EEPROM interna
 * Saída:		Nenhuma (void)
 * Descrição:	Função principal
 *****************************************************************************/
unsigned char leitura_eeprom_imterna(unsigned int address)
{
    /*Read operation*/
    EEADR=address;	/* Read data at location 0x00*/
    EECON1bits.WREN=0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.RD=1;	/* To Read data of EEPROM memory set RD=1*/
    return(EEDATA);
}

/******************************************************************************
 * Funcao:		void escreve_string_eeprom_imterna(	unsigned int address, 
													unsigned char *data)
 * Entrada:		unsigned int address: endereço na EEPROM interna
				unsigned char data: dado para ser escrito na EEPROM interna
 * Saída:		Nenhuma (void)
 * Descrição:	Função principal
 *****************************************************************************/
void escreve_string_eeprom_imterna(unsigned int address, unsigned char *data)
{
    /*Write Operation for String*/
    while(*data!=0)
    {
        escreve_eeprom_imterna(address,*data);
        address++;
        *data++;
    }    
}