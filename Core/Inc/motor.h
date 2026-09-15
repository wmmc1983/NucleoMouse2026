/*
 * motor.h
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

typedef struct {
	volatile uint16_t arr_l;	//左モータPWM周期
	volatile uint16_t arr_r;	//右モータPWM周期
	bool dir_l;	//左モータ回転方向
	bool dir_r;	//右モータ回転方向
	float speed_mm_s_l;	//左モータ速度[mm/s]
	float speed_mm_s_r;	//右モータ速度[mm/s]
	float acc_mm_s2_l;	//左モータ加速度[mm/s^2]
	float acc_mm_s2_r;	//右モータ加速度[mm/s^2]

	volatile uint32_t pulse_l, pulse_r; //左モータパルス数、右モータパルス数
	volatile float distance_mm_l, distance_mm_r; //左モータ走行距離[mm]、右モータ走行距離[mm]
} MotorData;

extern MotorData motor;

void MotorInit(void);

void MotorSetSpeedAcc(float speed_l, float speed_r, float acc_l, float acc_r);

void MotorUpdate(void);

void MotorEnable(void);

void MotorDisable(void);

void MotorStart(void);

void MotorStop(void);

void MotorSetDirection(bool dir_l, bool dir_r);

void MotorSetDistanceZero(void);


#endif /* INC_MOTOR_H_ */
