 /******************************************************************************
 * Nome do Arquivo 	: uart.h
 *
 * Descrição       	: Implementa interfaces para comunicação serial UART
 *
 * Ambiente			: MPLAB, XC8 versão 1.45, PIC18F4550
 *
 * Responsável		: Torres, Weslley
 *
 * Versão/Data		: v00.01 - 09/05/2017 - versão inicial 
 *
 *****************************************************************************/
 
#ifndef __UART_H
#define __UART_H


/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/

#include "hardware.h"

/*****************************************************************************/

/******************************************************************************
* Defines e Tipos Criados pelo densenvolvedor
******************************************************************************/

#define PINO_TX		TRISCbits.RC6
#define PINO_RX		TRISCbits.RC7

/*****************************************************************************/


/******************************************************************************
* Prototipos das funções
******************************************************************************/

void envia_dado_uart(char data);

unsigned char verifica_buffer_uart(void);

void envia_string_uart(const char *string);

char verifica_dado_disponivel_uart(void);

char recebe_dado_uart(void);

void recebe_string_uart(char *buffer, unsigned char length);

void escreve_inteiro_uart(int valor);

void inicializa_uart(void);

/*****************************************************************************/

#endif
