#include "driveTest.h"


void DriveTest(void) {
    printf("Drive Test Mode\n");
    uint8_t mode = 0;
    MotorInit();
    MotorEnable();

    while (true) {
        mode = select_mode(mode); // モード選択関数を呼び出して、モードを更新
        switch (mode) {
            case 0:
                for (int8_t i = 0; i < 8; i++) {
                    DriveRotateR90();
                    HAL_Delay(100);
                }
                break;
            case 1:
                for (int8_t i = 0; i < 8; i++) {
                    DriveRotateL90();
                    HAL_Delay(100);
                }
                break;
            case 2:
                for (int8_t i = 0; i < 4; i++) {
                    DriveRotateR180();
                    HAL_Delay(100);
                }
                break;
            case 3:
                DriveSetPosition();
                HAL_Delay(100);
                break;
            case 4:
                DriveTurnR90();
                MotorStop();
                HAL_Delay(100);
                break;
            case 5:
                DriveTurnL90();
                MotorStop();
                HAL_Delay(100);
                break;
            case 6:
                DriveTurn180();
                MotorStop();
                HAL_Delay(100);
                break;
            case 7:
                DriveOneSection();
                MotorStop();
                HAL_Delay(100);
                break;
            default:
            break;
        }
    }
}