 /******************************************************************************
 * Nome do Arquivo 	: uart.c
 *
 * Descri��o       	: Implementa interfaces para comunica��o serial 
 *
 * Ambiente			: MPLAB, XC8 vers�o 1.45, PIC18F4550
 *
 * Respons�vel		: Torres, Weslley
 *
 * Vers�o/Data		: v00.01 - 09/05/2017 - vers�o inicial 
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necess�rios nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include <stdlib.h>
#include "uart.h"

/*****************************************************************************/


/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das fun��es
******************************************************************************/
char configura_baudrate_uart(const long int baudrate);

/*****************************************************************************/


/******************************************************************************
 * Funcao:		char configura_baudrate_uart(const long int baudrate)
 * Entrada:	   (const long int baudrate)
 * Sa�da:	   (char configura_baudrate_uart)
 * Descri��o:	Configura alguns registradore do PIC 18F4550 referentes 
 *	a comunica��o UART. Configura os parametros de comunica��o tais com:
 *	- modo de opera��o
 *	- Taxa de transfer�ncia
 *	Ao final retorna (1) caso tenha sucesso na inicializa��o, ou (0) se houver falha na inicializa��o.
 *****************************************************************************/
char configura_baudrate_uart(const long int baudrate)
{
    unsigned int x;
    
	x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
    if(x > 255)
    {
        x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
        BRGH = 1;
    }
    if(x < 256)
    {
        SPBRG = x;
        SYNC =  0;
        SPEN =  1;
        PINO_RX = 1;
        PINO_TX = 0;
        CREN =  1;
        TXEN =  1;
        
        return (1);
    }
    return (0);
	
}


/******************************************************************************
 * Funcao:		void envia_dado_uart(char data)
 * Entrada:		(char data) : Caractere que ser� enviado
 * Sa�da:		Nenhuma (void)
 * Descri��o:	(Envia Um caractere no UART).
 *****************************************************************************/
void envia_dado_uart(char data)
{
    while(!PIR1bits.TXIF);
    TXREG = data;
}


/******************************************************************************
 * Funcao:		unsigned char verifica_buffer_uart(void);
 * Entrada:		Nenhuma (void)
 * Sa�da:		(char)
 * Descri��o:	(Verifica o Registro de Transmiss�o).
 *****************************************************************************/
unsigned char verifica_buffer_uart(void)
{
    return (TRMT);
}


/******************************************************************************
 * Funcao:		void envia_string_uart(const char *string)
 * Entrada:		*string: Ponteiro que direciona o endere�o dos caracteres de uma string
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Envia uma string na comunica��o UART.
 *****************************************************************************/
void envia_string_uart(const char *string)
{
    while(*string)
    {
        envia_dado_uart(*string++);
    }
}


/******************************************************************************
 * Funcao:		char verifica_dado_disponivel_uart(void);
 * Entrada:		Nenhuma (void)
 * Sa�da:		(char)
 * Descri��o:	(Verifica se os dados est�o prontos para a leitura)
 * Utiliza o bit de sinaliza��o(Flag) RCIF que ser� definido quando a recep��o
 * de dados for conclu�da.
 *****************************************************************************/
char verifica_dado_disponivel_uart(void)
{
    return (RCIF);
}


/******************************************************************************
 * Funcao:		char recebe_dado_uart(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		(char)
 * Descri��o:	Essa rotina espera at� que a recep��o esteja completa e em seguida
 * l� os dados de "8 bits" do registrador RCREG.
 *****************************************************************************/
char recebe_dado_uart(void)
{
    while(RCIF==0);      /*wait for receive interrupt flag*/
    if(RCSTAbits.OERR)
    {           
        CREN = 0;
        NOP();
        CREN=1;
    }
    return(RCREG);       /*received in RCREG register and return to main program */
}



/******************************************************************************
 * Funcao:		void recebe_string_uart(char *buffer, unsigned int length)
 * Entrada:		char *buffer: Ponteiro que direciona para os endere�os dos caracteres
 *                            da string recebida.
 *              unsigned int length: Tamanho da string que foi recebida.
 * Sa�da:	    Nenhuma (void)
 * Descri��o:	Essa rotina l� uma string a partir do endere�o do primeiro caractere
 *              e de seu tamanho.
 *****************************************************************************/
void recebe_string_uart(char *buffer, unsigned char length)
{
    unsigned int i;
    char aux;
    for(i = 0; i < length; i++)
    {
        aux = recebe_dado_uart();
        
        if(aux == 0x0D)
        {
            for(i; i < length; i++)
            {
              buffer[i] = ' ';
            }
            break;
        }
        buffer[i] = aux;
        
    }
}


/******************************************************************************
 * Funcao:		void escreve_inteiro_uart(int valor)
 * Entrada:		int valor: tipo inteiro sinalizado.
 * Sa�da:	    Nenhuma (void)
 * Descri��o:	Essa rotina envia um valor tipo signed int (de -32768 a 32767)
 *  pela comunica��o UART, Sendo que este valor � convertido para Unsigned char .
 *****************************************************************************/
void escreve_inteiro_uart(int valor)
{
    unsigned char valor_final[7] = "000000";
    
    itoa(valor_final,valor, 10); 
    envia_string_uart(valor_final);  //Escreve o valor j� convertido
    envia_string_uart("\r\n");       //Quebra de linha
    
}

/******************************************************************************
 * Funcao:		void Init_UART(void)
 * Entrada:		Nenhuma.
 * Sa�da:	    Nenhuma (void)
 * Descri��o:	Essa fun��o inicializa o periferico UART.
 *****************************************************************************/
void inicializa_uart(void)
{
	PINO_TX    = 0; 				// Registrador responsavel pelo envio de dados via UART.
    PINO_RX    = 0; 				// Registrador responsavel pelo recebimento de dados via UART.
	
	(void)configura_baudrate_uart(9600);                // Inicializa Comunica��o UART.
    
    TXSTA = 0x20;  	/* Enable Transmit(TX) */ 
    RCSTA = 0x90;  	/* Enable Receive(RX) & Serial */
}

/*********************************FIM****************************************/
