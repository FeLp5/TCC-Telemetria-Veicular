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
/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/


const unsigned char fence_diff_lat[4][11]  = {"-2339.49579", "-2339.49579", "-2339.92237", "-2339.92237"};
const unsigned char fence_diff_long[4][12] = {"-04632.23318", "-04631.75907", "-04631.75907", "-04632.23318"};

char temp_buff_lat[11]; //=  "-2342.09112";
char temp_buff_long[12];// = "-04629.73479";

fence_ext_struct poligono_ext[2];
//bit_field flag[3];
/*****************************************************************************/

/******************************************************************************
* Prototipos das funções
******************************************************************************/

long int diff_calc(unsigned char *p_buff_fence, unsigned char *p_buff_point);
/*****************************************************************************/

/******************************************************************************
 * Funcao:		void armazena_longitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void armazena_longitude(void)
{
    strcpy(temp_buff_long, Longitude());
}


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void armazena_latitude(void)
{
    strcpy(temp_buff_lat, Latitude());
}


/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void verifica_diferenca_graus(unsigned char select_data)
{
    
    unsigned char point, point_index_lat ,point_index_long;
    unsigned char i, j;
    point = 0;
    point_index_long = 0;
    point_index_lat  = 0;
    
    switch(select_data)
    {        
        case 1:
            while(point<4)
            {
                for(i=1; i<5; i++)
                {
                    if(point == 0 || point == 2)
                    {

                        if(point == 2 && !point_index_lat)
                        {
                            point_index_lat++;
                        }


                        for(j= 0; i < 5; j++)
                        {
                            poligono_ext[point_index_lat].point_lat_deg[j] = temp_buff_lat[i];  
                            poligono_ext[point_index_lat].fence_lat_deg[j] = fence_diff_lat[point][i]; 
                            i++;
                        }

                    }
                    else
                    {
                        break;
                    }
                }
                point++;
            }
        break;  
        
        case 0:
            while(point<4)
            {
                for(i=1; i<6; i++)
                {
                    if(point == 0 || point == 2)
                    {      
                        if(point == 2 && !point_index_long)
                        {
                            point_index_long++;
                        }
                        for(j= 0; i < 6; j++)
                        {
                            poligono_ext[point_index_long].point_long_deg[j] = temp_buff_long[i];  
                            poligono_ext[point_index_long].fence_long_deg[j] = fence_diff_long[point][i]; 

                            i++;
                        }

                    }
                    else
                    {
                        break;
                    }

                }

                point++;
            }
        break;
        

        
    }
    
    
}

/******************************************************************************
 * Funcao:		void leitura_chave(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura da chave em RC0
 *****************************************************************************/
void calcula_diff_graus(unsigned char select_data)
{
    unsigned char point, count;
//    long int temp_var_deg;
    
    point = 0;
    count = 0;
    
    switch(select_data)
    {
        case 1:
                while(point<4)
                {
                    if(point == 0 || point == 2)
                    {
                        
//                        poligono_ext[count].diff_lat_deg = atoi()
                        
                        poligono_ext[count].diff_lat_deg = diff_calc(poligono_ext[count].fence_lat_deg, poligono_ext[count].point_lat_deg);
//                        temp_var_deg = atoi(poligono_ext[count].point_lat_deg);
//                        poligono_ext[count].diff_lat_deg = atoi(poligono_ext[count].fence_lat_deg);
//                        poligono_ext[count].diff_lat_deg = poligono_ext[count].diff_lat_deg - temp_var_deg;            

                        count++;
                    }
                    point++;
                }
        break;
        
        
        
        case 0:
                while(point<4)
                {
                    if(point == 0 || point == 2)
                    {     
                        poligono_ext[count].diff_long_deg = diff_calc(poligono_ext[count].fence_long_deg, poligono_ext[count].point_long_deg);
//                        temp_var_deg = atoi(poligono_ext[count].point_long_deg);
//                        poligono_ext[count].diff_long_deg =  atoi(poligono_ext[count].fence_long_deg);
//                        poligono_ext[count].diff_long_deg = poligono_ext[count].diff_long_deg - temp_var_deg; 

                        count++;
                    }
                    point++;
                }
        break;
    }
    
    
    

    
}



/******************************************************************************
 * Funcao:		void armazena_latitude(void)
 * Entrada:		Nenhuma (void)
 * Saída:		Nenhuma (void)
 * Descrição:	Armazena longitude no buffers
 *****************************************************************************/
void verifica_diferenca_minutos(unsigned char select_data)
{
    unsigned char point, point_index_lat ,point_index_long;
    unsigned char i, j;
    
    point_index_lat = 0;
    point_index_long = 0;
    point = 0;
    switch(select_data)
    {
        case 1:
            while(point<4)
            {
                for(i=6; i<11; i++)
                {
                    if(point == 0 || point == 2)
                    {
                        if(point == 2 && !point_index_lat)
                        {
                            point_index_lat++;
                        }
//                        if(fence_diff_lat[point][i] != temp_buff_lat[i])
//                        {
                        for(j= 0; i < 11; j++)
                        {
                            poligono_ext[point_index_lat].fence_lat_min[j] = fence_diff_lat[point][i];
                            poligono_ext[point_index_lat].point_lat_min[j] = temp_buff_lat[i];
                                i++;
                        }

//                        }

                    }
                    else
                    {
//                       i = 11;
                        break;
                    }    
                }
                point++;
            }
            break;
            
        case 0:
            while(point<4)
            {
                for(i=7; i<12; i++)
                {
                    if(point == 0 || point == 2)
                    {
                        if(point == 2 && !point_index_long)
                        {
                            point_index_long++;
                        }


//                        if(fence_diff_long[point][i] != temp_buff_long[i])
//                        {
                            for(j= 0; i < 12; j++)
                            {
                                poligono_ext[point_index_long].fence_long_min[j] = fence_diff_long[point][i];
                                poligono_ext[point_index_long].point_long_min[j] = temp_buff_long[i];
                                i++;
                            }
//                        }
                    }
                    else
                    {
//                        i = 11;
                        break;
                    }

                }
                point++;
            }
            break;     
    }  
}


/******************************************************************************
 * Funcao:		void leitura_chave(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a leitura da chave em RC0
 *****************************************************************************/
void calcula_diff_minutos(unsigned char select_data)
{
    unsigned char point, count;
//    unsigned long int temp_var_deg;
    
    switch(select_data)
    {
        case 1:
            while(point<4)
            {
                if(point == 0 || point == 2)
                {
                    poligono_ext[count].diff_lat_min = diff_calc(poligono_ext[count].fence_lat_min, poligono_ext[count].point_lat_min );
//                    poligono_ext[count].diff_lat_min = (atoi(poligono_ext[count].fence_lat_min)) - (atoi(poligono_ext[count].point_lat_min));
                    count++;
                }
                point++;
            }
        break;
        
        case 0:
            while(point<4)
            {
                if(point == 0 || point == 2)
                {
                    poligono_ext[count].diff_long_min =diff_calc(poligono_ext[count].fence_long_min, poligono_ext[count].point_long_min );
//                    poligono_ext[count].diff_long_min = (atoi(poligono_ext[count].fence_long_min)) - (atoi(poligono_ext[count].point_long_min));
                    count++;
                }
                point++;
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
unsigned char verifica_plausibilidade_long(void)
{
    switch(temp_buff_long[0])
    {
        case '-':
            if(
                ((poligono_ext[0].diff_long_min >= 0  &&
               poligono_ext[1].diff_long_min <= 0)  &&
                    
               (poligono_ext[0].diff_long_deg >= 0  &&
               poligono_ext[1].diff_long_deg <= 0)))
            {
                return 0;
            }
            else if((poligono_ext[0].diff_long_min == 0 ||
                    poligono_ext[1].diff_long_min == 0 ) &&
                    
                    
                    (poligono_ext[0].diff_long_deg == 0 ||
                    poligono_ext[1].diff_long_deg == 0))
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
               (poligono_ext[0].diff_long_min <= 0  &&
               poligono_ext[1].diff_long_min >= 0 )  &&
              (poligono_ext[0].diff_lat_deg <= 0  &&  
               poligono_ext[1].diff_lat_deg >= 0))
            {
                return 0;
            }
            else if((poligono_ext[0].diff_long_min == 0 ||
                    poligono_ext[1].diff_long_min == 0) &&
                    (poligono_ext[0].diff_long_deg == 0 ||
                    poligono_ext[1].diff_long_deg == 0))
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
    switch(temp_buff_lat[0])
    {
        case '-':
            if(
              (poligono_ext[0].diff_lat_min <= 0  &&
               poligono_ext[1].diff_lat_min >= 0)  &&

               (poligono_ext[0].diff_lat_deg <= 0  &&  
               poligono_ext[1].diff_lat_deg >= 0))  
            {
                
                return 0;
            }
            else if(
                    (poligono_ext[0].diff_lat_min == 0 ||
                    poligono_ext[1].diff_lat_min == 0) &&
                    (poligono_ext[0].diff_lat_deg == 0 ||
                    poligono_ext[1].diff_lat_deg == 0))
            {
                
                return 0;
            }
            else
            {
                return 1;
            }
        break;
        
        default:
            if((poligono_ext[0].diff_lat_min >= 0  &&
               poligono_ext[1].diff_lat_min <= 0 ) &&
                    
                    
               (poligono_ext[0].diff_lat_deg >= 0   &&
               poligono_ext[1].diff_lat_deg <= 0)     
             )
            {
                return 0;
            }
            else if((poligono_ext[0].diff_lat_min == 0 ||
                    poligono_ext[1].diff_lat_min == 0) &&
                    
                    (poligono_ext[0].diff_lat_deg == 0 ||
                    poligono_ext[1].diff_lat_deg == 0))
            {
                return 0;
            }
            else
            {
//                posicao_cursor_lcd(2,0);
//                escreve_frase_ram_lcd("SWITCH3");
                return 1;
            }
        break;
                
    }
    

}


long int diff_calc(unsigned char *p_buff_fence, unsigned char *p_buff_point)
{
    unsigned char fence[12];
    unsigned char point[12];
    
    strcpy(fence, p_buff_fence);
    strcpy(point, p_buff_point);
    return ((atoi(fence))-(atoi(point)));
}


/*Final do Arquivo modelo.c **************************************************/