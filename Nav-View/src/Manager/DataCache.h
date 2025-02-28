#pragma once

#include <mutex>
#include "imu_330/imu_330.h"
#include <map>
#include <vector>
using namespace std;

enum emDataFormat {
    IMU330_Format,
};

class DataCache
{
public:
    static DataCache& Instance();
private:
    DataCache();   // 构造函数私有化
    DataCache(DataCache const&); // 禁止拷贝构造
    DataCache& operator=(DataCache const&);

    static std::once_flag once_flag;
    static std::unique_ptr<DataCache> instance;
public:
    void init();
    void addIMUData(IMU_330_IMU1* imu);
    void addIMURAWCOUNTS(vector<IMU_330_IMU_RAWCOUNTS>& list);
    void addIMUSCALED(vector<IMU_330_IMU_SCALEDS>& list);
    void addIMUCALIBRATED(vector<IMU_330_IMU_CALIBRATEDS>& list);
public:
    IMU_330_IMU1 m_imu;
    vector<IMU_330_IMU_RAWCOUNTS> m_imu_rawcount_list;
    vector<IMU_330_IMU_SCALEDS> m_imu_scaled_list;
    vector<IMU_330_IMU_CALIBRATEDS> m_imu_calibrated_list;
};
