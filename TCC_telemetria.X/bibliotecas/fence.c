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
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void armazena_ltlo(unsigned char *buffer, unsigned char index)
{
//    longitude_to_convert(&temp_buff_long);
    unsigned char i, j;
//    static unsigned char index = 0;
    
    
    
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


//void armazena_lat(void)
//{
//    unsigned char i, j;
//    j = 0;    
//    latitude_to_convert(&temp_buff_lat);
//    for(i=0; i<10; i++)
//    {
//        if(*temp_buff_lat != '\0' && *temp_buff_lat != '-')
//        {
//        
//            latitude_dado[i] = *temp_buff_lat;
//            j++;
//        }   
////        
//            
//       temp_buff_lat++;
//    }
////    posicao_cursor_lcd(1,0);
////    escreve_frase_ram_lcd(latitude_dado);
//    
//}


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
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
//            posicao_cursor_lcd(1,0);
//            escreve_inteiro_lcd(valor_lat);
        break;
        
        default:
            valor_long = convert_to_int_point_long();
            poligono_ext[0].diff_long = convert_to_int(fence_diff_long[0]) - valor_long;
            poligono_ext[1].diff_long = convert_to_int(fence_diff_long[2]) - valor_long;
//            posicao_cursor_lcd(2,0);
//            escreve_inteiro_lcd(valor_long);
        break;
    }
    


    
    
    
//    posicao_cursor_lcd(1,0);
//    escreve_inteiro_lcd(valor_long);
//    posicao_cursor_lcd(2,0);
//    escreve_inteiro_lcd(poligono_ext[1].diff_long );
    
}

/******************************************************************************
 * Funcao:		void leitura_chave(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura da chave em RC0
 *****************************************************************************/
//void calcula_diff_graus(unsigned char select_data)
//{
//    unsigned char point, count;
////    long int temp_var_deg;
//    
//    point = 0;
//    count = 0;
//    
//    switch(select_data)
//    {
//        case 1:
//                while(point<4)
//                {
//                    if(point == 0 || point == 2)
//                    {
//                        
////                        poligono_ext[count].diff_lat_deg = atoi()
//                        
//                        poligono_ext[count].diff_lat_deg = diff_calc(poligono_ext[count].fence_lat_deg, poligono_ext[count].point_lat_deg);
////                        posicao_cursor_lcd(1,0);
////                        escreve_inteiro_lcd( poligono_ext[1].diff_lat_deg);
////                        temp_var_deg = atoi(poligono_ext[count].point_lat_deg);
////                        poligono_ext[count].diff_lat_deg = atoi(poligono_ext[count].fence_lat_deg);
////                        poligono_ext[count].diff_lat_deg = poligono_ext[count].diff_lat_deg - temp_var_deg;            
//
//                        count++;
//                    }
//                    point++;
//                }
//        break;
//        
//        
//        
//        case 0:
//                while(point<4)
//                {
//                    if(point == 0 || point == 2)
//                    {     
//                        poligono_ext[count].diff_long_deg = diff_calc(poligono_ext[count].fence_long_deg, poligono_ext[count].point_long_deg);
//                        count++;
//                    }
//                    point++;
//                }
//        break;
//    }
//    
//    
//    
//
//    
//}
//


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
//void diferenca_lat(void)
//{
//    unsigned long int valor_lat;
//    valor_lat = convert_to_int_point_lat();
//    poligono_ext[0].diff_lat  = convert_to_int(fence_diff_lat[0]) - valor_lat;
//    poligono_ext[1].diff_lat  = convert_to_int(fence_diff_lat[2]) - valor_lat;
   
//}


/******************************************************************************
 * Funcao:		void leitura_chave(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura da chave em RC0
 *****************************************************************************/
//void calcula_diff_minutos(unsigned char select_data)
//{
//    unsigned char point, count;
////    unsigned long int temp_var_deg;
//    point = 0;
//    count = 0;
//    switch(select_data)
//    {
//        case 1:
//            while(point<4)
//            {
//                if(point == 0 || point == 2)
//                {
//                    poligono_ext[count].diff_lat_min = diff_calc(poligono_ext[count].fence_lat_min, poligono_ext[count].point_lat_min);
//
////                    posicao_cursor_lcd(2,15);
////                    escreve_inteiro_lcd(fence_flag[1].point);
//    
////                    poligono_ext[count].diff_lat_min = (atoi(poligono_ext[count].fence_lat_min)) - (atoi(poligono_ext[count].point_lat_min));
//                    count++;
//                }
//                point++;
//            }
//        break;
//        
//        case 0:
//            while(point<4)
//            {
//                if(point == 0 || point == 2)
//                {
//                    poligono_ext[count].diff_long_min = diff_calc(poligono_ext[count].fence_long_min, poligono_ext[count].point_long_min);
////                    poligono_ext[count].diff_long_min = (atoi(poligono_ext[count].fence_long_min)) - (atoi(poligono_ext[count].point_long_min));
//                    count++;
//                }
//                point++;
//            }
//        break; 
//    }
//    
//
//    
//}

/******************************************************************************
 * Funcao:		void verifica_plausibilidade(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char: 0 - chave desligada/ 1 - chave ligada
 * Descrição:	Retorna estado da chave tres (RC2)
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
        
        default:
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
                
    }
    

}

/******************************************************************************
 * Funcao:		void verifica_plausibilidade(void)
 * Entrada:		Nenhuma (void)
 * Saída:		unsigned char: 0 - chave desligada/ 1 - chave ligada
 * Descrição:	Retorna estado da chave tres (RC2)
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
        
        default:
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
                
    }
    

}

long int convert_to_int(const unsigned char *p_buff)
{
//    return strtol(p_buff, &convert, 10);
    p_buff++;
    return atol(p_buff);
//    return atoi(p_buff);
}

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
////    
//
//    
//    
//    return atoi(p_buff);
}


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
//    posicao_cursor_lcd(2,0);
//    escreve_frase_ram_lcd(convert_str_long);
    return (atol(convert_str_long));
    
}

/*Final do Arquivo modelo.c **************************************************/