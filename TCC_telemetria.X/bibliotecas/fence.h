///******************************************************************************
// * Nome do Arquivo 	: fence.h
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB, XC8 versao 1.45, PIC18F4550
// *
// * Responsavel		: Souza, Deivide Conceicao de
//			              Silva, Felipe Alves da
//			              Souza, Ricardo de 
//			  
// *
// * Versao/Data		: v00.01 - 26/09/2021 - versao inicial l
// *
// *****************************************************************************/
#ifndef __FENCE_H
#define __FENCE_H

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/

typedef struct
{
    unsigned char point:1;
}bit_field;



typedef struct
{
    unsigned char fence_lat_deg[5];
    unsigned char fence_long_deg[6];
    unsigned char point_lat_deg[5];
    unsigned char point_long_deg[6];
    long int diff_lat_deg;
    long int diff_long_deg;
    unsigned char fence_lat_min[6];
    unsigned char fence_long_min[6];
    unsigned char point_lat_min[6];
    unsigned char point_long_min[6];
    long int diff_lat_min;
    long int diff_long_min;
}fence_ext_struct;

/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/
void armazena_latitude(void);
void armazena_longitude(void);
void verifica_diferenca_graus(unsigned char select_data);
void verifica_diferenca_minutos(unsigned char select_data);
void calcula_diff_graus(void);
void calcula_diff_minutos(void);
unsigned char verifica_plausibilidade_long(void);
unsigned char verifica_plausibilidade_lat(void);

/*****************************************************************************/

#endif