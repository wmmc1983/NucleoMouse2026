/*
 * drive.c
 *
 *  Created on: Jul 4, 2026
 *      Author: akiaki96
 */

#include "drive.h"
#include "motor.h"

DriveData drive = { 0 };

void DriveUpdate(void) {
    // 機体の速度と加速度をモータの速度と加速度から計算する
    drive.speed_mm_s = (motor.speed_mm_s_l + motor.speed_mm_s_r) / 2.0f;
    drive.accel_mm_s2 = (motor.acc_mm_s2_l + motor.acc_mm_s2_r) / 2.0f;
    drive.angle_speed_deg_s = (motor.speed_mm_s_r - motor.speed_mm_s_l) / TREAD_MM * (180.0f / PI);
    drive.angle_accel_deg_s2 = (motor.acc_mm_s2_r - motor.acc_mm_s2_l) / TREAD_MM * (180.0f / PI);

    if (drive.wall_following) {
        int16_t dif_l = (int16_t)sensor.ad_l - (int16_t)BASE_L;
        int16_t dif_r = (int16_t)sensor.ad_r - (int16_t)BASE_R;
        if (CTRL_BASE_L < dif_l) {
            float ctrl = (float)(dif_l) * CTRL_CONT;
            DriveSetAngleSpeed(ctrl);
        }
        if (CTRL_BASE_R < dif_r) {
            float ctrl = (float)(dif_r) * CTRL_CONT;
            DriveSetAngleSpeed(-ctrl);
        }
    } else {
    
    }
}

void DriveSetSpeed(float speed) {
    // モータの速度を設定する
    MotorSetSpeedAcc(speed, speed, motor.acc_mm_s2_l, motor.acc_mm_s2_r);
    drive.speed_mm_s = speed; // 即座に並進速度を同期
}

void DriveSetAccel(float accel) {
    // モータの加速度を設定する
    MotorSetSpeedAcc(motor.speed_mm_s_l, motor.speed_mm_s_r, accel, accel);
    drive.accel_mm_s2 = accel; // 即座に並進加速度を同期
}

void DriveSetAngleSpeed(float angle_speed) {
    drive.angle_speed_deg_s = angle_speed; // 角速度を設定する
    // 角速度を設定する
    float speed_l = drive.speed_mm_s - (angle_speed * TREAD_MM / 2.0f) * (PI / 180.0f);
    float speed_r = drive.speed_mm_s + (angle_speed * TREAD_MM / 2.0f) * (PI / 180.0f);
    MotorSetSpeedAcc(speed_l, speed_r, motor.acc_mm_s2_l, motor.acc_mm_s2_r);
}

void DriveSetAngleAccel(float angle_accel) {
    drive.angle_accel_deg_s2 = angle_accel; // 即座に角加速度を同期
    // モータの角加速度を設定する
    float accel_l = drive.accel_mm_s2 - (angle_accel * TREAD_MM / 2.0f) * (PI / 180.0f);
    float accel_r = drive.accel_mm_s2 + (angle_accel * TREAD_MM / 2.0f) * (PI / 180.0f);
    MotorSetSpeedAcc(motor.speed_mm_s_l, motor.speed_mm_s_r, accel_l, accel_r);
}

/*
 * DriveHalfSectionAccel
 * 半区画加速走行を行う
 * 引数：なし
 * 戻り値：なし
*/
void DriveHalfSectionAccel(void) {
    drive.wall_following = true;
    DriveSetSpeed(0.f);
    DriveSetAccel(DEFAULT_ACCEL);
    MotorStart();
    while((motor.distance_mm_l < HALF_CELL_MM) || (motor.distance_mm_r < HALF_CELL_MM)) {
    }
    DriveSetSpeed(DEFAULT_SPEED);
    MotorStop();
}

/*
 * DriveHalfSectionBreak
 * 半区画減速走行を行う
 * 引数：なし
 * 戻り値：なし
*/
void DriveHalfSectionBreak(void) {
    drive.wall_following = true;
    DriveSetSpeed(DEFAULT_SPEED);
    DriveSetAccel(-DEFAULT_ACCEL);
    MotorStart();
    while((motor.distance_mm_l < HALF_CELL_MM) || (motor.distance_mm_r < HALF_CELL_MM)) {
    }
    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStop();
}

void DriveOneSection(void) {
    drive.wall_following = true;
    DriveSetSpeed(DEFAULT_SPEED);
    DriveSetAccel(0.f);
    MotorStart();
    while((motor.distance_mm_l < FULL_CELL_MM) || (motor.distance_mm_r < FULL_CELL_MM)) {
    }
    MotorStop();
}

void DriveRotateR90(void) {
    drive.wall_following = false; // 壁追従を無効化

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStart();
    DriveSetAngleSpeed(-DEFAULT_ROTATE_SPEED);

    while ((motor.distance_mm_l < ROTATE_90_MM) || (motor.distance_mm_r < ROTATE_90_MM)) {

    }

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    DriveSetAngleSpeed(0.f);
    MotorStop();
}

void DriveRotateL90(void) {
    drive.wall_following = false; // 壁追従を無効化

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStart();
    DriveSetAngleSpeed(DEFAULT_ROTATE_SPEED);

    while ((motor.distance_mm_l < ROTATE_90_MM) || (motor.distance_mm_r < ROTATE_90_MM)) {

    }

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    DriveSetAngleSpeed(0.f);
    MotorStop();
}

void DriveRotateR180(void) {
    drive.wall_following = false; // 壁追従を無効化

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStart();
    DriveSetAngleSpeed(-DEFAULT_ROTATE_SPEED);

    while ((motor.distance_mm_l < ROTATE_180_MM) || (motor.distance_mm_r < ROTATE_180_MM)) {

    }

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    DriveSetAngleSpeed(0.f);
    MotorStop();
}

void DriveRotateL180(void) {
    drive.wall_following = false; // 壁追従を無効化
    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStart();
    DriveSetAngleSpeed(DEFAULT_ROTATE_SPEED);

    while ((motor.distance_mm_l < ROTATE_180_MM) || (motor.distance_mm_r < ROTATE_180_MM)) {

    }

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    DriveSetAngleSpeed(0.f);
    MotorStop();
}

void DriveTurnR90(void) {
    drive.wall_following = false; // 壁追従を無効化
    DriveHalfSectionBreak();
    DriveRotateR90();
    DriveHalfSectionAccel();
}

void DriveTurnL90(void) {
    drive.wall_following = false; // 壁追従を無効化
    DriveHalfSectionBreak();
    DriveRotateL90();
    DriveHalfSectionAccel();
}

void DriveTurn180(void) {
    drive.wall_following = false; // 壁追従を無効化
    DriveHalfSectionBreak();
    DriveRotateR180();
    DriveHalfSectionAccel();
}

void DriveSetPosition(void) {
    DriveSetAngleSpeed(0.f);
    DriveSetAngleAccel(0.f);
    DriveSetAccel(0.f);
    DriveSetSpeed(-DEFAULT_SPEED/4);
	MotorStart();
    while((motor.distance_mm_l < SETPOS_BACK_MM) || (motor.distance_mm_r < SETPOS_BACK_MM)) {
    }

    MotorStop();
    MotorStart();
    DriveSetSpeed(DEFAULT_SPEED/4);
    while ((motor.distance_mm_l < SETPOS_SET_MM) || (motor.distance_mm_r < SETPOS_SET_MM)) {
    }

    DriveSetSpeed(0.f);
    DriveSetAccel(0.f);
    MotorStop();
}
