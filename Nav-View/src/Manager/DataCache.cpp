#include "DataCache.h"

std::once_flag DataCache::once_flag;
std::unique_ptr<DataCache> DataCache::instance;

DataCache& DataCache::Instance() {
    std::call_once(once_flag, [&]() {
        instance.reset(new DataCache());
    });
    return *instance;
}
DataCache::DataCache()
{
    m_auto_connect = false;
    m_ret0x01 = false;
    memset(&m_imu, 0, sizeof(m_imu));
    init();
}
void DataCache::init()
{
    m_imu_rawcount_list.clear();
    m_imu_scaled_list.clear();
    m_imu_calibrated_list.clear();
}

void DataCache::addIMUData(IMU_330_IMU1* pimu)
{
    memcpy(&m_imu, pimu, sizeof(IMU_330_IMU1));
}

void DataCache::addIMURAWCOUNTS(vector<IMU_330_IMU_RAWCOUNTS>& list)
{
    m_imu_rawcount_list.swap(list);
}

void DataCache::addIMUSCALED(vector<IMU_330_IMU_SCALEDS>& list)
{
    m_imu_scaled_list.swap(list);
}

void DataCache::addIMUCALIBRATED(vector<IMU_330_IMU_CALIBRATEDS>& list)
{
    m_imu_calibrated_list.swap(list);
}

void DataCache::addIMUGetValues(IMU_330_GET_VALUE* pimu)
{
    memcpy(&m_imu_get_value, pimu, sizeof(IMU_330_IMU1));
}

void DataCache::addIMUGetValues_A1(IMU_330_GET_VALUE *pimu)
{
    memcpy(&m_imu_get_value_a1, pimu, sizeof(IMU_330_IMU1));
}

void DataCache::addIMUGetValues_F1(IMU_330_GET_VALUE* pimu)
{
    memcpy(&m_imu_get_value_f1, pimu, sizeof(IMU_330_IMU1));
}

void DataCache::addIMUGetValues_F2(IMU_330_GET_VALUE* pimu)
{
    memcpy(&m_imu_get_value_f2, pimu, sizeof(IMU_330_IMU1));
}
void DataCache::addIMUGetValues_F3(IMU_330_GET_VALUE* pimu)
{
    memcpy(&m_imu_get_value_f3, pimu, sizeof(IMU_330_IMU1));
}