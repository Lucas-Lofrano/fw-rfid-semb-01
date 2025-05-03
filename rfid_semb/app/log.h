/*
 * log.h
 *
 *  Created on: May 2, 2025
 *      Author: lofrano
 */

// log.h
#pragma once

// Explicit forward declaration with packed attribute
typedef struct __attribute__((packed)) log_entry_t log_entry_t;

// Function prototypes
int log_init(void);
int log_append(uint32_t tag_id, char op);
