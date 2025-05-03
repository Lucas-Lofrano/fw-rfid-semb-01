/*
 * AT45DB021E.h
 *
 *  Created on: Apr 29, 2025
 *      Author: lofrano
 */

#ifndef APP_AT45DB021E_H_
#define APP_AT45DB021E_H_

#include "lfs.h"
#include "stdbool.h"
#include "stdint.h"

/*---------------------------------------------------------- LITTLE FILESYSTEM DEFINES ---------------------------------------------------------*/
#define READ_SIZE              1
#define PROG_SIZE              1
#define PAGE_SIZE            256
#define BLOCK_COUNT         1024	//num pages
#define BLOCK_CYCLES         500
#define CACHE_SIZE           256
#define LOOKAHEAD_SIZE        16
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------- AT45DB021E DEFINES -------------------------------------------------------------*/
#define MAX_TIMEOUT_BYTE_PROG_MS	     1
#define MAX_TIMEOUT_PAGE_PROG_MS	     3
#define MAX_TIMEOUT_PAGE_ERASE_MS	    25
#define MAX_TIMEOUT_BLOCK_ERASE_MS	    35

#define AT45DB_OP_READ_MAIN           0xD2
#define AT45DB_OP_WRITE_MAIN          0x82
#define AT45DB_OP_ERASE_PAGE          0x81

#define AT45DB_OP_READ_SR	 		  0xD7

#define AT45DB_OP_WP_ENABLE_DISABLE_0 0x3D
#define AT45DB_OP_WP_ENABLE_DISABLE_1 0x2A
#define AT45DB_OP_WP_ENABLE_DISABLE_2 0x7F
#define AT45DB_OP_WP_ENABLE_DISABLE_3 0x9A

#define AT45DB_OP_PAGE_BIN_CONF_0     0x3D
#define AT45DB_OP_PAGE_BIN_CONF_1     0x2A
#define AT45DB_OP_PAGE_BIN_CONF_2     0x80
#define AT45DB_OP_PAGE_BIN_CONF_3     0xA6
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------- AT45DB021E FUNCTIONS -------------------------------------------------------------*/
void AT45DB_conf_bin(void);
bool AT45DB_is_sr_ready(void);
bool AT45DB_is_sector_protect_active(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------ LITTLE_FS FUNCTIONS -------------------------------------------------------------*/
int lfs_AT45DB_mount(lfs_t *lfs, const struct lfs_config *c);
int lfs_AT45DB_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lsf_AT45DB_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_AT45DB_erase(const struct lfs_config *c, lfs_block_t block);
int lfs_AT45DB_sync(const struct lfs_config *c);

static const struct lfs_config lfs_AT45DB_cfg = {
    .read  = lfs_AT45DB_read,
    .prog  = lsf_AT45DB_write,
    .erase = lfs_AT45DB_erase,
    .sync  = lfs_AT45DB_sync,

    .read_size      = READ_SIZE,     //Minimal unit that can be read         -> 1byte
    .prog_size      = PROG_SIZE,     //Minimal unit that can be written      -> 1byte
    .block_size     = PAGE_SIZE,     //Minimal unit that can be erased       -> 1page -> 256bytes
    .block_count    = BLOCK_COUNT,   //Total number of pages 		         -> 1024 pages
    .block_cycles   = BLOCK_CYCLES,  //Makes a more aggressive wear leveling -> parameter used in internal calculations
    .cache_size     = CACHE_SIZE,    //Cache used for internal lfs operations
    .lookahead_size = LOOKAHEAD_SIZE //Make lfs choose better where to place data (bitfield of the flash)
};
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* APP_AT45DB021E_H_ */
