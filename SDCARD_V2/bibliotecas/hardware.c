/******************************************************************************
 * Nome do Arquivo 	: hardware.c
 *
 * Descricao       	: Implementa funcoes de acesso ao harware do uC
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsavel		: Torres, Weslley
 *
 * Versao/Data		: v00.01 - 16/08/2020 - versao inicial 
 *
 *****************************************************************************/

/******************************************************************************
* HEADER-FILES (Somente os arquivos necessarios nesse arquivo)
******************************************************************************/

#include "hardware.h"
#include "adc.h"
//#include "biblioteca/display_lcd.h"

/*****************************************************************************/

/******************************************************************************
* Variaveis Globais
******************************************************************************/

/* used to hold 16-bit timer value */
union Timers
{
  unsigned int lt;
  char bt[2];
};

/*****************************************************************************/

/******************************************************************************
* Prototipos das funcoes
******************************************************************************/
void config_timer_zero(void);
void config_timer_dois(void);
void init_interrupt(void);
void init_ports(void);
void init_oscilador(void);

/*****************************************************************************/

/******************************************************************************
 * Funcao:		void init_hardware(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao dos perifericos do uC
 *****************************************************************************/
void init_hardware(void)
{
    /* Inicializa oscilador interno do microcontrolador */
    init_oscilador();
    
    /* Inicializa portas do microcontrolador */
    init_ports();
    
    /* Configura Timer0 */
	config_timer_zero();
    
    /* Configura Interrupcoes do microcontrolador */
	init_interrupt();
    
    /* Inicializa conversor AD*/
    inicializa_adc();
}


 /******************************************************************************
 * Funcao:		void config_timer_zero(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao dos perifericos do uC do timer 0
 *****************************************************************************/
void config_timer_zero(void)
{
    T0CON   = 0b11000011;
    SPPCON  = 0b00000000;
    SPPCFG  = 0b00000000;
	write_timer_zero(0x0083);

}

 /******************************************************************************
 * Funcao:		void init_interrupt(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao dos perifericos do uC da interrupcao
 *****************************************************************************/
void init_interrupt(void)
{
	INTCON  = 0b11110000;
    INTCON2 = 0b00000000;
    INTCON3 = 0b00000000;	
	PIE1    = 0b10100000;   //SSPIE Enable
}

 /******************************************************************************
 * Funcao:		void open_timer0(unsigned char config)
 * Entrada:		unsigned char config - 0/1 to disable/ enable timer0
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a habilitação do periferico Timer0
 *****************************************************************************/
void open_timer0(unsigned char config)
{
    T0CON = (0x7f & config);  // Configure timer, but don't start it yet
    TMR0H = 0;                // Reset Timer0 to 0x0000
    TMR0L = 0;
    INTCONbits.TMR0IF = 0;      // Clear Timer0 overflow flag

    if(config&0x80)           // If interrupts enabled
    {
        INTCONbits.TMR0IE = 1;    // Enable Timer0 overflow interrupt
    }
    else
    {
        INTCONbits.TMR0IE = 0;
    }

    T0CONbits.TMR0ON = 1;
}

 /******************************************************************************
 * Funcao:		void write_timer_zero(unsigned int timer0)
 * Entrada:		unsigned int timer0 - valor a ser setado no periferico timer0
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a escrita no periferico Timer0
 *****************************************************************************/
void write_timer_zero(unsigned int timer0)
{
  union Timers timer;

  timer.lt = timer0;    // Copy timer value into union

  TMR0H = timer.bt[1];  // Write high byte to Timer0
  TMR0L = timer.bt[0];  // Write low byte to Timer0
}

 /******************************************************************************
 * Funcao:		void init_ports(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a inicializacao das portas do microcontrolador
 *****************************************************************************/
void init_ports(void)
{
    TRISB = 0b00000001;            
	LATB  = 0x00;
    TRISD = 0x00;           
    TRISA = 0b00000111; 
    TRISC = 0x01111111;
    TRISE = 0b00000011;

}

 /******************************************************************************
 * Funcao:		void init_oscilador(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao do oscilador interno do uC
 *****************************************************************************/
void init_oscilador(void)
{
    /* Oscilador Interno de 8MHz */
	OSCCONbits.IRCF0 = 1;
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF2 = 1;
}

 /******************************************************************************
 * Funcao:		void ms_delay(unsigned int val)
 * Entrada:		unsigned int val - tempo de espera
 * Saida:		Nenhuma (void)
 * Descricao:	Funcao de espera por hardware
 *****************************************************************************/
void ms_delay(unsigned int val)
{
 unsigned int i,j;
 for(i=0;i<val;i++)
     for(j=0;j<165;j++);  /*This count Provide delay of 1 ms for 8MHz Frequency */
 }



 /******************************************************************************
 * Funcao:		void config_timer_dois(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao dos perifericos do uC do timer 2
 *****************************************************************************/
void config_timer_dois(void)
{
    T2CON = 0b00000111;
    TMR2 = 0;
    PR2 = 0xFF;
}

 /******************************************************************************
 * Funcao:		void timer_dois(void)
 * Entrada:		Nenhuma (void)
 * Saida:		Nenhuma (void)
 * Descricao:	Realiza a configuracao dos perifericos do uC do timer 2
 *****************************************************************************/
void timer_dois(void)
{
    if(TMR2IF)
    {
        TMR2IF = 0;
        TMR2 = 0;
//        posicao_cursor_lcd(2,1);
//        escreve_frase_ram_lcd("olá");
        PORTBbits.RB4 = ~PORTBbits.RB4;

    }
}

/* Fim do arquivo hardware.c */