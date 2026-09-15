/*
 * motor.c
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */
#include "global.h"
#include "motor.h"
#include "tim.h"
#include <math.h>
MotorData motor = { 0 };


void MotorInit(void)
{
	//====モータデータの初期化====
	motor = (MotorData){ 0 }; //構造体を0で初期化
	motor.dir_l = true;  //左モータの回転方向を前進に設定
	motor.dir_r = true;  //右モータの回転方向を前進に設定
	//====ステッピングモータの初期化====
	MotorDisable();       //ステッピングモータ励磁OFF
	//====PWM出力に使うタイマの設定====
	__HAL_TIM_SET_AUTORELOAD(&htim16, DEFAULT_INTERVAL);
	__HAL_TIM_SET_AUTORELOAD(&htim17, DEFAULT_INTERVAL);
}

void MotorSetSpeedAcc(float speed_l, float speed_r, float acc_l, float acc_r)
{
	motor.speed_mm_s_l = speed_l;
	motor.speed_mm_s_r = speed_r;
	motor.acc_mm_s2_l = acc_l;
	motor.acc_mm_s2_r = acc_r;
}

void MotorUpdate(void)
{
	//速度に加速度を加えて更新する
	motor.speed_mm_s_l += motor.acc_mm_s2_l / 1000.0f; // 1msごとに加速度を加算
	motor.speed_mm_s_r += motor.acc_mm_s2_r / 1000.0f; // 1msごとに加速度を加算
	//モーターの回転方向を設定する
	motor.dir_l = (motor.speed_mm_s_l >= 0.0f); // 左モータの回転方向を設定（正の速度で前進、負の速度で後退）
	motor.dir_r = (motor.speed_mm_s_r >= 0.0f); // 右モータの回転方向を設定（正の速度で前進、負の速度で後退）
	MotorSetDirection(motor.dir_l, motor.dir_r); // 回転方向を設定
	// モータの速度に応じて回転数を計算する(絶対値を使用)
	float rps_l = fabsf(motor.speed_mm_s_l) / (WHEEL_DIAMETER_MM * PI); // 左モータの回転数[回転/s]
	float rps_r = fabsf(motor.speed_mm_s_r) / (WHEEL_DIAMETER_MM * PI); // 右モータの回転数[回転/s]
	//0に近い場合は最小値を設定してパルス周期が無限大にならないようにする
	rps_l = (rps_l < 0.001f) ? 0.001f : rps_l;
	rps_r = (rps_r < 0.001f) ? 0.001f : rps_r;
	//パルス周期を計算
	float period_l = 1.0f / (rps_l * PULSE_PER_REV);
	float period_r = 1.0f / (rps_r * PULSE_PER_REV);
	// PWM周期を計算
	motor.arr_l = (uint16_t)(period_l * 1000000.0f); // マイクロ秒単位に変換
	motor.arr_r = (uint16_t)(period_r * 1000000.0f); // マイクロ秒単位に変換
}



//+++++++++++++++++++++++++++++++++++++++++++++++
//MotorEnable
// ステッピングモータを励磁する
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MotorEnable(void) {
	HAL_GPIO_WritePin(M3_GPIO_Port, M3_Pin, GPIO_PIN_RESET);  //ステッピングモーター励磁ON
	HAL_GPIO_WritePin(M3_2_GPIO_Port, M3_2_Pin, GPIO_PIN_RESET);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//MotorDisable
// ステッピングモータの励磁を切る
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MotorDisable(void) {
	HAL_GPIO_WritePin(M3_GPIO_Port, M3_Pin, GPIO_PIN_SET);    //ステッピングモーター励磁OFF
	HAL_GPIO_WritePin(M3_2_GPIO_Port, M3_2_Pin, GPIO_PIN_SET);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//MotorStart
// 走行を開始する
// （pulse_l,pulse_rを0にリセットしてタイマを有効にする）
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MotorStart(void) {
	//====モータのパルス数をリセット====
	motor.pulse_l = motor.pulse_r = 0;
	//====モータの走行距離をリセット====
	motor.distance_mm_l = motor.distance_mm_r = 0.0f;
	//====PWM出力に使うタイマを有効にする====
	/*--------------------------------------------------------------------
	  TIM16 : 16ビットタイマ。左モータの制御に使う。出力はTIM16_CH1
	--------------------------------------------------------------------*/
	__HAL_TIM_CLEAR_FLAG(&htim16, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim16, TIM_IT_UPDATE);
	// 初期ARR が設定されている想定なので CCR を ARR/2 にセットして50%出力にする
	__HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, (__HAL_TIM_GET_AUTORELOAD(&htim16) / 2));
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);

	/*--------------------------------------------------------------------
	  TIM17 : 16ビットタイマ。右モータの制御に使う。出力はTIM17_CH1
	--------------------------------------------------------------------*/
	__HAL_TIM_CLEAR_FLAG(&htim17, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim17, TIM_IT_UPDATE);
	__HAL_TIM_SET_COMPARE(&htim17, TIM_CHANNEL_1, (__HAL_TIM_GET_AUTORELOAD(&htim17) / 2));
	HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

}

//+++++++++++++++++++++++++++++++++++++++++++++++
//MotorStop
// 走行を終了する
// （タイマを止めてタイマカウント値を0にリセットする）
// 引数1：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MotorStop(void) {

	//====PWM出力に使うタイマを停止させる====
	/*--------------------------------------------------------------------
	  TIM16 : 16ビットタイマ。左モータの制御に使う。出力はTIM16_CH1
	--------------------------------------------------------------------*/
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);

	/*--------------------------------------------------------------------
	  TIM17 : 16ビットタイマ。右モータの制御に使う。出力はTIM17_CH1
	--------------------------------------------------------------------*/
	HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);

	__HAL_TIM_SET_COUNTER(&htim16, 0);  // Reset Counter
	__HAL_TIM_SET_COUNTER(&htim17, 0);  // Reset Counter
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//MotorSetDirection
// 進行方向を設定する
// 引数1：dir_l : 左モータの回転方向（true:前進、false:後退）
// 引数2：dir_r : 右モータの回転方向（true:前進、false:後退）
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MotorSetDirection(bool dir_l, bool dir_r) {
	// 左モータの回転方向を設定
	if (dir_l) {
		HAL_GPIO_WritePin(CW_CCW_L_GPIO_Port, CW_CCW_L_Pin, MT_FWD_L);  //左を前進方向に設定
	} else {
		HAL_GPIO_WritePin(CW_CCW_L_GPIO_Port, CW_CCW_L_Pin, MT_BACK_L); //左を後進方向に設定
	}
	// 右モータの回転方向を設定
	if (dir_r) {
		HAL_GPIO_WritePin(CW_CCW_R_GPIO_Port, CW_CCW_R_Pin, MT_FWD_R);  //右を前進方向に設定
		HAL_GPIO_WritePin(CW_CCW_R_2_GPIO_Port, CW_CCW_R_2_Pin, MT_FWD_R);
	} else {
		HAL_GPIO_WritePin(CW_CCW_R_GPIO_Port, CW_CCW_R_Pin, MT_BACK_R); //右を後進方向に設定
		HAL_GPIO_WritePin(CW_CCW_R_2_GPIO_Port, CW_CCW_R_2_Pin, MT_BACK_R);
	}
}

void MotorSetDistanceZero(void) {
	motor.distance_mm_l = 0.0f;
	motor.distance_mm_r = 0.0f;
}