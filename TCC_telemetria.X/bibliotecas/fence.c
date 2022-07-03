///******************************************************************************
// * Nome do Arquivo 	: fence.c
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

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessários nesse arquivo)
******************************************************************************/
#include "../main.h"
#include "display_lcd.h"
#include "fence.h"
#include "GPS.h"
#include "string.h"
#include "stdlib.h"
#include "SDCard.h"
/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


const unsigned char fence_diff_lat[4][10]  = {"-233646653", "-233646653", "-234189573", "-234189573"};
const unsigned char fence_diff_long[4][11] = {"-0463394050", "-0462941474", "-0462941474", "-0463394050"};

char *temp_buff_long; //=  "-2342.09112";
char *temp_buff_lat;

//volatile unsigned char latitude_dado[9];
volatile unsigned char dado[10];

fence_ext_struct poligono_ext[2];
//bit_field flag[3];
/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

long int convert_to_int(const unsigned char *p_buff);
unsigned long long int convert_to_int_point_long(void);
long int convert_to_int_point_lat(void);
/*****************************************************************************/



/******************************************************************************
 * Funcao:		void armazena_latitude(unsigned char *buffer, unsigned char index)
 * Entrada:		unsigned char *buffer, unsigned char index
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena latitude/longitude no buffer para verificacao da Cerca
 *****************************************************************************/
void armazena_ltlo(unsigned char *buffer, unsigned char index)
{
    unsigned char i, j;

    switch(index)
    {
        case 1:    
            j = 0;
            for(i=0; i<10; i++)
            {
                if(*buffer != '\0')
                {

                    dado[j] = *buffer;
                    j++;
                }   
        //        
                buffer++;
            }
    
        break;
        
        default:
            j = 0;
            for(i=0; i<11; i++)
            {
                if(*buffer != '\0')
                {

                    dado[j] = *buffer;
                    j++;
                }   
        //        
                buffer++;
            }
        break;
                    
        
    }

    diferenca(index);
    


    
}


/******************************************************************************
 * Funcao:		void armazena_latitude(unsigned char index)
 * Entrada:		unsigned char
 * Saída:		Nenhuma (void)
 * Descrição:	Calcula a diferença entre latitude/longitude do
 *  ponto da cerca e latitude/longitude de onde o veiculo esta.
 *****************************************************************************/
void diferenca(unsigned char index)
{
    unsigned long int valor_lat, valor_long;
    
    switch(index)
    {
        case 1:
            valor_lat = convert_to_int_point_lat();
            poligono_ext[0].diff_lat  = convert_to_int(fence_diff_lat[0]) - valor_lat;
            poligono_ext[1].diff_lat  = convert_to_int(fence_diff_lat[2]) - valor_lat;
        break;
        
        default:
            valor_long = convert_to_int_point_long();
            poligono_ext[0].diff_long = convert_to_int(fence_diff_long[0]) - valor_long;
            poligono_ext[1].diff_long = convert_to_int(fence_diff_long[2]) - valor_long;
        break;
    }

}




/******************************************************************************
 * Funcao:		unsigned char verifica_plausibilidade_long(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char
 * Descrição:	Verifica a posicao do veiculo com relacao a longitude e retorna 
 * um falor booleano
 *****************************************************************************/
unsigned char verifica_plausibilidade_long(void)
{
    char posicao = posicao_long(); 
    switch(posicao)
    {
        case '-':
            if(
                (poligono_ext[0].diff_long >= 0  &&
               poligono_ext[1].diff_long <= 0))         
            {
                return 0;
            }
            else if( 
                    (poligono_ext[0].diff_long == 0 ||
                    poligono_ext[1].diff_long == 0 )) 
            {
                return 0;
            }
            else
            {
                return 1;
            }
        break;
        
        case '+':
            if(
               (poligono_ext[0].diff_long <= 0  &&
               poligono_ext[1].diff_long >= 0 ) )
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_long == 0 ||
                    poligono_ext[1].diff_long == 0 ))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        break;
        default:
            return 1;
        break;
    }
    

}

/******************************************************************************
 * Funcao:		unsigned char verifica_plausibilidade_lat(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char
 * Descrição:	Verifica a posicao do veiculo com relacao a latitude e retorna 
 * um falor booleano
 *****************************************************************************/
unsigned char verifica_plausibilidade_lat(void)
{
    char posicao = posicao_lat(); 
    switch(posicao)
    {
        case '-':
            if(
              (  poligono_ext[0].diff_lat <= 0  &&
               poligono_ext[1].diff_lat >= 0))
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat == 0 ||
                    poligono_ext[1].diff_lat == 0)) 
  
            {
                return 0;
            }
            else
            {

                return 1;
            }
        break;
        
        case '+':
            if((poligono_ext[0].diff_lat >= 0  &&
               poligono_ext[1].diff_lat <= 0 ))
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat >= 0  ||  
                    poligono_ext[1].diff_lat <= 0))
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat == 0 ||
                    poligono_ext[1].diff_lat == 0))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        break;
        
        default:
            return 1;
        break;
    }
    

}
/******************************************************************************
 * Funcao:		long int convert_to_int(const unsigned char *p_buff)
 * Entrada:		Nenhuma (void)
 * Saída:		long int
 * Descrição:	Realiza a conversao de string para inteiro
 *****************************************************************************/

long int convert_to_int(const unsigned char *p_buff)
{
    p_buff++;
    return atol(p_buff);
}

/******************************************************************************
 * Funcao:		long int convert_to_int_point_lat(void)
 * Entrada:		Nenhuma (void)
 * Saída:		long int
 * Descrição:	Trata a string da latitude e realiza a conversao de string para inteiro
 *****************************************************************************/

long int convert_to_int_point_lat(void)
{
    unsigned char **temp_str_lat = temp_buff_lat; 
    unsigned char convert_str_lat[9];
    
    unsigned char i, j;

    j= 0;
    for(i=0; i<10;i++)
    {
        if(dado[i] != '\0')
        {

               convert_str_lat[j] =  dado[i];
               j++;
        }  
    }

    return (atol(convert_str_lat));
}


/******************************************************************************
 * Funcao:		long int convert_to_int_point_lat(void)
 * Entrada:		Nenhuma (void)
 * Saída:		long int
 * Descrição:	Trata a string da longitude e realiza a conversao de string para inteiro
 *****************************************************************************/

unsigned long long int convert_to_int_point_long(void)
{
    unsigned char *p;; 
    unsigned char i, j;
    unsigned char convert_str_long[10];
    
    
    j = 0;
    for(i=0; i<10;i++)
    {
        if(dado[i]  != '\0')
        {

               convert_str_long[j] =  dado[i];
               j++;
        } 
    }

    return (atol(convert_str_long));
    
}

/*Final do Arquivo modelo.c **************************************************/