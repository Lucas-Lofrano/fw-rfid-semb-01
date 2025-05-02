/**
  ******************************************************************************
  * @file    database.c
  * @author  Luiza Mariel Leal Caetano de Souza
  * 		 Pedro Aires Ferreira da Silva Arantes
  * @brief   Database of the RFID Tags.
  ******************************************************************************
**/

// Includes --------------------------------------------------------------------
#include <stdint.h>
#include "app.h"

// Database --------------------------------------------------------------------
uint8_t tags_number = 15;
uint32_t tags_database[MAX_QTD_TAGS] =
{
	0x7A65FCD2, // Tag Manager
	0x98B1E309,
	0x2D4FA8C7,
	0x1B9764A5,
	0x5E83B6FD,
	0xAFD02E81,
	0xC74DE5B9,
	0x309AC63F,
	0xE7F912D8,
	0x86BD7A03,
	0x58A31F6E,
	0x412C8DB6,
	0xF62EB1C4,
	0xD8FA3E5D,
	0x9E07C41A
};
