/**
  ******************************************************************************
  * @file    app.h
  * @author  Pedro Aires Ferreira da Silva Arantes
  * 		 Luiza Mariel Leal Caetano de Souza
  * 		 Lucas Campos Lofrano
  * @brief   Header file of app HAL module for RFID Control Access.
  ******************************************************************************
**/

#ifndef APP_H_
#define APP_H_

#ifdef __cplusplus
extern "C"{
#endif

// Define  ---------------------------------------------------------------------
#define APP_DEBOUNCING_TIME_MS 1000
#define MAX_QTD_TAGS 25
#define TAG_MANAGER 0x7A65FCD2

// Functions -------------------------------------------------------------------
uint8_t app_tag_check_registered(uint32_t tag);
uint8_t app_is_tag_manager(uint32_t tag);
uint8_t app_rfid_read_tag_manager(void);
uint8_t app_rfid_insert_new_tag(void);

void app_rfid_checks_tag(void);
void app_config_mode(void);

void app_door_open(void);
void app_door_access_denied(void);

void app_init(void);
void app_loop(void);


#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */
