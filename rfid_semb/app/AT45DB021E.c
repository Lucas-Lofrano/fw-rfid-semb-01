/*
 * AT45DB021E.c
 *
 *  Created on: Apr 29, 2025
 *      Author: lofrano
 */

#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "AT45DB021E.h"

#define NUM_ELEMENTS(array) (sizeof(array) / sizeof((array)[0]))

extern SPI_HandleTypeDef hspi2;
static uint8_t page_buffer[PAGE_SIZE];

void ATAT45DB_nCS_start_transaction(void)
{
	HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_RESET);
}

void ATAT45DB_nCS_finish_transaction(void)
{
	HAL_GPIO_WritePin(nCS_FLASH_GPIO_Port, nCS_FLASH_Pin, GPIO_PIN_SET);
}

void AT45DB_write_protect_disable(bool b)
{
	HAL_GPIO_WritePin(nWP_FLASH_GPIO_Port, nWP_FLASH_Pin, (b ? (GPIO_PIN_SET) : (GPIO_PIN_RESET)));
}

/*------------------------------------------------------------ READ STATUS REGISTER ------------------------------------------------------------*/
uint16_t ATAT45DB_read_sr(void)
{
	uint8_t data[2] = {0xFF, 0xFF};
	uint8_t cmd[] = {
			AT45DB_OP_READ_SR
	};

		ATAT45DB_nCS_start_transaction();

		HAL_SPI_Transmit(&hspi2, cmd, NUM_ELEMENTS(cmd), MAX_TIMEOUT_BYTE_PROG_MS*NUM_ELEMENTS(cmd));
		HAL_SPI_Receive(&hspi2, data, NUM_ELEMENTS(data), MAX_TIMEOUT_BYTE_PROG_MS*NUM_ELEMENTS(data));

		ATAT45DB_nCS_finish_transaction();

		uint16_t res = (((uint16_t) data[0]) << 8) | ((uint16_t) data[1]);
		return res;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------- CONFIGURATION TO THE FLASH MEMORY -> PAGE = 256Bytes --------------------------------------------*/
void AT45DB_conf_bin(void)
{
	uint8_t cmd[] =
	{
			AT45DB_OP_PAGE_BIN_CONF_0,
			AT45DB_OP_PAGE_BIN_CONF_1,
			AT45DB_OP_PAGE_BIN_CONF_2,
			AT45DB_OP_PAGE_BIN_CONF_3
	};

	ATAT45DB_nCS_start_transaction();

	HAL_SPI_Transmit(&hspi2, cmd, NUM_ELEMENTS(cmd), MAX_TIMEOUT_BYTE_PROG_MS*NUM_ELEMENTS(cmd));

	ATAT45DB_nCS_finish_transaction();
}
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------- VERIFY IF SECTOR_PROTECT IS SET -----------------------------------------------------*/
bool AT45DB_is_sector_protect_active(void)
{
	uint16_t sr = ATAT45DB_read_sr();
	uint16_t mask = 0x0200;
	uint8_t active = ((sr & mask) >> 9);

	return (active == 0x01);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------- VERIFY IF READY BIT IS SET -------------------------------------------------------*/
bool AT45DB_is_sr_ready(void)
{
	uint16_t sr = ATAT45DB_read_sr();
	uint16_t mask = 0x8000;
	uint8_t ready = ((sr & mask) >> 15);

	return (ready == 0x01);
}
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------- LITTLE_FS FUNCTIONS ----------------------------------------------------------*/
int lfs_AT45DB_mount(lfs_t *lfs, const struct lfs_config *c)
{
	AT45DB_conf_bin();
	int err = lfs_mount(lfs, c);

	if (err)
	{
		err = lfs_format(lfs, c);
		if (err)
			return err;

		err = lfs_mount(lfs, c);
	}

	return err;
}

//For what i understand :
//Block is the address 								 -> Which page ?     (0 - 1023) 10bits
//Off is the offset in the Block i want to read from -> Byte in the page (0 - 255)  8bits
//Buffer is the data pointer
//Size is the number of bytes i want to read
int lfs_AT45DB_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	while (!AT45DB_is_sr_ready()) { }

	uint8_t dummy_value = 0xFF;

	//Main Memory Page Read
	// OP + (ADDR + ADDR + ADDR) + (DUMMY + DUMMY + DUMMY + DUMMY)
	// The following SCK receives the data
	uint8_t cmd[] =
	{
			AT45DB_OP_READ_MAIN,
			((uint8_t) ((block >> 8) & 0x03)),  // Start page address (2 bits) the rest are dummy values
			((uint8_t) (block        & 0xFF)),  // Final page address (8 bits)
			((uint8_t) (off          & 0xFF)),	// Which byte in the page ? (0 - 255)
			dummy_value,
			dummy_value,
			dummy_value,
			dummy_value
	};

	ATAT45DB_nCS_start_transaction();

	HAL_SPI_Transmit(&hspi2, cmd, NUM_ELEMENTS(cmd), NUM_ELEMENTS(cmd)*MAX_TIMEOUT_PAGE_PROG_MS);
	HAL_SPI_Receive(&hspi2, (uint8_t*) buffer, size, size*MAX_TIMEOUT_PAGE_PROG_MS);

	ATAT45DB_nCS_finish_transaction();

	return LFS_ERR_OK;
}

//The idea is that i can maintain the rest of the data in the page by reading it and writing all over again
//Use the opcode that has buit-in erase before write
int lsf_AT45DB_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	while (!AT45DB_is_sr_ready()) { }

	//Verify if the content that is going to be written is a whole page
	//if not i store the previous content and add the new one
	bool partial_write = ((off != 0) || (size != PAGE_SIZE));

	if (partial_write)
	{
		int status = lfs_AT45DB_read(c, block, 0, page_buffer, PAGE_SIZE);
		UNUSED(status);

		//write the data of 'buffer' in 'page_buffer' after the offset
		memcpy(page_buffer + off, buffer, size);

		buffer = page_buffer;
		off = 0;
		size = PAGE_SIZE;
	}

	//Main Memory Page Program through Buffer with Built-In Erase
	// OP + ADDR + ADDR + ADDR
	uint8_t cmd[] =
	{
			AT45DB_OP_WRITE_MAIN,
			((uint8_t) ((block >> 8) & 0x03)),  // Start page address (2 bits) the rest are dummy values
			((uint8_t) (block        & 0xFF)),  // Final page address (8 bits)
			((uint8_t) (off          & 0xFF)),	// Which byte in the page ? (0 - 255)
	};

	ATAT45DB_nCS_start_transaction();

	HAL_SPI_Transmit(&hspi2, cmd, NUM_ELEMENTS(cmd), MAX_TIMEOUT_PAGE_PROG_MS);
	HAL_SPI_Transmit(&hspi2, (uint8_t*) buffer, size, MAX_TIMEOUT_PAGE_PROG_MS);

	ATAT45DB_nCS_finish_transaction();

	while (!AT45DB_is_sr_ready()) { }

	return LFS_ERR_OK;
}


//Choose which page i want to erase (0 - 1023)
int lfs_AT45DB_erase(const struct lfs_config *c, lfs_block_t block)
{
	while (!AT45DB_is_sr_ready()) { }

	uint8_t dummy_value = 0xFF;

	//Page Erase
	// OP + ADDR + ADDR + DUMMY
	uint8_t cmd[] =
	{
			AT45DB_OP_ERASE_PAGE,
			((uint8_t) ((block >> 8) & 0x03)),  // Start page address (2 bits) the rest are dummy values
			((uint8_t) (block        & 0xFF)),  // Final page address (8 bits)
			dummy_value
	};

	ATAT45DB_nCS_start_transaction();

	HAL_SPI_Transmit(&hspi2, cmd, NUM_ELEMENTS(cmd), MAX_TIMEOUT_PAGE_ERASE_MS);

	ATAT45DB_nCS_finish_transaction();

	while (!AT45DB_is_sr_ready()) { }

	return LFS_ERR_OK;
}

//Useless function, just implemented to fill the configuration
int lfs_AT45DB_sync(const struct lfs_config *c)
{
	while (!AT45DB_is_sr_ready()) { }

	return LFS_ERR_OK;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
