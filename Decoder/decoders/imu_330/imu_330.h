/********************************************************************
     imu_330.h -
*********************************************************************/

#ifndef __IMU_330_H_
#define __IMU_330_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include <stdint.h>
#include <stdbool.h>

/*==================================================================================================
                                    STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

#pragma pack(1)

//| sync1 | sync2 | msgid | length | data | crc16(lsb first) |
//|  1B | 1B | 1B | 1B | nB | 2B |
typedef struct {
    uint32_t nbyte;
    uint16_t header;
    uint8_t msgid;
    uint8_t length;
    uint8_t data[257U];
    uint16_t crc16;
} IMU_330_MSG;

typedef struct {//0xA1 
    uint16_t     week;
    uint32_t     tow;      // [ms]
    float        accel_x;  // [m/s/s]
    float        accel_y;  // [m/s/s]
    float        accel_z;  // [m/s/s]
    float        gyro_x;   // [deg/s]
    float        gyro_y;   // [deg/s]
    float        gyro_z;   // [deg/s]
    float        temp;     // [℃]
    uint16_t     status;
    uint16_t     counter;  // [unitless]
} IMU_330_IMU1;

typedef struct {// 0xA2  (CALIBRATEDS)
    uint16_t     week;
    uint32_t     tow;      // [ms]
    float        accel_x;  // [m/s/s]
    float        accel_y;  // [m/s/s]
    float        accel_z;  // [m/s/s]
    float        gyro_x;   // [deg/s]
    float        gyro_y;   // [deg/s]
    float        gyro_z;   // [deg/s]
    float        temp;     // [℃]
    uint16_t     status;
    uint16_t     counter;  // [unitless]
} IMU_330_IMU2;

typedef struct {//0xC1   (现在用SCALEDS,等校准完用CALIBRATEDS)
    uint16_t     week;
    uint32_t     tow;      // [ms]
    float        roll;     // [deg]
    float        pitch;    // [deg]
    float        yaw;      // [deg]
    float        gyro_bias_x; // [deg/s]
    float        gyro_bias_y; // [deg/s]
    float        gyro_bias_z; // [deg/s]
    float        accel_x;  // [m/s/s]
    float        accel_y;  // [m/s/s]
    float        accel_z;  // [m/s/s]
    float        gyro_x;   // [deg/s]
    float        gyro_y;   // [deg/s]
    float        gyro_z;   // [deg/s]
    float        temp;     // [℃]
    uint16_t     status;
    uint16_t     counter;  // [unitless]
} IMU_330_AHRS1;

typedef struct {
    int32_t accel_x;
    int32_t accel_y;
    int32_t accel_z;
    int32_t gyro_x;
    int32_t gyro_y;
    int32_t gyro_z;
    int32_t temperature;
} IMU_330_IMU_RAWCOUNTS;

//typedef struct {
//    int32_t accel_x;
//    int32_t accel_y;
//    int32_t accel_z;
//    int32_t gyro_x;
//    int32_t gyro_y;
//    int32_t gyro_z;
//    int32_t temperature;
//    int32_t accel2_x;
//    int32_t accel2_y;
//    int32_t accel2_z;
//    int32_t gyro2_x;
//    int32_t gyro2_y;
//    int32_t gyro2_z;
//    int32_t temperature2;
//    uint16_t counter;  // [unitless]
//} IMU_330_IMU_RAWCOUNTS;

typedef struct {
    float accel_x;      // [m/s/s]
    float accel_y;      // [m/s/s]
    float accel_z;      // [m/s/s]
    float gyro_x;       // [deg/s]
    float gyro_y;       // [deg/s]
    float gyro_z;       // [deg/s]
    float temperature;  // [℃]
} IMU_330_IMU_SCALEDS;

//typedef struct {
//    float accel_x;      // [m/s/s]
//    float accel_y;      // [m/s/s]
//    float accel_z;      // [m/s/s]
//    float gyro_x;       // [deg/s]
//    float gyro_y;       // [deg/s]
//    float gyro_z;       // [deg/s]
//    float temperature;  // [℃]
//    float accel2_x;      // [m/s/s]
//    float accel2_y;      // [m/s/s]
//    float accel2_z;      // [m/s/s]
//    float gyro2_x;       // [deg/s]
//    float gyro2_y;       // [deg/s]
//    float gyro2_z;       // [deg/s]
//    float temperature2;  // [℃]
//    uint16_t counter;  // [unitless]
//} IMU_330_IMU_SCALEDS;

typedef struct {
    float accel_x;      // [m/s/s]
    float accel_y;      // [m/s/s]
    float accel_z;      // [m/s/s]
    float gyro_x;       // [deg/s]
    float gyro_y;       // [deg/s]
    float gyro_z;       // [deg/s]
    float temperature;  // [℃]
}IMU_330_IMU_CALIBRATEDS;

//typedef struct {
//    float accel_x;      // [m/s/s]
//    float accel_y;      // [m/s/s]
//    float accel_z;      // [m/s/s]
//    float gyro_x;       // [deg/s]
//    float gyro_y;       // [deg/s]
//    float gyro_z;       // [deg/s]
//    float temperature;  // [℃]
//    float accel2_x;      // [m/s/s]
//    float accel2_y;      // [m/s/s]
//    float accel2_z;      // [m/s/s]
//    float gyro2_x;       // [deg/s]
//    float gyro2_y;       // [deg/s]
//    float gyro2_z;       // [deg/s]
//    float temperature2;  // [℃]
//    uint16_t counter;  // [unitless]
//}IMU_330_IMU_CALIBRATEDS;

#pragma pack()



#endif /* __IMU_330_H_ */