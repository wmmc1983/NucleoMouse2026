/*
 * interrupt.c
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */
#include "global.h"
#include "interrupt.h"
#include "tim.h"
#include "motor.h"
#include "drive.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM16) {
		__HAL_TIM_SET_AUTORELOAD(&htim16, motor.arr_l);
		__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, (motor.arr_l / 2)); // 50%デューティ比に設定
		//pulse_lをカウントアップ
		motor.pulse_l++;
		//走行距離を計算
		motor.distance_mm_l = (motor.pulse_l / PULSE_PER_REV) * (WHEEL_DIAMETER_MM * PI);
	}
	else if (htim->Instance == TIM17) {
		__HAL_TIM_SET_AUTORELOAD(&htim17, motor.arr_r);
		__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (motor.arr_r / 2)); // 50%デューティ比に設定
		//pulse_rをカウントアップ
		motor.pulse_r++;
		//走行距離を計算
		motor.distance_mm_r = (motor.pulse_r / PULSE_PER_REV) * (WHEEL_DIAMETER_MM * PI);
	}
	else if (htim->Instance == TIM6) {
		//モーター処理
		MotorUpdate();
		DriveUpdate();
		//----センサ処理----
		//led発光
		HAL_GPIO_WritePin(IR_SIDE_GPIO_Port, IR_SIDE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IR_FRONT_GPIO_Port, IR_FRONT_Pin, GPIO_PIN_SET);
		tim6_wait_us(IR_WAIT_US);    // 光が強まるまで少し待機
		SensorUpdate();
		HAL_GPIO_WritePin(IR_SIDE_GPIO_Port, IR_SIDE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IR_FRONT_GPIO_Port, IR_FRONT_Pin, GPIO_PIN_RESET);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// tim6_wait_us
// 1us毎にカウントアップするTIM6を使ってusマイクロ秒処理を止める関数。
// （whileループ中にオーバーフローが起こると機能しないのでTIM6タイマ更新割り込みハンドラ内のみで使用することを推奨する）
// 引数：us …… 処理を止めたいマイクロ秒
// 戻り値：無し
//+++++++++++++++++++++++++++++++++++++++++++++++
void tim6_wait_us(uint32_t us) {
	uint32_t start = __HAL_TIM_GET_COUNTER(&htim6);
	while((uint16_t)(__HAL_TIM_GET_COUNTER(&htim6) - start) < us);
}
