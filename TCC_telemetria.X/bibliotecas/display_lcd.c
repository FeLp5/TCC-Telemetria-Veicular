/******************************************************************************
 * Nome do Arquivo 	: display.c
 *
 * Descri��o       	: Implementa controle do display HD44780 (16 x 2)
 *
 * Ambiente			: MPLAB v3.51, XC8 v2.32, PIC18F4620
 *
 * Respons�vel		: Torres, Weslley
 *
 * Vers�o/Data		: v01.00 - 08/09/2014 - Vers�o inicial
 *					  v01.01 - 22/09/2019 - Corrigido fun��o EscreveFloatLCD. 
 *					  v01.02 - 16/08/2020 - Ajustado de acordo com padr�o de 
 *											codifica��o
 *
 *****************************************************************************/


/******************************************************************************
* HEADER-FILES (Somente os arquivos necess�rios nesse arquivo)
******************************************************************************/
#include "hardware.h"
#include <stdlib.h>
#include "display_lcd.h"

/*****************************************************************************/
/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das fun��es
******************************************************************************/ 

/*****************************************************************************/

/******************************************************************************
 * Funcao:		void delay(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa atraso baseado no cilco de instru��o do uC
 *****************************************************************************/
void delay(void)
{
	unsigned char contador_ext, contador_int;

	for(contador_ext = 0; contador_ext<100; contador_ext++)
	{
        NOP();		
		for(contador_int = 0; contador_int<50; contador_int++)
		{
            NOP();
		}
	}
}
/******************************************************************************
 * Funcao:		void init_lcd(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Configura portas do PIC 18F4550 que est�o conectadas ao 
 *	display LCD como saida. Essa rotina � baseada no controlador de LCD 
 *  Hitachi HD44780. Configura os parametros de comunica��o com o display,
 *  tais como:
 *	- modo de opera��o (4 ou 8 data bits)
 *	- tipo de cursor
 *	- tamanho da fonte
 *	Ao final da configura��o limpa o display.
 *****************************************************************************/
void init_lcd(void)
{
	// Configura os pinos de controle (RW,RS,E) como sa�da.
	TRIS_RW = 0;
	TRIS_RS = 0;
	TRIS_E = 0;

	// Configura os pinos de dados (D4,D5,D6,D7) como sa�da.
	TRIS_D4=0;
	TRIS_D5=0;
	TRIS_D6=0;
	TRIS_D7=0;

// Inicio da rotina de inicializa��o do display LCD
// Aguardar tempos e enviar 3 vezes o comando 0b0011XXXX como descrito no datasheet
// do controlador HD44780 ou SPLC780D

	//Seleciona pinos de Controle para escrita do comando
	RW_PIN = 0;	//escrita=0, leitura=1
	RS_PIN = 0;		// comando=0, dados=1
	E_PIN =  0; 	// desabilita=0, habilita=1

	//selecioan pinos de dados com comando de inicializa��o 0b0011 no Nibble-High
	//(os 4 bits de dados mais significativos), os bits do Nibble-Low s�o Don't Care 
	D7=0;
	D6=0;
	D5=1;
	D4=1;

//Passo 1: Envio do comando de inicializa��o 0b0011

	// Delay inicial de 15ms ap�s alimentar o LCD: Vdd > 4,5V
	__delay_ms(15);

	//Habilita leitura do comando pelo LCD, atrav�s do pino de enable, para tanto,
	//um pulso de 1ms com borda de descida � efetuado
	E_PIN = 1;      			
    delay();
	E_PIN = 0;
	delay();

//Passo 2: Envio do comando de inicializa��o 0b0011

	// Delay de pelo menos 4.1ms
	__delay_ms(5);

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;      			
    delay();
	E_PIN = 0;
	delay();

//Passo 3: Envio do comando de inicializa��o 0b0011

	// Delay de pelo menos 100us
	__delay_ms(5);

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;      			
    delay();
	E_PIN = 0;
	delay();

//Passo 4: Envio da Configura��o 0b0010 no Nibble-High
	D7=0;
	D6=0;
	D5=1;
	D4=0;


	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;      			
    delay();
	E_PIN = 0;
	delay();


//Envio dos comandos de Configura��o do LCD
	escreve_cmd_lcd(0b00101000);	// Function Set: 4bit interface, 2 lines, Font 5x7  
	escreve_cmd_lcd(0b00000110);	// Entry mode set: Increment, Shift OFF
	escreve_cmd_lcd(0b00001110);	// Display Control: Display ON, Cursor ON, Blink OFF
	escreve_cmd_lcd(0b00000001);	// Clear display
}


/******************************************************************************
 * Funcao:		void escreve_cmd_lcd(unsigned char comando)
 * Entrada:		comando: comando que ser� enviado para o LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve um comando para o display de LCD.
 *****************************************************************************/
void escreve_cmd_lcd(unsigned char comando)
{
	//Seleciona pinos de Controle para escrita do comando
	RW_PIN = 0;		//escrita=0, leitura=1
	RS_PIN = 0;		// comando=0, dados=1
	E_PIN =  0; 	// desabilita=0, habilita=1

	//Seleciona pinos de Dados: Nibble-High
	if(comando&0b00010000){D4=1;}
	else {D4=0;}
	if(comando&0b00100000){D5=1;}
	else {D5=0;}
	if(comando&0b01000000){D6=1;}
	else {D6=0;}
	if(comando&0b10000000){D7=1;}
	else {D7=0;}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();

	//Seleciona pinos de Dados: Nibble-Low
	if(comando&0b00000001){D4=1;}
	else {D4=0;}
	if(comando&0b00000010){D5=1;}
	else {D5=0;}
	if(comando&0b00000100){D6=1;}
	else {D6=0;}
	if(comando&0b00001000){D7=1;}
	else {D7=0;}    

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();
	
	//Como os comandos Clear Display(0b00000001) e Cursor Home(0b0000001x),
	//demoram mais tempo para serem executados,cerca de cerca de 1,5ms.
	//Garante-se um atraso maior caso os mesmos sejam efetuados
	if (comando==0x01 || comando==0x02 || comando==0x03)
	{ delay(); }
}


/******************************************************************************
 * Funcao:		void endereco_cursor_lcd(unsigned char endereco)
 * Entrada:		endereco: endere�o da memoria de dados do display que se 
 *				colocar o cursor
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina posiciona o cursor no endere�o desejado da memoria 
 *				RAM do display LCD. Os comandos de escrita seguintes v�o 
 *				come�ar a partir da posi��o escolhida por esta fun��o
 *****************************************************************************/
void endereco_cursor_lcd(unsigned char endereco)
{
	//Seleciona pinos de Controle para escrita do comando
	RW_PIN = 0;	    //escrita=0, leitura=1
	RS_PIN = 0;		// comando=0, dados=1
	E_PIN =  0; 	// desabilita=0, habilita=1

	//Seleciona pinos de Dados: Nibble-High
	D7 = 1;	
	if(endereco&0b00010000){D4=1;}
	else {D4=0;}
	if(endereco&0b00100000){D5=1;}
	else {D5=0;}
	if(endereco&0b01000000){D6=1;}
	else {D6=0;}
	
	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();

	//Seleciona pinos de Dados: Nibble-Low
	if(endereco&0b00000001){D4=1;}
	else {D4=0;}
	if(endereco&0b00000010){D5=1;}
	else {D5=0;}
	if(endereco&0b00000100){D6=1;}
	else {D6=0;}
	if(endereco&0b00001000){D7=1;}
	else {D7=0;}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();
}


/******************************************************************************
 * Funcao:		void posicao_cursor_lcd(unsigned char linha, unsigned char 
 *				coluna)
 * Entrada:		linha: n�mero de 1 � 4 da linha em que se deseja deixar o 
 *				cursor
 *				coluna: n�mero de 1 � 16 coluna linha em que se deseja deixar 
 *				o cursor
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina posiciona o cursor na tela do display, na linha e 
 *				coluna desejados. Os comandos de escrita seguintes v�o come�ar
 *  			a partir da posi��o escolhida por esta fun��o
 *****************************************************************************/
void posicao_cursor_lcd(unsigned char linha, unsigned char coluna)
{
	unsigned char endereco = 0x00;
	
	if(coluna <= NUM_COLUNAS)
	{
		//Calcula o endere�o da mem�ria RAM do display que deve ser usado
		//para posicionar o cursor na linha e coluna desejados
		switch (linha)
		{
			case 1:
				endereco = LINHA_UM + coluna;
				endereco_cursor_lcd(endereco);		
			break;
		
			case 2:
				endereco = LINHA_DOIS + coluna;
				endereco_cursor_lcd(endereco);
			break;
			
			case 3:
				endereco = LINHA_TRES + coluna;
				endereco_cursor_lcd(endereco);		
			break;
		
			case 4:
				endereco = LINHA_QUATRO + coluna;
				endereco_cursor_lcd(endereco);
			break;
			
			default:
				endereco = LINHA_UM;
			break;
		}
        
        coluna++;
	}
	else
	{
		endereco = LINHA_UM;
	}
}


/******************************************************************************
 * Funcao:		void escreve_caractere_lcd(unsigned char data)
 * Entrada:		data: Byte de dados para ser escrito na memoria RAM do LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve o byte de caracteres no display a partir da posi��o
 *				de memoria atual do cursor ou da posi��o de mem�ria	escolhida 
 *				com as fun��es posicao_cursor_lcd() ou endereco_cursor_lcd().
 *****************************************************************************/
void escreve_caractere_lcd(char data)
{
	//Seleciona pinos de Controle para escrita do comando
	RW_PIN = 0;	//escrita=0, leitura=1
	RS_PIN = 1;		// comando=0, dados=1
	E_PIN =  0; 	// desabilita=0, habilita=1

	//Seleciona pinos de Dados: Nibble-High
	if(data&0b00010000){D4=1;}
	else {D4=0;}
	if(data&0b00100000){D5=1;}
	else {D5=0;}
	if(data&0b01000000){D6=1;}
	else {D6=0;}
	if(data&0b10000000){D7=1;}
	else {D7=0;}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();


	//Seleciona pinos de Dados: Nibble-Low
	if(data&0b00000001){D4=1;}
	else {D4=0;}
	if(data&0b00000010){D5=1;}
	else {D5=0;}
	if(data&0b00000100){D6=1;}
	else {D6=0;}
	if(data&0b00001000){D7=1;}
	else {D7=0;}    

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	E_PIN = 1;
    delay();
	E_PIN = 0;
	delay();
}


/******************************************************************************
 * Funcao:		void escreve_inteiro_lcd(int valor)
 * Entrada:		valor: numero inteiro com sinal (de -32768 a 32767) a ser 
 * 				escrito no display.
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina escreve o numero inteiro no display a partir da 
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria
 *				escolhida com as fun��es posicao_cursor_lcd() ou 
 *				endereco_cursor_lcd().
 *****************************************************************************/
void escreve_inteiro_lcd(int valor)
{
	char texto[7] = "000000";
    itoa(texto,valor, 10);
	escreve_frase_ram_lcd(texto);
}


/******************************************************************************
 * Funcao:		void escreve_float_lcd(float valor, char formatacao)
 * Entrada:		valor: numero float com sinal (de -9999.9999 a 9999.9999) que
 *				ser� escrito no display
 *				formatacao: numero de casas decimais desejadas ap�s a 
 *				virgula (de 0 � 4 casas decimais).
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve o numero float no display a partir da posi��o atual 
 *				do cursor com quantas casas decimais forem definidas no 
 *				par�metro formatacao.
 *****************************************************************************/
void escreve_float_lcd(float valor, char formatacao)
{
	int numero_inteiro = 0;		// Guarda parte inteira
	float numero_ponto_flutuante = 0; // Guarda parte fracion�ria
	unsigned char contador;				// contador do comando for
	
	// Escreve parte inteira
	numero_inteiro = (int)valor;
	escreve_inteiro_lcd(numero_inteiro);
	
	// Escreve as casas decimais indicada na formata��o
	if( formatacao > 0 )
	{
		escreve_caractere_lcd('.');
		// Separa parte fracion�ria
		numero_ponto_flutuante = (valor - numero_inteiro);
		// Se numero_ponto_flutuante � negativo, corrige o sinal
		numero_ponto_flutuante = (int) abs(numero_ponto_flutuante);
		
		// Garante que so escreve ate 5 casas decimais
		if(formatacao>4)
			{formatacao=4;}
		
		// Calcula as casas decimais
		for (contador = 0; contador<formatacao; contador++)
		{
			numero_ponto_flutuante *= 10;
			// Escreve zeros ap�s a virgula
			if(numero_ponto_flutuante<1)
			{
				escreve_caractere_lcd('0');
			}
		}
		// Escreve parte fracion�ria
		escreve_inteiro_lcd((int)numero_ponto_flutuante);		
	}
}

/******************************************************************************
 * Funcao:		void escreve_frase_ram_lcd(char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de dados(RAM)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina copia uma string terminada com caracter nulo
 *				da memoria de dados do microcontrolador para o display de LCD.
 *				A string apontada por *buffer � copiada no display a partir da
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria
 *  			escolhida com as fun��es posicao_cursor_lcd() ou 
 *   			endereco_cursor_lcd().
 *****************************************************************************/
void escreve_frase_ram_lcd(char *buffer)
{
	while(*buffer)		// escreve dados no LCD ate achar caracter nulo
	{
		escreve_caractere_lcd(*buffer); // escreve caracter no LCD
		buffer++;		// incrementa apontador
	}
}


/******************************************************************************
 * Funcao:		void escreve_frase_lcd(const char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de programa(ROM)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Copia uma string terminada com caracter nulo da memoria de 
 *				programa do microcontrolador para o display de LCD.
 *				A string apontada por *buffer � copiada no display a partir da
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria 
 *				escolhida com as fun��es posicao_cursor_lcd() ou 
 *				endereco_cursor_lcd().
 *****************************************************************************/
void escreve_frase_lcd(const char *buffer)
{
	while(*buffer)		// escreve dados no LCD ate achar caracter nulo
	{
		escreve_caractere_lcd(*buffer); // escreve caracter no LCD
		buffer++;		// incrementa apontador
	}
}


/******************************************************************************
 * Funcao:		void cria_caractere_lcd(unsigned char endereco, const char *buffer)
 * Entrada:		endereco: Numero de 0 � 7 que indica a posi��o da CGRAM que o 
 *				caractere ser� criado.
 *
 *				*buffer: apontador para o vetor de char com tamanho 8 que 
 *				guarda cada linha de desenho do caractere que ser� criado.
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:   Essa rotina cria um caractere de usu�rio em uma das 7 posi��es
 *				disponiveis na CGRAM do display. O caractere criado tem tamanho 
 *				8x5 pixel (8 linhas e 5 colunas), portanto cada valor do vetor 
 *				buffer representa uma linha, onde os 3 bits mais significativos
 *				de cada valor � desprezado.
 *****************************************************************************/
void cria_caractere_lcd(unsigned char endereco, const char *buffer)
{
	unsigned char comando;		//guarda comando a ser enviado para LCD
	unsigned char contador = 0;	//contador do comando for

	//Dependendo do valor de endereco, seleciona o comando correto
	//Garante que enderco s� tenha valores de 0 � 7
	endereco = endereco & 0b00000111;
	
	switch (endereco)
	{	case 0: comando = 0b01000000;	break;
		case 1: comando = 0b01001000;	break;
		case 2: comando = 0b01010000;	break;
		case 3: comando = 0b01011000;	break;
		case 4: comando = 0b01100000;	break;
		case 5: comando = 0b01101000;	break;
		case 6: comando = 0b01110000;	break;
		case 7: comando = 0b01111000;	break;
		default:comando = 0b01000000;	break;
	}	
	//Comando de escrita na CGRAM (cria��o de caratere personalizado)
	escreve_cmd_lcd(comando);
	
	//Escreve cada linha do caractere com a sequencia de valores do buffer
	for(contador=0; contador<8; contador++)
	{
		escreve_caractere_lcd(*buffer);	// escreve caracter no LCD
	    buffer++;	// incrementa apontador
	}
	
	//Comando para sair do modo cria��o de caratere e posicionar cursor
	//na linha 1, coluna 1 da tela.
	endereco_cursor_lcd(0x00);	
}

/******************************************************************************
 * Funcao:		void itoa(unsigned int val, char* str )
 * Entrada:		val: 
 *
 *				*str: 
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:   Essa rotina .
 *****************************************************************************/
void converte_inteiro_matriz(unsigned int val, char* str )
{
  str[0]=(val/10000)+0x30;  
  str[1]=((val%10000)/1000)+0x30;  
  str[2]=((val%1000)/100)+0x30;  
  str[3]=((val%100)/10)+0x30;
  str[4]=(val%10)+0x30;
  str[5]=0;
}