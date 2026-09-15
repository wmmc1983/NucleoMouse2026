/*
 * flash.h
 *
 *  Created on: 2026/07/04
 *      Author: Karasawa
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#define EEPROM_START_ADDRESS  (uint32_t)0x0800F800  // EEPROM emulation start address: Page 31

HAL_StatusTypeDef FlashEnableWrite(void);

HAL_StatusTypeDef FlashDisableWrite(void);

HAL_StatusTypeDef FlashWriteHalfword(uint32_t, uint16_t);

HAL_StatusTypeDef FlashWriteWord(uint32_t, uint32_t);

uint16_t FlashReadHalfword(uint32_t);

uint32_t FlashReadWord(uint32_t);


#endif /* INC_FLASH_H_ */
