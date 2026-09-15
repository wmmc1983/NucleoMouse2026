/*
 * ui.c
 *
 *  Created on: 2026/06/22
 *      Author: Karasawa
 */




#include "global.h"
#include "ui.h"
#include "usart.h"
 //+++++++++++++++++++++++++++++++++++++++++++++++
 // led_write_1
 // LEDを点灯させる
 // 引数1：led1 …… led1のON/OFF　0なら消灯
 // 戻り値：なし
 //+++++++++++++++++++++++++++++++++++++++++++++++
void led_write_1(uint8_t led1) {
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, led1);
}

//++++++++++++++++++++++++++++++++++++++++++++++
// led_write_2
// LEDを点灯させる
// 引数2：led2 …… led2のON/OFF　0なら消灯
// 戻り値：なし
//++++++++++++++++++++++++++++++++++++++++++++++
void led_write_2(uint8_t led2) {
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, led2);
}

//++++++++++++++++++++++++++++++++++++++++++++++
// led_write_3
// LEDを点灯させる
// 引数3：led3 …… led3のON/OFF　0なら消灯
// 戻り値：なし
//++++++++++++++++++++++++++++++++++++++++++++++
void led_write_3(uint8_t led3) {
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, led3);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// led_write
// LEDを点灯させる
// 引数1：led1 …… led1のON/OFF　0なら消灯
// 引数2：led2 …… led2のON/OFF　0なら消灯
// 引数3：led3 …… led3のON/OFF　0なら消灯
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void led_write(uint8_t led1, uint8_t led2, uint8_t led3) {
	led_write_1(led1);
	led_write_2(led2);
	led_write_3(led3);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// led_write_4
// LED4-1,LED4-2を点灯させる
// 引数1：led4 …… led4のON/OFF　0なら消灯
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void led_write_4(uint8_t led4) {
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, led4);
}

/*------------------------------------------------------------
	モード選択
------------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
// select_mode
// モード選択を行う
// 引数1：モード番号の初期値
// 戻り値：選択されたモード番号
//+++++++++++++++++++++++++++++++++++++++++++++++
int select_mode(int mode) {
    printf("Mode : %d\n", mode);

    while (1) {
        led_write(mode & 0b001, mode & 0b010, mode & 0b100);
        if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET) {
            HAL_Delay(100);
            while (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
                ;
            mode++;
            if (mode > 7) {
                mode = 0;
            }
            printf("Mode : %d\n", mode);
        }
        if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET) {
            HAL_Delay(100);
            while (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET)
                ;
            mode--;
            if (mode < 0) {
                mode = 7;
            }
            printf("Mode : %d\n", mode);
        }

        if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET) {
            HAL_Delay(100);
            while (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET)
                ;
            return mode;
        }
    }
}

/*------------------------------------------------------------
    printf 用
------------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
//__io_putchar
// printf を使うために必要
//+++++++++++++++++++++++++++++++++++++++++++++++
int __io_putchar(int ch) {
	uint8_t c = (uint8_t)ch;

	if (c == '\n') {
		uint8_t cr = '\r';
		HAL_UART_Transmit(&huart2, &cr, 1, HAL_MAX_DELAY);
	}

	HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
	return ch;
}