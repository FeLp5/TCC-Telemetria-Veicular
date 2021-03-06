/******************************************************************************
 * Nome do Arquivo 	: main.h
 *
 * Descri??o      	: Implementa a fun??o principal do projeto
 *
 * Ambiente			: MPLAB v3.51, XC8 versao 2.30, PIC18F4550
 *
 * Respons?vel		: Torres, Weslley
 *
 * Vers?o/Data		: v00.01 - 16/08/2020 - vers?o inicial
 *					  
 *
 *****************************************************************************/
#ifndef __MAIN_H
#define __MAIN_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Definicoes
******************************************************************************/

#define YES                        1
#define NO                         0

#define ENABLE                     YES
#define DISABLE                    NO 

#define NUM_CARACTERES				17

#define NUMBER_OF_TASKS				3

#define VELOCIDADE_LIMITE           50

#define TEMPO_SD                    28000

#define TIME_150_MS					150   //time unit: ms
#define TIME_50_MS                  50
#define TIME_100_MS                 100 
#define TIME_200_MS					200   //time unit: ms
#define TIME_500_MS					500   //time unit: ms
#define TIME_1000_MS				1000   //time unit: ms
#define TIME_2000_MS                2000   //time unit: ms
#define TIME_5000_MS                5000
#define TIME_10000_MS                10000


#define TASK_TIMEOUT				65000  //time unit: ms

#define BIT_0   0
#define BIT_1   1
#define BIT_2   2
#define BIT_3   3
#define BIT_4   4
#define BIT_5   5
#define BIT_6   6
#define BIT_7   7

typedef struct
{
    unsigned char point:1;
}bit_field;


/*****************************************************************************/

/******************************************************************************
* Prototipos das funcoes
******************************************************************************/


/*****************************************************************************/

#endif