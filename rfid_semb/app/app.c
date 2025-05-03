/**
  ******************************************************************************
  * @file    app.c
  * @author  Pedro Aires Ferreira da Silva Arantes
  * 		 Luiza Mariel Leal Caetano de Souza
  * 		 Lucas Campos Lofrano
  * @brief   App HAL module for RFID Control Access.
  ******************************************************************************
**/

// Includes --------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "lfs.h"
#include "AT45DB021E.h"
#include "log.h"
#include "app.h"
#include "main.h"
#include "hw.h"
#include "rfid.h"

// External variables ----------------------------------------------------------
extern uint8_t MFRC522_CheckTag(uint32_t* id);
extern uint32_t tags_database[];
extern uint8_t tags_number;
extern uint8_t pos;

bool irq_rfid_flag = false;
bool config_rfid_flag = false;


// Functions -------------------------------------------------------------------
/*
 ===============================================================================
                    ##### Tag Manager Functions #####
 ===============================================================================
*/

// Checks if the tag is inserted in the database
uint8_t app_tag_check_registered(uint32_t tag)
{
	for (uint8_t i = 0 ; i < tags_number; i++)
	{
		if(tag == tags_database[i])
			return 1; // True
	}
	return 0;
}

// Checks if the tag is the tag manager
uint8_t app_is_tag_manager(uint32_t tag)
{
	return tag == TAG_MANAGER; // True
}


/*
 ===============================================================================
                 ##### RFID Configuration Functions #####
 ===============================================================================
*/

/* Tries to read tag manager for 15 seconds */
uint8_t app_rfid_read_tag_manager(void)
{
	uint8_t wait = 0;
	uint8_t status = 0;
	uint32_t tag_manager_ID = 0;
	while((wait <= 15) || !(status))
	{
		status = MFRC522_CheckTag(&tag_manager_ID); // Checa se tem tag, salva o num na variavel, retorna true pro status
		hw_delay_ms(1000);
		wait++;
	}
	if(app_is_tag_manager(tag_manager_ID))
		return 1; // True
	else
		return 0;
}

/* Tries to read a tag for 15 seconds */
uint8_t app_rfid_insert_new_tag(void)
{
	uint8_t wait = 0;
	uint8_t status = 0;
	uint32_t new_tag_ID = 0;
	while((wait <= 15) | !(status))
	{
		status = MFRC522_CheckTag(&new_tag_ID);
		hw_delay_ms(1000);
		wait++;
	}
	if(new_tag_ID)
	{
		tags_database[tags_number] = new_tag_ID;
		log_append(new_tag_ID, 'R');
		tags_number++;
		return 1;
	}
	return 0;
}

// Enter configuration mode
void app_config_mode(void)
{
	static uint32_t deboucing_time_ms = 0;
	uint8_t status = 0;
	if((hw_tick_ms_get() - deboucing_time_ms) >= APP_DEBOUNCING_TIME_MS)
	{
		pos = 0;
		hw_timer_start();
		status = app_rfid_read_tag_manager();
		if(!status) // Se tag nao eh manager
		{
			hw_timer_stop();
			hw_led_write(LED_BLUE_Pin, 1);
			hw_delay_ms(2000);
			hw_led_write(LED_BLUE_Pin, 0);
			return;
		}
		pos = 1;
		status = app_rfid_insert_new_tag();
		hw_timer_stop();
		if(!status) // Se nao salvou tag nova
		{
			hw_led_write(LED_BLUE_Pin, 1);
			hw_delay_ms(2000);
			hw_led_write(LED_BLUE_Pin, 0);
		}
		else // Se salvou tag nova
		{
			hw_led_write(LED_GREEN_Pin, 1);
			hw_delay_ms(2000);
			hw_led_write(LED_GREEN_Pin, 0);
		}
		deboucing_time_ms = hw_tick_ms_get();
	}
}


/*
 ===============================================================================
                         ##### Access Functions #####
 ===============================================================================
*/

// Open the door
void app_door_open(void)
{
	hw_led_write(LED_GREEN_Pin, 1);
	hw_servo_unlock();
	hw_led_write(LED_GREEN_Pin, 0);
}

// Access Denied
void app_door_access_denied(void)
{
	hw_led_write(LED_BLUE_Pin, 1);
	hw_delay_ms(1500);
	hw_led_write(LED_BLUE_Pin, 0);
}

// Control access
void app_rfid_checks_tag(void)
{
	static uint32_t debouncing_time_ms = 0;

    // Ignorar leituras muito rápidas (debounce)
	if(hw_tick_ms_get() - debouncing_time_ms < APP_DEBOUNCING_TIME_MS) {
        return;
    }

    uint32_t tagID;
	if(MFRC522_CheckTag(&tagID)) // Se tem uma tag...
	{

		if(app_tag_check_registered(tagID))
		{
			int status = log_append(tagID, 'E');
			UNUSED(status);
			app_door_open();
		}
		else
		{
			app_door_access_denied();
		}

	}
	 debouncing_time_ms = hw_tick_ms_get();
}


/*
 ===============================================================================
                    ##### Program Startup Functions #####
 ===============================================================================
*/
// App entry point
void app_init(void)
{
	int status = log_init();
	UNUSED(status);

    MFRC522_Init();
    hw_servo_init();
    hw_servo_set_position(SERVO_CLOSED); // Garantir posição inicial como fechada
}

void app_loop(void)
{
	if (irq_rfid_flag)
	{
		app_rfid_checks_tag();
		irq_rfid_flag = !irq_rfid_flag;
	}

	if (config_rfid_flag)
	{
		app_config_mode();
		config_rfid_flag = !config_rfid_flag;
	}
}
