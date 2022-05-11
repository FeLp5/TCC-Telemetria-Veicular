///******************************************************************************
// * Nome do Arquivo 	: fence.c
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


const unsigned char fence_diff_lat[4][10]  = {"-234158961", "-234158961", "-234200179", "-234200179"};
const unsigned char fence_diff_long[4][11] = {"-0463052101", "-0453008424", "-0453008424", "-0463052101"};

char *temp_buff_lat; //=  "-2342.09112";
char *temp_buff_long;// = "-04629.73479";

fence_ext_struct poligono_ext[2];
//bit_field flag[3];
/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

long int convert_to_int(const unsigned char *p_buff);
long int convert_to_int_point(const unsigned char *p_buff);
/*****************************************************************************/

/******************************************************************************
 * Funcao:		void armazena_longitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void armazena_longitude(void)
{
    temp_buff_long = Longitude();
}


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void armazena_latitude(void)
{
    temp_buff_lat = Latitude();
}


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void verifica_diferenca(void)
{
    poligono_ext[0].diff_long = convert_to_int(fence_diff_long[0]) - convert_to_int_point(temp_buff_long);
//    posicao_cursor_lcd(1,0);
//    escreve_inteiro_lcd(poligono_ext[0].diff_long);
    poligono_ext[1].diff_long = convert_to_int(fence_diff_long[2]) - convert_to_int_point(temp_buff_long);
    poligono_ext[0].diff_lat  = convert_to_int(fence_diff_lat[0]) - convert_to_int_point(temp_buff_lat);
    poligono_ext[1].diff_lat  = convert_to_int(fence_diff_lat[2]) - convert_to_int_point(temp_buff_lat);
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
//void verifica_diferenca_minutos(unsigned char select_data)
//{
//    unsigned char point, point_index_lat ,point_index_long;
//    unsigned char i, j;
//    
//    point_index_lat = 0;
//    point_index_long = 0;
//    point = 0;
//    switch(select_data)
//    {
//        case 1:
//            while(point<4)
//            {
//                for(i=6; i<11; i++)
//                {
//                    if(point == 0 || point == 2)
//                    {
//                        if(point == 2 && !point_index_lat)
//                        {
//                            point_index_lat++;
//                        }
//                        for(j= 0; i < 11; j++)
//                        {
//                            poligono_ext[point_index_lat].fence_lat_min[j] = fence_diff_lat[point][i];
//                            poligono_ext[point_index_lat].point_lat_min[j] = temp_buff_lat[i];
//                                i++;
//                        }
//                    }
//                    else
//                    {
//                        break;
//                    }    
//                }
//                point++;
//            }
//            break;
//            
//        case 0:
//            while(point<4)
//            {
//                for(i=7; i<12; i++)
//                {
//                    if(point == 0 || point == 2)
//                    {
//                        if(point == 2 && !point_index_long)
//                        {
//                            point_index_long++;
//                        }
//
//                        for(j= 0; i < 12; j++)
//                        {
//                            poligono_ext[point_index_long].fence_long_min[j] = fence_diff_long[point][i];
//                            poligono_ext[point_index_long].point_long_min[j] = temp_buff_long[i];
//                            i++;
//                        }
//                    }
//                    else
//                    {
//                        break;
//                    }
//
//                }
//                point++;
//            }
//            break;     
//    }  
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
    switch(temp_buff_long[0])
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
                    poligono_ext[0].diff_long == 0 ))
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
    unsigned char flag = 0;
    switch(temp_buff_lat[0])
    {
        case '-':
            if(
              (  poligono_ext[0].diff_lat <= 0  &&
               poligono_ext[1].diff_lat >= 0))
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat == 0 &&
                    poligono_ext[0].diff_lat == 0)) 
  
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
                    (poligono_ext[0].diff_lat >= 0  &&  
                    poligono_ext[1].diff_lat <= 0))
            {
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat == 0 ||
                    poligono_ext[0].diff_lat == 0))
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
    unsigned char *convert;
//    return strtol(p_buff, &convert, 10);
    p_buff++;
    return atol(p_buff);
//    return atoi(p_buff);
}

long int convert_to_int_point(const unsigned char *p_buff)
{
    unsigned char temp_str[12], convert_str[10];
    strcpy(temp_str, p_buff);

    unsigned i, j;
    j= 0;
    for(i=0; i<12;i++)
    {
        if((temp_str[i] != '.') && (temp_str[i] != '-') && (temp_str[i] != '\0'))
        {

               convert_str[j] =  temp_str[i];
               j++;
        }  

    }
//    posicao_cursor_lcd(1,0);
//    escreve_frase_ram_lcd(convert_str);
//    __delay_ms(1000);
//    return strtol(p_buff, &convert, 10);
    return (atol(convert_str));
//    return atoi(p_buff);
}

/*Final do Arquivo modelo.c **************************************************/