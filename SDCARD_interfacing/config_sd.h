/******************************************************************************
 * Nome do Arquivo 	: chaves.h
 *
 * Descrição       	: Implementa a leitura de chaves do projeto
 *
 * Ambiente			: MPLAB, XC8 versao 1.45, PIC18F4550
 *
 * Responsável		: Weslley Torres
 *
 * Versão/Data		: 01.00 - 16/08/2021 - Versao inicial 
 *
 *****************************************************************************/


/******************************************************************************
* HEADER-FILES (Somente os arquivos necessÃ¡rios nesse arquivo)
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/
/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/
//
//#ifndef _CONFIG_SD_H
//#define _CONFIG_SD_H
//
//#ifdef _WIN32	/* FatFs development platform */
//
//#include <windows.h>
//#include <tchar.h>
//
//
//#else			/* Embedded platform */
//
//
//
///* These types must be 16-bit, 32-bit or larger integer */
//typedef int				INT;
//typedef unsigned int	UINT;
//
///* These types must be 8-bit integer */
//typedef char			CHAR;
//typedef unsigned char	UCHAR;
//typedef unsigned char	BYTE;
//
///* These types must be 16-bit integer */
//typedef short			SHORT;
//typedef unsigned short	USHORT;
//typedef unsigned short	WORD;
//typedef unsigned short	WCHAR;
//
///* These types must be 32-bit integer */
//typedef long			LONG;
//typedef unsigned long	ULONG;
//typedef unsigned long	DWORD;
//
//#endif
//
//#endif
/*****************************************************************************/

/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef CONFIG_SD
#define CONFIG_SD

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

#endif

#endif
