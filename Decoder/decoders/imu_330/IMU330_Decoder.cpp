#include "IMU330_Decoder.h"
#include <memory.h>
#include "FileManager.h"

static int16_t crc16_ccitt_false(uint8_t* buf, uint16_t length)
{
    uint16_t crc = 0xFFFF; // seed is 0xFFFF
    for (int i = 0; i < length; i++) {
        crc ^= buf[i] << 8;

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // poly is 0x1021
            }
            else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

IMU330_Decoder::IMU330_Decoder()
{
    init();
}

IMU330_Decoder::~IMU330_Decoder()
{
}

void IMU330_Decoder::init()
{
    memset(&raw, 0, sizeof(raw));
    memset(&m_imu, 0, sizeof(m_imu));
    memset(&m_imu_rawcount, 0, sizeof(m_imu_rawcount));
    memset(&m_imu_scaled, 0, sizeof(m_imu_scaled));
    memset(&m_imu_calibrated, 0, sizeof(m_imu_calibrated));
    counter_A1 = 0;
    counter_F1 = 0;
    counter_F2 = 0;
    counter_F3 = 0;
#ifdef REALTIME
    m_imu_rawcount_list.clear();
    m_imu_scaled_list.clear();
    m_imu_calibrated_list.clear();
#endif
}

int IMU330_Decoder::input_raw(uint8_t c)
{
    if (raw.nbyte == 0)
    {
        raw.header = (raw.header << 8) | (uint8_t)c;
        if (raw.header == 0xA596)
            raw.nbyte = 2;
    }
    else if (raw.nbyte == 2)
    {
        raw.msgid = c;
        raw.nbyte++;
    }
    else if (raw.nbyte == 3)
    {
        raw.length = c;
        raw.nbyte++;
    }
    else
    {
        raw.data[raw.nbyte - 4] = c;
        raw.nbyte++;
        if (raw.nbyte == raw.length + 6)//length 的长度只包含data，不包括4字节头，和2字节的crc
        {
            raw.nbyte = 0;
            raw.header = 0;
            memcpy(&raw.crc16, raw.data + raw.length, 2);
            //raw.crc16 = (raw.data[raw.length - 2] << 8) | raw.data[raw.length - 1];

            uint16_t crc = crc16_ccitt_false(&raw.msgid, raw.length + 2);
#ifdef OUTPUT
            FILE* f_debug = FileManager::Instance().get_file("debug.log", "");
#endif
            if (crc != raw.crc16) {
#ifdef OUTPUT
                if(f_debug) fprintf(f_debug, "error: msgid = %02X, crc = %4X, raw_crc = %4X\n", raw.msgid, crc, raw.crc16);
#endif
                return -1;
            }
            int msgid = decode_msg();
#ifdef OUTPUT
            if (msgid > 0) {
                if (msgid == 0xA1) {
                    if (f_debug) fprintf(f_debug, "%d: msgid = %02X, crc = %4X, raw_crc = %4X, tow = %d\n", counter_A1, raw.msgid, crc, raw.crc16, m_imu.tow);
                }else if (msgid == 0xF1) {          
                    if (f_debug) fprintf(f_debug, "%d: msgid = %02X, crc = %4X, raw_crc = %4X\n", counter_F1, raw.msgid, crc, raw.crc16);
                }else if (msgid == 0xF2) {          
                    if (f_debug) fprintf(f_debug, "%d: msgid = %02X, crc = %4X, raw_crc = %4X\n", counter_F2, raw.msgid, crc, raw.crc16);
                }else if (msgid == 0xF3) {          
                    if (f_debug) fprintf(f_debug, "%d: msgid = %02X, crc = %4X, raw_crc = %4X\n", counter_F3, raw.msgid, crc, raw.crc16);
                }
            }
#endif
            return msgid;
        }
    }
    return 0;
}

int IMU330_Decoder::decode_msg()
{
    switch (raw.msgid)
    {
    case 0xA1://IMU1
    {
        if (raw.length != sizeof(IMU_330_IMU1)) {
            return -1;
        }
        memcpy(&m_imu, raw.data, sizeof(IMU_330_IMU1));
#ifdef OUTPUT
        print_IMU_330_IMU1();
#endif
        counter_A1++;
    }
    break;
    case 0xF1://IMU_RAWCOUNTS
    {
        uint32_t i = 0;
#ifdef REALTIME
        m_imu_rawcount_list.clear();
#endif
        while (raw.length >= sizeof(IMU_330_IMU_RAWCOUNTS)*(i+1)) {
            memcpy(&m_imu_rawcount, raw.data + (i* sizeof(IMU_330_IMU_RAWCOUNTS)), sizeof(IMU_330_IMU_RAWCOUNTS));
#ifdef OUTPUT
            print_IMU_330_IMU_RAWCOUNTS();
#endif
#ifdef REALTIME
            m_imu_rawcount_list.push_back(m_imu_rawcount);
#endif
            i++;
            counter_F1++;
        }    
        //if (raw.length != sizeof(IMU_330_IMU_RAWCOUNTS)) {
        //    return -1;
        //}
        //memcpy(&m_imu_rawcount, raw.data, sizeof(IMU_330_IMU_RAWCOUNTS));
        //print_IMU_330_IMU_RAWCOUNTS();
        //counter_F1++;
    }
    break;
    case 0xF2://IMU_SCALEDS
    {
        uint32_t i = 0;
#ifdef REALTIME
        m_imu_scaled_list.clear();
#endif
        while (raw.length >= sizeof(IMU_330_IMU_SCALEDS) * (i + 1)) {
            memcpy(&m_imu_scaled, raw.data + (i * sizeof(IMU_330_IMU_SCALEDS)), sizeof(IMU_330_IMU_SCALEDS));
#ifdef OUTPUT
            print_IMU_330_IMU_SCALEDS();
#endif
#ifdef REALTIME
            m_imu_scaled_list.push_back(m_imu_scaled);
#endif
            i++;
            counter_F2++;
        }
        //if (raw.length != sizeof(IMU_330_IMU_SCALEDS)) {
        //    return -1;
        //}
        //memcpy(&m_imu_scaled, raw.data, sizeof(IMU_330_IMU_SCALEDS));
        //print_IMU_330_IMU_SCALEDS();
        //counter_F2++;
    }
    break;
    case 0xF3://IMU_CALIBRATEDS 
    {
        uint32_t i = 0;
#ifdef REALTIME
        m_imu_calibrated_list.clear();
#endif
        while (raw.length >= sizeof(IMU_330_IMU_CALIBRATEDS) * (i + 1)) {
            memcpy(&m_imu_calibrated, raw.data + (i * sizeof(IMU_330_IMU_CALIBRATEDS)), sizeof(IMU_330_IMU_CALIBRATEDS));
#ifdef OUTPUT
            print_IMU_330_IMU_CALIBRATEDS();
#endif
#ifdef REALTIME
            m_imu_calibrated_list.push_back(m_imu_calibrated);
#endif
            i++;
            counter_F3++;
        }
        //if (raw.length != sizeof(IMU_330_IMU_CALIBRATEDS)) {
        //    return -1;
        //}
        //memcpy(&m_imu_calibrated, raw.data, sizeof(IMU_330_IMU_CALIBRATEDS));
        //print_IMU_330_IMU_CALIBRATEDS();
        //counter_F3++;
    }
    break;
    default:
        break;
    }
    return raw.msgid;
}
IMU_330_IMU1* IMU330_Decoder::get_imu()
{
    return &m_imu;
}
IMU_330_IMU_RAWCOUNTS* IMU330_Decoder::get_imu_rawcount()
{
    return &m_imu_rawcount;
}
IMU_330_IMU_SCALEDS* IMU330_Decoder::get_imu_scaled()
{
    return &m_imu_scaled;
}
IMU_330_IMU_CALIBRATEDS* IMU330_Decoder::get_imu_calibrated()
{
    return &m_imu_calibrated;
}
#ifdef REALTIME
vector<IMU_330_IMU_RAWCOUNTS>& IMU330_Decoder::get_imu_rawcount_list()
{
    return m_imu_rawcount_list;
}
vector<IMU_330_IMU_SCALEDS>& IMU330_Decoder::get_imu_scaled_list()
{
    return m_imu_scaled_list;
}
vector<IMU_330_IMU_CALIBRATEDS>& IMU330_Decoder::get_imu_calibrated_list()
{
    return m_imu_calibrated_list;
}
#endif
//加计放在陀螺前面
void IMU330_Decoder::print_IMU_330_IMU1()
{
    FILE* f_imu = FileManager::Instance().get_file("imu.csv", "week,tow,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,temperature,counter\r\n");
    fprintf(f_imu, "%4i,%11.4f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_imu.week,(double)m_imu.tow/1000.0, m_imu.accel_x, m_imu.accel_y, m_imu.accel_z, m_imu.gyro_x, m_imu.gyro_y, m_imu.gyro_z, m_imu.temp, m_imu.counter);
}

void IMU330_Decoder::print_IMU_330_IMU_RAWCOUNTS()
{
    FILE* f_imu = FileManager::Instance().get_file("imu_rawcounts.csv",
        "accel_x1,accel_y1,accel_z1,gyro_x1,gyro_y1,gyro_z1,temperature1, \
        accel_x2,accel_y2,accel_z2,gyro_x2,gyro_y2,gyro_z2,temperature2,counter\r\n");

    //打印raw.data中的所有字节
    //for (int i = 0; i < raw.length; i++) {
    //    fprintf(f_imu, "%02X ", raw.data[i]);
    //    if (i % 4 == 3) {
    //        fprintf(f_imu, ",");
    //    }
    //}
    //fprintf(f_imu, "\r\n");

    fprintf(f_imu, "%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d\r\n",
        m_imu_rawcount.accel_x, m_imu_rawcount.accel_y, m_imu_rawcount.accel_z, m_imu_rawcount.gyro_x, m_imu_rawcount.gyro_y, m_imu_rawcount.gyro_z, m_imu_rawcount.temperature,
        m_imu_rawcount.accel2_x, m_imu_rawcount.accel2_y, m_imu_rawcount.accel2_z, m_imu_rawcount.gyro2_x, m_imu_rawcount.gyro2_y, m_imu_rawcount.gyro2_z, m_imu_rawcount.temperature2,
        m_imu_rawcount.counter);
}

void IMU330_Decoder::print_IMU_330_IMU_SCALEDS()
{
    FILE* f_imu = FileManager::Instance().get_file("imu_scaleds.csv",
        "accel_x1,accel_y1,accel_z1,gyro_x1,gyro_y1,gyro_z1,temperature1, \
        accel_x2,accel_y2,accel_z2,gyro_x2,gyro_y2,gyro_z2,temperature2,counter\r\n");

    fprintf(f_imu, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_imu_scaled.accel_x, m_imu_scaled.accel_y, m_imu_scaled.accel_z, m_imu_scaled.gyro_x, m_imu_scaled.gyro_y, m_imu_scaled.gyro_z, m_imu_scaled.temperature,
        m_imu_scaled.accel2_x, m_imu_scaled.accel2_y, m_imu_scaled.accel2_z, m_imu_scaled.gyro2_x, m_imu_scaled.gyro2_y, m_imu_scaled.gyro2_z, m_imu_scaled.temperature2,
        m_imu_scaled.counter);
}

void IMU330_Decoder::print_IMU_330_IMU_CALIBRATEDS()
{
    FILE* f_imu = FileManager::Instance().get_file("imu_calibrates.csv",
        "accel_x1,accel_y1,accel_z1,gyro_x1,gyro_y1,gyro_z1,temperature1, \
        accel_x2,accel_y2,accel_z2,gyro_x2,gyro_y2,gyro_z2,temperature2,counter\r\n");

    fprintf(f_imu, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_imu_calibrated.accel_x, m_imu_calibrated.accel_y, m_imu_calibrated.accel_z, m_imu_calibrated.gyro_x, m_imu_calibrated.gyro_y, m_imu_calibrated.gyro_z, m_imu_calibrated.temperature,
        m_imu_calibrated.accel2_x, m_imu_calibrated.accel2_y, m_imu_calibrated.accel2_z, m_imu_calibrated.gyro2_x, m_imu_calibrated.gyro2_y, m_imu_calibrated.gyro2_z, m_imu_calibrated.temperature2,
        m_imu_calibrated.counter);
}


