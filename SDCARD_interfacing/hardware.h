/******************************************************************************
 * Nome do Arquivo 	: hardware.h
 *
 * Descrição       	: Implementa a configuração do hardware do microcontrolador
 *
 * Ambiente			: MPLAB, XC8 versão 1.45, PIC18F4620
 *
 * Responsável		: Torres, Weslley
 *
 * Versão/Data		: v00.01 - 16/08/2020 - versão inicial 
 *
 *****************************************************************************/
#ifndef __HARDWARE_H
#define	__HARDWARE_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include <xc.h>
#include <p18f4550.h>

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

#define _XTAL_FREQ                 8000000      //clock frequency: 48MHz

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

#endif	/* _HARDWARE_H */

