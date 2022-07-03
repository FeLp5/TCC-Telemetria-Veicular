///******************************************************************************
// * Nome do Arquivo 	: fence.h
// *
// * Descricao       	: Implementa a interface para gravacao no sd card
// *
// * Ambiente			 : MPLAB, XC8 versao v2.30, PIC18F4550
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
//    unsigned char fence_lat_deg[5];
//    unsigned char fence_long_deg[6];
//    unsigned char point_lat_deg[5];
//    unsigned char point_long_deg[6];
    long int diff_lat;
    long int diff_long;
//    unsigned char fence_lat_min[6];
//    unsigned char fence_long_min[6];
//    unsigned char point_lat_min[6];
//    unsigned char point_long_min[6];
//    long long int diff_lat_min;
//    long long int diff_long_min;
}fence_ext_struct;

/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/
//void armazena_latitude(void);
//void armazena_long(void);
//void armazena_lat(void);
void armazena_ltlo(unsigned char *buffer, unsigned char index);
//void armazena_longitude(void);
void diferenca(unsigned char index);
//void diferenca_long(void);
//void diferenca_lat(void);
//void verifica_diferenca_minutos(unsigned char select_data);
//void calcula_diff_graus(unsigned char select_data);
//void calcula_diff_minutos(unsigned char select_data);
unsigned char verifica_plausibilidade_long(void);
unsigned char verifica_plausibilidade_lat(void);

/*****************************************************************************/

#endif