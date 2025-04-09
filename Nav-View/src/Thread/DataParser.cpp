#include "DataParser.h"
#include "DataCache.h"
#include "ThreadManager.h"

DataParser::DataParser(QObject *parent)
	: QObject(parent)
{
	m_DataFormat = IMU330_Format;
	m_IMU330_Decoder = new IMU330_Decoder();
}

DataParser::~DataParser()
{}

void DataParser::init()
{
	m_IMU330_Decoder->init();
	DataCache::Instance().init();
}

void DataParser::setDataFormat(int index)
{
	m_DataFormat = index;
}

void DataParser::parseData(QByteArray& array)
{
	if (IMU330_Format == m_DataFormat) {
		processIMU330(array);
	}
}

void DataParser::processIMU330(QByteArray array)
{
	for (int i = 0; i < array.size(); i++) {
		int ret = m_IMU330_Decoder->input_raw(array[i]);
		if (ret == 0xA1) {
			IMU_330_IMU1* pImu = m_IMU330_Decoder->get_imu();
            DataCache::Instance().addIMUData(pImu);
			emit sgnUpdate(ret);
		}
		else if (ret == 0xF1) {
			vector<IMU_330_IMU_RAWCOUNTS>& list = m_IMU330_Decoder->get_imu_rawcount_list();
            DataCache::Instance().addIMURAWCOUNTS(list);
			emit sgnUpdate(ret);
		}
		else if (ret == 0xF2) {
			vector<IMU_330_IMU_SCALEDS>& list = m_IMU330_Decoder->get_imu_scaled_list();
            DataCache::Instance().addIMUSCALED(list);
			emit sgnUpdate(ret);
		}
		else if (ret == 0xF3) {
			vector<IMU_330_IMU_CALIBRATEDS>& list = m_IMU330_Decoder->get_imu_calibrated_list();
            DataCache::Instance().addIMUCALIBRATED(list);
			emit sgnUpdate(ret);
		}
		else if (ret == 0x01) {
			ThreadManager::Instance().m_DetectionThread->m_Mutex.lock();
			DataCache::Instance().m_ret0x01 = true;
			ThreadManager::Instance().m_DetectionThread->m_WaitCondition.notify_all();
            ThreadManager::Instance().m_DetectionThread->m_Mutex.unlock();
		}
		else if (ret == 0x11) {
			IMU_330_GET_VALUE* pImu = m_IMU330_Decoder->get_imu_get_value();
			if (pImu->reg_id == 0x0040|| pImu->reg_id == 0x0060) {
				DataCache::Instance().addIMUGetValues_A1(pImu);
				emit sgnUpdate(ret);
			}
			else if (pImu->reg_id == 0x0041 || pImu->reg_id == 0x0061) {
				DataCache::Instance().addIMUGetValues_F1(pImu);
				emit sgnUpdate(ret);
			}
			else if (pImu->reg_id == 0x0042 || pImu->reg_id == 0x0062) {
				DataCache::Instance().addIMUGetValues_F2(pImu);
				emit sgnUpdate(ret);
			}
			else if (pImu->reg_id == 0x0043 || pImu->reg_id == 0x0063) {
				DataCache::Instance().addIMUGetValues_F3(pImu);
				emit sgnUpdate(ret);
			}
			else
			{
				DataCache::Instance().addIMUGetValues(pImu);
				emit sgnUpdate(ret);
			}
		}
		else if(ret != 0){
			//TODO: get value from decoder
			emit sgnUpdateValues(ret,array);
		}
	}
}