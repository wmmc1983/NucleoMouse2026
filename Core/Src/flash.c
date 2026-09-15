/*
 * flash.c
 *
 *  Created on: 2026/07/04
 *      Author: Karasawa
 */

#include "global.h"
#include "flash.h"

 //+++++++++++++++++++++++++++++++++++++++++++++++
 // FlashEnableWrite
 // eepromとして使うflashメモリ領域をeraseし，書き込みを有効にする
 // 引数：なし
 // 戻り値：問題が起こらなければHAL_OKを返す
 //+++++++++++++++++++++++++++++++++++++++++++++++
HAL_StatusTypeDef FlashEnableWrite(void) {
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = EEPROM_START_ADDRESS;
    EraseInitStruct.NbPages = 1;
    status = HAL_FLASH_Unlock();
    if (status != HAL_OK)
        return status;
    status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    return status;
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// FlashDisableWrite
// eepromとして使うflashメモリ領域への書き込みを無効にする
// 引数：なし
// 戻り値：問題が起こらなければHAL_OKを返す
//+++++++++++++++++++++++++++++++++++++++++++++++
HAL_StatusTypeDef FlashDisableWrite(void) { return HAL_FLASH_Lock(); }
//+++++++++++++++++++++++++++++++++++++++++++++++
// FlashWriteHalfword
// eepromとして使うflashメモリ領域へ2バイトの値を書き込む
// 引数：address ……
// eepromとして使うflashメモリ領域の先頭アドレスからのオフセット，data ……
// 書き込みたいuin16_t型データ 戻り値：問題が起こらなければHAL_OKを返す
//+++++++++++++++++++++++++++++++++++++++++++++++
HAL_StatusTypeDef FlashWriteHalfword(uint32_t address, uint16_t data) {
    HAL_StatusTypeDef status;
    address = address * 2 + EEPROM_START_ADDRESS;
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);
    return status;
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// FlashWriteWord
// eepromとして使うflashメモリ領域へ4バイトの値を書き込む
// 引数：address ……
// eepromとして使うflashメモリ領域の先頭アドレスからのオフセット，data ……
// 書き込みたいuin32_t型データ 戻り値：問題が起こらなければHAL_OKを返す
//+++++++++++++++++++++++++++++++++++++++++++++++
HAL_StatusTypeDef FlashWriteWord(uint32_t address, uint32_t data) {
    HAL_StatusTypeDef status;
    address = address * 4 + EEPROM_START_ADDRESS;
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
    return status;
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// FlashReadHalfword
// eepromとして使うflashメモリ領域から2バイトの値を読み込む
// 引数：address ……
// eepromとして使うflashメモリ領域の先頭アドレスからのオフセット
// 戻り値：読み込んだuin16_t型データ
//+++++++++++++++++++++++++++++++++++++++++++++++
uint16_t FlashReadHalfword(uint32_t address) {
    uint16_t val = 0;
    address = address * 2 + EEPROM_START_ADDRESS;
    val = *(__IO uint16_t*)address;
    return val;
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// FlashReadWord
// eepromとして使うflashメモリ領域から4バイトの値を読み込む
// 引数：address ……
// eepromとして使うflashメモリ領域の先頭アドレスからのオフセット
// 戻り値：読み込んだuin32_t型データ
//+++++++++++++++++++++++++++++++++++++++++++++++
uint32_t FlashReadWord(uint32_t address) {
    uint32_t val = 0;
    address = address * 4 + EEPROM_START_ADDRESS;
    val = *(__IO uint32_t*)address;
    return val;
}
