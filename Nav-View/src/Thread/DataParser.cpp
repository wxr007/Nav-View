#include "DataParser.h"
#include "DataCache.h"

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
		else if(ret != 0){
			//TODO: get value from decoder
			emit sgnUpdateValues(ret,array);
		}
	}
}