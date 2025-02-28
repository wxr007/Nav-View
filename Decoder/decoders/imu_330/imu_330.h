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

typedef struct {
    uint16_t     week;
    uint32_t     tow;      // [ms]
    float        accel_x;  // [m/s/s]
    float        accel_y;  // [m/s/s]
    float        accel_z;  // [m/s/s]
    float        gyro_x;   // [deg/s]
    float        gyro_y;   // [deg/s]
    float        gyro_z;   // [deg/s]
    float        temp;     // [¡æ]
    uint16_t     status;
    uint16_t     counter;  // [unitless]
} IMU_330_IMU1;

typedef struct {
    int32_t accel_x;
    int32_t accel_y;
    int32_t accel_z;
    int32_t gyro_x;
    int32_t gyro_y;
    int32_t gyro_z;
    int32_t temperature;
    int32_t accel2_x;
    int32_t accel2_y;
    int32_t accel2_z;
    int32_t gyro2_x;
    int32_t gyro2_y;
    int32_t gyro2_z;
    int32_t temperature2;
    uint16_t counter;  // [unitless]
} IMU_330_IMU_RAWCOUNTS;

typedef struct {
    float accel_x;      // [m/s/s]
    float accel_y;      // [m/s/s]
    float accel_z;      // [m/s/s]
    float gyro_x;       // [deg/s]
    float gyro_y;       // [deg/s]
    float gyro_z;       // [deg/s]
    float temperature;  // [¡æ]
    float accel2_x;      // [m/s/s]
    float accel2_y;      // [m/s/s]
    float accel2_z;      // [m/s/s]
    float gyro2_x;       // [deg/s]
    float gyro2_y;       // [deg/s]
    float gyro2_z;       // [deg/s]
    float temperature2;  // [¡æ]
    uint16_t counter;  // [unitless]
} IMU_330_IMU_SCALEDS;

typedef struct {
    float accel_x;      // [m/s/s]
    float accel_y;      // [m/s/s]
    float accel_z;      // [m/s/s]
    float gyro_x;       // [deg/s]
    float gyro_y;       // [deg/s]
    float gyro_z;       // [deg/s]
    float temperature;  // [¡æ]
    float accel2_x;      // [m/s/s]
    float accel2_y;      // [m/s/s]
    float accel2_z;      // [m/s/s]
    float gyro2_x;       // [deg/s]
    float gyro2_y;       // [deg/s]
    float gyro2_z;       // [deg/s]
    float temperature2;  // [¡æ]
    uint16_t counter;  // [unitless]
}IMU_330_IMU_CALIBRATEDS;

#pragma pack()



#endif /* __IMU_330_H_ */