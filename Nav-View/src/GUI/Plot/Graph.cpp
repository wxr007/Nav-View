#include "Graph.h"
#include "ThreadManager.h"
#include "DataCache.h"

Graph::Graph(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.combo_PacketType->addItem("IMU (0xA1)");
	ui.combo_PacketType->addItem("IMU_RAWCOUNTS (0xF1)");
	ui.combo_PacketType->addItem("IMU_SCALEDS (0xF2)");
	ui.combo_PacketType->addItem("IMU_CALIBRATEDS (0xF3)");

	//angles_bar = new MonitorBar(this,"Angles (deg)");
	//angles_bar->setYRange(-200, 200);
	//angles_bar->initGraphName(QStringList() << "roll" << "pitch" << "yaw");
	//ui.verticalLayout->addWidget(angles_bar);

	//ang_rate_bar = new MonitorBar(this,"Ang Rate (deg/s)");
 //   ang_rate_bar->setYRange(-240, 240);
 //   ang_rate_bar->initGraphName(QStringList() << "x rate" << "y rate" << "z rate");
 //   ui.verticalLayout->addWidget(ang_rate_bar);

	accel_bar = new MonitorBar(this,"Accel (m/s^2)");
    accel_bar->setYRange(-4.8, 4.8);
    accel_bar->initGraphName(QStringList() << "x accel" << "y accel" << "z accel");
    ui.verticalLayout->addWidget(accel_bar);

	gyro_bar = new MonitorBar(this,"Gyro (deg/s)");
    gyro_bar->setYRange(-200, 200);
    gyro_bar->initGraphName(QStringList() << "x rate" << "y rate" << "z rate");
    ui.verticalLayout->addWidget(gyro_bar);

	temp_bar = new MonitorBar(this,"Temp (C)");
    temp_bar->setYRange(-80, 120);
    temp_bar->initGraphName(QStringList() << "x rate temp");
    ui.verticalLayout->addWidget(temp_bar);

	connect(ThreadManager::Instance().m_DataParser, SIGNAL(sgnUpdate(int)), this, SLOT(onUpdate(int)), Qt::QueuedConnection);
}

Graph::~Graph()
{}

void Graph::onUpdate(int type)
{
	if (type == 0xA1 && ui.combo_PacketType->currentIndex() == 0) {
		DataCache::Instance().m_imu;
		accel_bar->addData("x accel",0, DataCache::Instance().m_imu.accel_x);
		accel_bar->addData("y accel",1, DataCache::Instance().m_imu.accel_y);
		accel_bar->addData("y accel",2, DataCache::Instance().m_imu.accel_z);
        gyro_bar->addData("x rate",0, DataCache::Instance().m_imu.gyro_x);
        gyro_bar->addData("y rate",1, DataCache::Instance().m_imu.gyro_y);
        gyro_bar->addData("z rate",2, DataCache::Instance().m_imu.gyro_z);
        temp_bar->addData("x rate temp",0, DataCache::Instance().m_imu.temp);
	}
	else if (type == 0xF1 && ui.combo_PacketType->currentIndex() == 1) {
		DataCache::Instance().m_imu_rawcount_list;
        for (int i = 0; i < DataCache::Instance().m_imu_rawcount_list.size(); i++) {
            accel_bar->addData("x accel",0, DataCache::Instance().m_imu_rawcount_list[i].accel_x);
            accel_bar->addData("y accel",1, DataCache::Instance().m_imu_rawcount_list[i].accel_y);
            accel_bar->addData("y accel",2, DataCache::Instance().m_imu_rawcount_list[i].accel_z);
            gyro_bar->addData("x rate",0, DataCache::Instance().m_imu_rawcount_list[i].gyro_x);
            gyro_bar->addData("y rate",1, DataCache::Instance().m_imu_rawcount_list[i].gyro_y);
            gyro_bar->addData("z rate",2, DataCache::Instance().m_imu_rawcount_list[i].gyro_z);
            temp_bar->addData("x rate temp",0, DataCache::Instance().m_imu_rawcount_list[i].temperature);
        }
	}
	else if (type == 0xF2 && ui.combo_PacketType->currentIndex() == 2) {
		DataCache::Instance().m_imu_scaled_list;
        for (int i = 0; i < DataCache::Instance().m_imu_scaled_list.size(); i++) {
            accel_bar->addData("x accel",0, DataCache::Instance().m_imu_scaled_list[i].accel_x);
            accel_bar->addData("y accel",1, DataCache::Instance().m_imu_scaled_list[i].accel_y);
            accel_bar->addData("y accel",2, DataCache::Instance().m_imu_scaled_list[i].accel_z);
            gyro_bar->addData("x rate",0, DataCache::Instance().m_imu_scaled_list[i].gyro_x);
            gyro_bar->addData("y rate",1, DataCache::Instance().m_imu_scaled_list[i].gyro_y);
            gyro_bar->addData("z rate",2, DataCache::Instance().m_imu_scaled_list[i].gyro_z);
            temp_bar->addData("x rate temp",0, DataCache::Instance().m_imu_scaled_list[i].temperature);
        }
	}
	else if (type == 0xF3 && ui.combo_PacketType->currentIndex() == 3) {
        DataCache::Instance().m_imu_scaled_list;
        for (int i = 0; i < DataCache::Instance().m_imu_scaled_list.size(); i++) {
            accel_bar->addData("x accel",0, DataCache::Instance().m_imu_scaled_list[i].accel_x);
            accel_bar->addData("y accel",1, DataCache::Instance().m_imu_scaled_list[i].accel_y);
            accel_bar->addData("y accel",2, DataCache::Instance().m_imu_scaled_list[i].accel_z);
            gyro_bar->addData("x rate",0, DataCache::Instance().m_imu_scaled_list[i].gyro_x);
            gyro_bar->addData("y rate",1, DataCache::Instance().m_imu_scaled_list[i].gyro_y);
            gyro_bar->addData("z rate",2, DataCache::Instance().m_imu_scaled_list[i].gyro_z);
            temp_bar->addData("x rate temp",0, DataCache::Instance().m_imu_scaled_list[i].temperature);
        }
	}
}