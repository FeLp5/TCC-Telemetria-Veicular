/******************************************************************************
 * Nome do Arquivo 	: hardware.h
 *
 * Descricao       	: Implementa funcoes de acesso ao harware do uC
 *
 * Ambiente			: MPLAB v3.51, XC8 v2.30, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 16/08/2020 - versao inicial 
 *
 *****************************************************************************/
#ifndef __HARDWARE_H
#define	__HARDWARE_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include <xc.h>
#include <pic18f4550.h>
/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/
void init_hardware(void);
void write_timer_zero(unsigned int timer0);
void open_timer0(unsigned char config);
void ms_delay(unsigned int val);
void config_timer_dois(void);
void timer_dois(void);
/*****************************************************************************/

#define _XTAL_FREQ                 48000000      //clock frequency: 48MHz

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

#endif	/* _HARDWARE_H */

