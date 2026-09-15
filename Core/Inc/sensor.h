/*
 * sensor.h
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

typedef struct {
	uint32_t ad_r, ad_fr, ad_fl, ad_l; //壁センサーのAD値
	uint32_t ad_batt; //バッテリー電圧のAD値
	uint16_t low_vol_count;				//低電圧フラグまでのカウント
	bool wall_r, wall_f, wall_l; //壁センサーの検知フラグ
} SensorData;

extern SensorData sensor;

void GetWallSensorInfo(void);

void SensorUpdate(void);


#endif /* INC_SENSOR_H_ */

