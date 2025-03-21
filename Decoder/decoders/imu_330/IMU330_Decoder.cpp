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
    memset(&m_imu2, 0, sizeof(m_imu2));
    memset(&m_ahrs1, 0, sizeof(m_ahrs1));
    memset(&m_imu_rawcount, 0, sizeof(m_imu_rawcount));
    memset(&m_imu_scaled, 0, sizeof(m_imu_scaled));
    memset(&m_imu_calibrated, 0, sizeof(m_imu_calibrated));
    counter_A1 = 0;
    counter_A2 = 0;
    counter_C1 = 0;
    counter_F1 = 0;
    counter_F2 = 0;
    counter_F3 = 0;
    m_imu_rawcount_list.clear();
    m_imu_scaled_list.clear();
    m_imu_calibrated_list.clear();
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
            FILE* f_debug = FileManager::Instance().get_file("debug.log", "msgid,count,len,crc,raw_crc,tow,status\n");
            if (crc != raw.crc16) {
                if(f_debug) fprintf(f_debug, "%02X,0,%4d,%04X,%04X,0,error\n", raw.msgid, raw.length, crc, raw.crc16);
                return -1;
            }
            int num = 0;
            int msgid = decode_msg(num);
            if (msgid > 0) {
                if (msgid == 0xA1) {
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_A1, raw.length, crc, raw.crc16, m_imu.tow, num);
                }else if (msgid == 0xA2) {
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_A2, raw.length, crc, raw.crc16, m_imu.tow, num);
                }else if (msgid == 0xC1) {
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_C1, raw.length, crc, raw.crc16, m_imu.tow, num);
                }else if (msgid == 0xF1) {
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_F1, raw.length, crc, raw.crc16, m_imu.tow, num);
                }else if (msgid == 0xF2) {          
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_F2, raw.length, crc, raw.crc16, m_imu.tow, num);
                }else if (msgid == 0xF3) {          
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_F3, raw.length, crc, raw.crc16, m_imu.tow, num);
				}
                else if (msgid == 0x11) {
                    if (f_debug) fprintf(f_debug, "%02X,%8d,%4d,%04X,%04X,%d,%d\n", raw.msgid, counter_11, raw.length, crc, raw.crc16, m_imu.tow, num);
                }
            }
            return msgid;
        }
    }
    return 0;
}

int IMU330_Decoder::decode_msg(int& num)
{
    switch (raw.msgid)
    {
    case 0x11://IMU1
    {
        uint32_t i = 0;
        m_imu_get_value_list.clear();
        size_t size = sizeof(IMU_330_GET_VALUE);
        while (raw.length >= sizeof(IMU_330_GET_VALUE) * (i + 1)) {
            memcpy(&m_imu_get_value, raw.data + (i * sizeof(IMU_330_GET_VALUE)), sizeof(IMU_330_GET_VALUE));
            
            i++;
            counter_11++;
        }
        num = i;
    }
    break;
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
    case 0xA2:
    {
        if (raw.length != sizeof(IMU_330_IMU2)) {
            return -1;
        }
        memcpy(&m_imu2, raw.data, sizeof(IMU_330_IMU2));
#ifdef OUTPUT
        print_IMU_330_IMU2();
#endif
        counter_A2++;
    }
    break;
    case 0xC1: 
    {
        if (raw.length != sizeof(IMU_330_AHRS1)) {
            return -1;
        }
        memcpy(&m_ahrs1, raw.data, sizeof(IMU_330_AHRS1));
#ifdef OUTPUT
        print_IMU_330_AHRS1();
#endif
        counter_C1++;
    }
    break;
    case 0xF1://IMU_RAWCOUNTS
    {
        uint32_t i = 0;
        m_imu_rawcount_list.clear();
        size_t size = sizeof(IMU_330_IMU_RAWCOUNTS);
        while (raw.length >= sizeof(IMU_330_IMU_RAWCOUNTS)*(i+1)) {
            memcpy(&m_imu_rawcount, raw.data + (i* sizeof(IMU_330_IMU_RAWCOUNTS)), sizeof(IMU_330_IMU_RAWCOUNTS));
            m_imu_rawcount_list.push_back(m_imu_rawcount);
            i++;
            counter_F1++;
        }
#ifdef OUTPUT
        print_IMU_330_IMU_RAWCOUNTS();
#endif
        num = i;
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
        m_imu_scaled_list.clear();
        size_t size = sizeof(IMU_330_IMU_SCALEDS);
        while (raw.length >= sizeof(IMU_330_IMU_SCALEDS) * (i + 1)) {
            memcpy(&m_imu_scaled, raw.data + (i * sizeof(IMU_330_IMU_SCALEDS)), sizeof(IMU_330_IMU_SCALEDS));
            m_imu_scaled_list.push_back(m_imu_scaled);
            i++;
            counter_F2++;
        }
#ifdef OUTPUT
        print_IMU_330_IMU_SCALEDS();
#endif
        num = i;
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
        m_imu_calibrated_list.clear();
        size_t size = sizeof(IMU_330_IMU_CALIBRATEDS);
        while (raw.length >= sizeof(IMU_330_IMU_CALIBRATEDS) * (i + 1)) {
            memcpy(&m_imu_calibrated, raw.data + (i * sizeof(IMU_330_IMU_CALIBRATEDS)), sizeof(IMU_330_IMU_CALIBRATEDS));
            m_imu_calibrated_list.push_back(m_imu_calibrated);
            i++;
            counter_F3++;
        }
#ifdef OUTPUT
        print_IMU_330_IMU_CALIBRATEDS();
#endif
        num = i;
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
IMU_330_GET_VALUE* IMU330_Decoder::get_imu_get_value()
{
	return &m_imu_get_value;
}
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
vector< IMU_330_GET_VALUE>& IMU330_Decoder::get_imu_get_value_list()
{
	return m_imu_get_value_list;
}
//加计放在陀螺前面
void IMU330_Decoder::print_IMU_330_IMU1()
{
    FILE* f_imu = FileManager::Instance().get_file("imu.csv", "week,tow,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,temperature,counter\r\n");
    fprintf(f_imu, "%4i,%11.4f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_imu.week,(double)m_imu.tow/1000.0, m_imu.accel_x, m_imu.accel_y, m_imu.accel_z, 
        m_imu.gyro_x, m_imu.gyro_y, m_imu.gyro_z, m_imu.temp, m_imu.counter);
}

void IMU330_Decoder::print_IMU_330_IMU2()
{
    FILE* f_imu = FileManager::Instance().get_file("imu2.csv", "week,tow,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,temperature,counter\r\n");
    fprintf(f_imu, "%4i,%11.4f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_imu2.week, (double)m_imu2.tow / 1000.0, m_imu2.accel_x, m_imu2.accel_y, m_imu2.accel_z, 
        m_imu2.gyro_x, m_imu2.gyro_y, m_imu2.gyro_z, m_imu2.temp, m_imu2.counter);
}

void IMU330_Decoder::print_IMU_330_AHRS1()
{
    FILE* f_imu = FileManager::Instance().get_file("ahrs1.csv", "week,tow,roll,pitch,yaw,gyro_bias_x,gyro_bias_y,gyro_bias_z,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,temperature,counter\r\n");

    fprintf(f_imu, "%4i,%11.4f,\
        %9.5f,%9.5f,%9.5f,\
        %9.5f,%9.5f,%9.5f,\
        %9.5f,%9.5f,%9.5f,\
        %9.5f,%9.5f,%9.5f,%6.2f,%d\r\n",
        m_ahrs1.week, (double)m_ahrs1.tow / 1000.0,
        m_ahrs1.roll, m_ahrs1.pitch, m_ahrs1.yaw,
        m_ahrs1.gyro_bias_x, m_ahrs1.gyro_bias_y, m_ahrs1.gyro_bias_z,
        m_ahrs1.accel_x, m_ahrs1.accel_y, m_ahrs1.accel_z,
        m_ahrs1.gyro_x, m_ahrs1.gyro_y, m_ahrs1.gyro_z,
        m_ahrs1.temp, m_ahrs1.counter);
}

void IMU330_Decoder::print_IMU_330_IMU_RAWCOUNTS()
{
    FILE* f_imu = NULL;
    if (!FileManager::Instance().has_file("imu_rawcounts.csv")) {
        char title[1024] = { 0 };
        int pos = 0;
        for (int i = 0; i < m_imu_rawcount_list.size(); i++) {
            pos += sprintf(title + pos, "accel_x%d,accel_y%d,accel_z%d,gyro_x%d,gyro_y%d,gyro_z%d,temperature%d,", i + 1, i + 1, i + 1, i + 1, i + 1, i + 1, i + 1);
        }
        sprintf(title + pos, "counter\r\n");
        f_imu = FileManager::Instance().get_file("imu_rawcounts.csv", title);
    }
    else {
        f_imu = FileManager::Instance().get_file("imu_rawcounts.csv", "");
    }

    for (int i = 0; i < m_imu_rawcount_list.size(); i++) {
        fprintf(f_imu, "%10d,%10d,%10d,%10d,%10d,%10d,%10d,", 
            m_imu_rawcount_list[i].accel_x, m_imu_rawcount_list[i].accel_y, m_imu_rawcount_list[i].accel_z, 
            m_imu_rawcount_list[i].gyro_x, m_imu_rawcount_list[i].gyro_y, m_imu_rawcount_list[i].gyro_z, m_imu_rawcount_list[i].temperature);
    }
    uint16_t counter = 0;
    memcpy(&counter, raw.data + raw.length - 2, sizeof(uint16_t));
    fprintf(f_imu,"%10d\r\n", counter);
}

void IMU330_Decoder::print_IMU_330_IMU_SCALEDS()
{
    FILE* f_imu = NULL;
    if (!FileManager::Instance().has_file("imu_scaleds.csv")) {
        char title[1024] = { 0 };
        int pos = 0;
        for (int i = 0; i < m_imu_scaled_list.size(); i++) {
            pos += sprintf(title + pos, "accel_x%d,accel_y%d,accel_z%d,gyro_x%d,gyro_y%d,gyro_z%d,temperature%d,", i + 1, i + 1, i + 1, i + 1, i + 1, i + 1, i + 1);
        }
        sprintf(title + pos, "counter\r\n");
        f_imu = FileManager::Instance().get_file("imu_scaleds.csv", title);
    }
    else {
        f_imu = FileManager::Instance().get_file("imu_scaleds.csv", "");
    }

    for (int i = 0; i < m_imu_scaled_list.size(); i++) {
        fprintf(f_imu, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,", 
            m_imu_scaled_list[i].accel_x, m_imu_scaled_list[i].accel_y, m_imu_scaled_list[i].accel_z, 
            m_imu_scaled_list[i].gyro_x, m_imu_scaled_list[i].gyro_y, m_imu_scaled_list[i].gyro_z, m_imu_scaled_list[i].temperature);
    }
    uint16_t counter = 0;
    memcpy(&counter, raw.data + raw.length - 2, sizeof(uint16_t));
    fprintf(f_imu, "%10d\r\n", counter);
}

void IMU330_Decoder::print_IMU_330_IMU_CALIBRATEDS()
{
    FILE* f_imu = NULL;
    if (!FileManager::Instance().has_file("imu_calibrates.csv")) {
        char title[1024] = { 0 };
        int pos = 0;
        for (int i = 0; i < m_imu_calibrated_list.size(); i++) {
            pos += sprintf(title + pos, "accel_x%d,accel_y%d,accel_z%d,gyro_x%d,gyro_y%d,gyro_z%d,temperature%d,", i + 1, i + 1, i + 1, i + 1, i + 1, i + 1, i + 1);
        }
        sprintf(title + pos, "counter\r\n");
        f_imu = FileManager::Instance().get_file("imu_calibrates.csv", title);
    }
    else {
        f_imu = FileManager::Instance().get_file("imu_calibrates.csv", "");
    }

    for (int i = 0; i < m_imu_calibrated_list.size(); i++) {
        fprintf(f_imu, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%6.2f,",
            m_imu_calibrated_list[i].accel_x, m_imu_calibrated_list[i].accel_y, m_imu_calibrated_list[i].accel_z,
            m_imu_calibrated_list[i].gyro_x, m_imu_calibrated_list[i].gyro_y, m_imu_calibrated_list[i].gyro_z, m_imu_calibrated_list[i].temperature);
    }
    uint16_t counter = 0;
    memcpy(&counter, raw.data + raw.length - 2, sizeof(uint16_t));
    fprintf(f_imu, "%10d\r\n", counter);
}


