#pragma once
#include "imu_330.h"
#ifdef REALTIME
#include <vector>
using namespace std;
#endif

class IMU330_Decoder
{
public:
	IMU330_Decoder();
	~IMU330_Decoder();
	void init();
	int input_raw(uint8_t c);
	int decode_msg();
	IMU_330_IMU1* get_imu();
    IMU_330_IMU_RAWCOUNTS* get_imu_rawcount();
    IMU_330_IMU_SCALEDS* get_imu_scaled();
    IMU_330_IMU_CALIBRATEDS* get_imu_calibrated();
#ifdef REALTIME
	vector<IMU_330_IMU_RAWCOUNTS>& get_imu_rawcount_list();
    vector<IMU_330_IMU_SCALEDS>& get_imu_scaled_list();
    vector<IMU_330_IMU_CALIBRATEDS>& get_imu_calibrated_list();
#endif
protected:
	void print_IMU_330_IMU1();
	void print_IMU_330_IMU_RAWCOUNTS();
    void print_IMU_330_IMU_SCALEDS();
	void print_IMU_330_IMU_CALIBRATEDS();
private:
	IMU_330_MSG raw;
	IMU_330_IMU1 m_imu;
	IMU_330_IMU_RAWCOUNTS m_imu_rawcount;
	IMU_330_IMU_SCALEDS m_imu_scaled;
	IMU_330_IMU_CALIBRATEDS m_imu_calibrated;
#ifdef REALTIME
	vector<IMU_330_IMU_RAWCOUNTS> m_imu_rawcount_list;
    vector<IMU_330_IMU_SCALEDS> m_imu_scaled_list;
    vector<IMU_330_IMU_CALIBRATEDS> m_imu_calibrated_list;
#endif
	int counter_A1;
	int counter_F1;
	int counter_F2;
	int counter_F3;
};

