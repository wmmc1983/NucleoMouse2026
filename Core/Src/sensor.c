/*
 * sensor.c
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */


#include "global.h"
#include "sensor.h"
#include "adc.h"
SensorData sensor = { 0 }; //センサーのAD値を格納する構造体
//+++++++++++++++++++++++++++++++++++++++++++++++
// SensorInit
// センサー情報の初期化を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SensorInit(void) {
	//センサーの初期化を行う
	sensor.ad_r = 0; //右壁センサーのAD値を初期化
	sensor.ad_fr = 0; //前壁右センサーのAD値を初期化
	sensor.ad_fl = 0; //前壁左センサーのAD値を初期化
	sensor.ad_l = 0; //左壁センサーのAD値を初期化
	sensor.ad_batt = 0; //バッテリー電圧のAD値を初期化
	sensor.low_vol_count = 0; //低電圧フラグまでのカウントを初期化
	sensor.wall_r = false; //右壁センサーの検知フラグを初期化
	sensor.wall_f = false; //前壁センサーの検知フラグを初期化
	sensor.wall_l = false; //左壁センサーの検知フラグを初期化
}
//+++++++++++++++++++++++++++++++++++++++++++++++
// GetWallSensorInfo
// 壁の有無を判定し、フラグを更新する
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void GetWallSensorInfo(void) {
	//壁センサーの検知フラグを更新する
	sensor.wall_r = (sensor.ad_r > WALL_BASE_R); //右壁センサーの検知フラグを更新
	sensor.wall_f = (sensor.ad_fr > WALL_BASE_FR || sensor.ad_fl > WALL_BASE_FL); //前壁センサーの検知フラグを更新
	sensor.wall_l = (sensor.ad_l > WALL_BASE_L); //左壁センサーの検知フラグを更新
}
//+++++++++++++++++++++++++++++++++++++++++++++++
// SensorUpdate
// 各センサーのAD値取得とバッテリー監視を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SensorUpdate(void) {
	//l,frのAD値を取得する
	HAL_ADC_Start(&hadc1); // Lの開始
	HAL_ADC_Start(&hadc2); // Rの開始
	HAL_ADC_PollForConversion(&hadc1, 10);
	HAL_ADC_PollForConversion(&hadc2, 10);
	sensor.ad_l = HAL_ADC_GetValue(&hadc1);
	sensor.ad_r = HAL_ADC_GetValue(&hadc2);

	HAL_ADC_Start(&hadc1); // FRの開始
	HAL_ADC_Start(&hadc2); // FLの開始
	HAL_ADC_PollForConversion(&hadc1, 10);
	HAL_ADC_PollForConversion(&hadc2, 10);
	sensor.ad_fr = HAL_ADC_GetValue(&hadc1);
	sensor.ad_fl = HAL_ADC_GetValue(&hadc2);

	HAL_ADC_Start(&hadc1); // Battの開始
	HAL_ADC_PollForConversion(&hadc1, 10);
	sensor.ad_batt = HAL_ADC_GetValue(&hadc1);

	//バッテリー電圧監視
	if (sensor.ad_batt < BATT_LOW_VOL) sensor.low_vol_count++;
	if (sensor.low_vol_count > 10000) {
		led_write_4(1); //LED4を点灯させる
	}
	else {
		led_write_4(0); //LED4を消灯させる
	}
}