/*
 * params.h
 *
 *  Created on: 2026/06/22
 *      Author: Karasawa
 */

#ifndef INC_PARAMS_H_
#define INC_PARAMS_H_

 /*============================================================
     各種定数（パラメータ）設定
 ============================================================*/
 /*------------------------------------------------------------
     走行系
 ------------------------------------------------------------*/
 //----数学定数----
#define PI 3.14159265358979323846f // 円周率
 //----機体定数----
#define PULSE_PER_REV      400.0f       // モータ1回転あたりのパルス数
#define WHEEL_DIAMETER_MM     53.8f        // ホイール直径(mm)
 //----走行パルス関連----
#define PULSE_SEC_HALF 218 // 半区画走行用パルス。2倍すると1区画分に
#define PULSE_ROT_R90 159     // 右90度回転用パルス数
#define PULSE_ROT_L90 159     // 左90度回転用パルス数
#define PULSE_ROT_180 316     // 180度回転用パルス数
#define PULSE_SETPOS_BACK 300 // 後ろ壁に当てるために下がるパルス数
#define PULSE_SETPOS_SET 94 // 後ろ壁から中央までのパルス数
//----テーブルカウンタ関連----
#define MAX_T_CNT PULSE_SEC_HALF // テーブルカウンタの最大値
#define MIN_T_CNT 0              // テーブルカウンタの最小値
//----タイマ関連----
#define DEFAULT_INTERVAL 3000 // デフォルトのインターバル

//----動作方向関連----
#define MT_FWD_L GPIO_PIN_SET    // CW/CCWで前に進む出力（左）
#define MT_BACK_L GPIO_PIN_RESET // CW/CCWで後ろに進む出力（左）
#define MT_FWD_R GPIO_PIN_RESET  // CW/CCWで前に進む出力（右）
#define MT_BACK_R GPIO_PIN_SET   // CW/CCWで後ろに進む出力（右）

/*------------------------------------------------------------
    センサ系
------------------------------------------------------------*/
//----壁判断閾値（しきい値）----
#define WALL_BASE_FR 100 // 前壁右センサ
#define WALL_BASE_FL 100 // 前壁左センサ
#define WALL_BASE_R 100  // 右壁センサ
#define WALL_BASE_L 100  // 左壁センサ

#define BATT_LOW_VOL 3204 * 0.88 // バッテリ電圧低下判定のしきい値
// 33kΩと10kΩの分圧抵抗を通してバッテリ電圧を取得している→ 11.1*(10/(10+33)/3.3)*4096=3204

//----制御閾値（しきい値）----
#define CTRL_BASE_L 300 // 左制御閾値
#define CTRL_BASE_R 300 // 右制御閾値
#define CTRL_CONT 0.04F  // 比例制御係数

//----赤外線（赤色）LED発光待機時間（単位はマイクロ秒）
#define IR_WAIT_US 15

/*------------------------------------------------------------
    探索系
------------------------------------------------------------*/
#define HALF_CELL_MM (90.f)
#define FULL_CELL_MM (180.f)
#define ROTATE_90_MM (TREAD_MM * PI / 4.f) // 90度回転時の走行距離[mm]
#define ROTATE_180_MM (TREAD_MM * PI / 2.f) // 180度回転時の走行距離[mm]
#define SETPOS_BACK_MM (100.f) // 後ろ壁に当てるために下がる距離[mm]
#define SETPOS_SET_MM (40.f) // 後ろ壁から中央までの距離[mm]

#define TREAD_MM (82.f) // トレッド幅[mm]（左右の車輪の中心間距離）

#define DEFAULT_ACCEL ((DEFAULT_SPEED * DEFAULT_SPEED - 0*0) / (2.f * HALF_CELL_MM)) // デフォルト加速度[mm/s^2]
#define DEFAULT_SPEED 200.0f // デフォルト速度[mm/s]
#define DEFAULT_ROTATE_SPEED 90.0f // デフォルト回転速度[deg/s]

//----ゴール座標----
#define GOAL_X 7 // 7
#define GOAL_Y 7 // 7
//----迷路データ----
#define MAZE_SIZE 16 // 迷路のサイズ（16x16）
#define MAX_ROUTE 256 // 最大経路数

#endif /* INC_PARAMS_H_ */
