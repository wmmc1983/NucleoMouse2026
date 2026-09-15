/*
 * ui.h
 *
 *  Created on: 2026/06/22
 *      Author: Karasawa
 */

#ifndef INC_UI_H_
#define INC_UI_H_


/* LED制御関数 */
void led_write_1(uint8_t led1);
void led_write_2(uint8_t led2);
void led_write_3(uint8_t led3);
void led_write(uint8_t led1, uint8_t led2, uint8_t led3);
void led_write_4(uint8_t led4);

/* モード選択関数 */
int select_mode(int mode);

#endif /* INC_UI_H_ */
