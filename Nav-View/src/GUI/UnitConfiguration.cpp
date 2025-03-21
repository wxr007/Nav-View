#include "UnitConfiguration.h"
#include "ThreadManager.h"
#include "DataCache.h"
// 波特率映射
QMap<QString, uint8_t> baudRateMap = {
	{"9600", 0x02},
	{"19200", 0x04},
	{"38400", 0x05},
	{"57600", 0x07},
	{"115200", 0x09},
	{"230400", 0x0C},
	{"460800", 0x0D},
	{"921600", 0x0E}
};

// 数据包类型映射
QMap<QString, QByteArray> packetTypeMap = {
    {"IMU (0xA1)", QByteArray::fromHex("4000")},
    {"IMU_RAWCOUNTS (0xF1)", QByteArray::fromHex("4100")},
    {"IMU_SCALEDS (0xF2)", QByteArray::fromHex("4200")},
    {"IMU_CALIBRATEDS (0xF3)", QByteArray::fromHex("4300")}
};

QMap<QString, uint8_t> PacketRateMap = {
    {"0 Hz", 0x00},
    {"1 Hz", 0x01},
    {"2 Hz", 0x02},
    {"4 Hz", 0x03},
    {"5 Hz", 0x04},
    {"10 Hz", 0x05},
    {"20 Hz", 0x06},
    {"25 Hz", 0x07},
    {"50 Hz", 0x08},
    {"100 Hz", 0x09},
    {"200 Hz", 0x0A}
};

static int16_t crc16_false(uint8_t* buf, uint16_t length)
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

static QString findKeyByValue(const QMap<QString, uint8_t>& map, uint8_t value)
{
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (it.value() == value) {
            return it.key();
        }
    }
    return QString(); // 如果没有找到匹配的值，返回一个空的 QString
}
UnitConfiguration::UnitConfiguration(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.combo_BaudRatet->addItem("9600");
    ui.combo_BaudRatet->addItem("19200");
    ui.combo_BaudRatet->addItem("38400");
    ui.combo_BaudRatet->addItem("57600");
    ui.combo_BaudRatet->addItem("115200");
    ui.combo_BaudRatet->addItem("230400");
    ui.combo_BaudRatet->addItem("460800");
    ui.combo_BaudRatet->addItem("921600");

    ui.combo_PacketType->addItem("IMU (0xA1)");
    ui.combo_PacketType->addItem("IMU_RAWCOUNTS (0xF1)");
    ui.combo_PacketType->addItem("IMU_SCALEDS (0xF2)");
    ui.combo_PacketType->addItem("IMU_CALIBRATEDS (0xF3)");

    // 设置发包频率选项
    ui.combo_PacketRate->addItem("0 Hz");
    ui.combo_PacketRate->addItem("1 Hz");
    ui.combo_PacketRate->addItem("2 Hz");
    ui.combo_PacketRate->addItem("4 Hz");
    ui.combo_PacketRate->addItem("5 Hz");
    ui.combo_PacketRate->addItem("10 Hz");
    ui.combo_PacketRate->addItem("20 Hz");
    ui.combo_PacketRate->addItem("25 Hz");
    ui.combo_PacketRate->addItem("50 Hz");
    ui.combo_PacketRate->addItem("100 Hz");
    ui.combo_PacketRate->addItem("200 Hz");

    connect(ThreadManager::Instance().m_DataParser, SIGNAL(sgnUpdate(int)), this, SLOT(onUpdateValues(int)), Qt::QueuedConnection);
    connect(ui.getValues, &QPushButton::clicked, this, &UnitConfiguration::on_GetAllValues_clicked);
    connect(ui.setValues, &QPushButton::clicked, this, &UnitConfiguration::on_SetAllValues_clicked);
}

UnitConfiguration::~UnitConfiguration()
{}

void UnitConfiguration::onUpdateValues(int type)
{
	if (type == 0x11) {
        DataCache::Instance().m_imu_get_value;
        DataCache::Instance().m_imu_get_value_a1;
		DataCache::Instance().m_imu_get_value_f1;
        DataCache::Instance().m_imu_get_value_f2;
        DataCache::Instance().m_imu_get_value_f3;
		// 处理波特率
		if (DataCache::Instance().m_imu_get_value.reg_id == 0x00 && DataCache::Instance().m_imu_get_value.rc ==0) 
        {
            QString result = findKeyByValue(baudRateMap, DataCache::Instance().m_imu_get_value.reg_value);
			if (!result.isEmpty() ) {
				ui.line_BaudRatet->setText(result);
			}
		}
        if ( DataCache::Instance().m_imu_get_value_a1.rc == 0 && DataCache::Instance().m_imu_get_value_a1.reg_value != 0)
        {

			QString result = findKeyByValue(PacketRateMap, DataCache::Instance().m_imu_get_value_a1.reg_value);
			if (!result.isEmpty()) {
				ui.line_PacketRate->setText(result);
				ui.line_PacketType->setText("IMU (0xA1)");
			}
		}
		if (DataCache::Instance().m_imu_get_value_f1.rc == 0 && DataCache::Instance().m_imu_get_value_f1.reg_value != 0)
        {
            QString result = findKeyByValue(PacketRateMap, DataCache::Instance().m_imu_get_value_f1.reg_value);
            if (!result.isEmpty()) {
                ui.line_PacketRate->setText(result);
                ui.line_PacketType->setText("IMU_RAWCOUNTS (0xF1)");
            }
		}
		if ( DataCache::Instance().m_imu_get_value_f2.rc == 0 && DataCache::Instance().m_imu_get_value_f2.reg_value != 0)
        {
            QString result = findKeyByValue(PacketRateMap, DataCache::Instance().m_imu_get_value_f2.reg_value);
            if (!result.isEmpty()) {
                ui.line_PacketRate->setText(result);
                ui.line_PacketType->setText("IMU_SCALEDS (0xF2)");
            }
		}
		if ( DataCache::Instance().m_imu_get_value_f3.rc == 0 && DataCache::Instance().m_imu_get_value_f3.reg_value != 0)
        {
            QString result = findKeyByValue(PacketRateMap, DataCache::Instance().m_imu_get_value_f3.reg_value);
            if (!result.isEmpty()) {
                ui.line_PacketRate->setText(result);
                ui.line_PacketType->setText("IMU_CALIBRATEDS (0xF3)");
            }
		}
	}
}

void UnitConfiguration::on_GetAllValues_clicked()
{
    QByteArray packet;
    if (ui.check_BaudRate->isChecked())
    {

        // 包头
        packet.append(static_cast<char>(0xA5));
        packet.append(static_cast<char>(0x96));

        // 数据部分
        QByteArray data;
        data.append(static_cast<char>(0x11));
        data.append(static_cast<char>(0x03));
        data.append(static_cast<char>(0x01));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));

        // 计算CRC校验值
        uint16_t crc = crc16_false(reinterpret_cast<uint8_t*>(data.data()), data.length());
        packet.append(data);
        packet.append(static_cast<char>(crc & 0xFF));
        packet.append(static_cast<char>(crc >> 8));
        

        // 发送数据包
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
    }

    // 处理数据包类型
    if (ui.check_PacketType->isChecked() || ui.check_PacketRate->isChecked())
    {
        for (int i = 0; i < 4; i++)
        {
            packet.clear();
        // 包头
            packet.append(static_cast<char>(0xA5));
            packet.append(static_cast<char>(0x96));

        // 数据部分
            QByteArray data;
            data.append(static_cast<char>(0x11));
            data.append(static_cast<char>(0x03));
            data.append(static_cast<char>(0x01));
		
		
			uint8_t packetType = 0x40 + i;
			data.append(static_cast<char>(packetType));
            data.append(static_cast<char>(0x00));
            // 计算CRC校验值
            uint16_t crc = crc16_false(reinterpret_cast<uint8_t*>(data.data()), data.length());
            packet.append(data);
            packet.append(static_cast<char>(crc & 0xFF));
            packet.append(static_cast<char>(crc >> 8));
            
            // 发送数据包
            emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
            QThread::msleep(10);
		}
        
        
    }

    // 发送数据包
    if (!packet.isEmpty()) {
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
    }
}

static void silence_device()
{
    for (int i = 0; i < 3; i++) {
        char data1[] = { static_cast<char>(0xA5), static_cast<char>(0x96), static_cast<char>(0x12), static_cast<char>(0x04), static_cast<char>(0x01), static_cast<char>(0x40), static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xCB), static_cast<char>(0x7D) };
        QByteArray packet1(data1, sizeof(data1));
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet1);

        char data2[] = { static_cast<char>(0xA5), static_cast<char>(0x96), static_cast<char>(0x12), static_cast<char>(0x04), static_cast<char>(0x01), static_cast<char>(0x41), static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xFB), static_cast<char>(0x4A) };
        QByteArray packet2(data2, sizeof(data2));
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet2);

        char data3[] = { static_cast<char>(0xA5), static_cast<char>(0x96), static_cast<char>(0x12), static_cast<char>(0x04), static_cast<char>(0x01), static_cast<char>(0x42), static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xAB), static_cast<char>(0x13) };
        QByteArray packet3(data3, sizeof(data3));
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet3);

        char data4[] = { static_cast<char>(0xA5), static_cast<char>(0x96), static_cast<char>(0x12), static_cast<char>(0x04), static_cast<char>(0x01), static_cast<char>(0x43), static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0x9B), static_cast<char>(0x24) };
        QByteArray packet4(data4, sizeof(data4));
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet4);
    }
}



void UnitConfiguration::on_SetAllValues_clicked()
{
    QByteArray packet;
	silence_device();
   /* if (ui.check_BaudRate->isChecked()) {
        emit ThreadManager::Instance().m_SerialThread->sgnWrite("save BAUDRATE " + ui.combo_BaudRatet->currentText().toLocal8Bit());
    }*/
    // 处理波特率
    if (ui.check_BaudRate->isChecked()) {
        
        // 包头
        packet.append(static_cast<char>(0xA5));
        packet.append(static_cast<char>(0x96));

        // 数据部分
        QByteArray data;
        data.append(static_cast<char>(0x12));
        data.append(static_cast<char>(0x04));
        data.append(static_cast<char>(0x01));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));

        // 添加波特率对应的16进制数据
        QString selectedBaudRate = ui.combo_BaudRatet->currentText();
        if (baudRateMap.contains(selectedBaudRate)) {
            data.append(baudRateMap[selectedBaudRate]);
        }

        // 计算CRC校验值
        uint16_t crc = crc16_false(reinterpret_cast<uint8_t*>(data.data()), data.length());
        packet.append(data);
        packet.append(static_cast<char>(crc >> 8));
        packet.append(static_cast<char>(crc & 0xFF));

        // 发送数据包
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
    }

    // 处理数据包类型
    if (ui.check_PacketType->isChecked()) { // 假设存在名为 check_PacketType 的复选框
        
        // 包头
        packet.append(static_cast<char>(0xA5));
        packet.append(static_cast<char>(0x96));

        // 数据部分
        QByteArray data;
        data.append(static_cast<char>(0x12));
        data.append(static_cast<char>(0x04));
        data.append(static_cast<char>(0x01));
        QString selectedPacketType = ui.combo_PacketType->currentText();
        if (packetTypeMap.contains(selectedPacketType)) {
            data.append(packetTypeMap[selectedPacketType]);
        }
        if (ui.check_PacketRate->isChecked()) {
            
            QString selectedPacketRate = ui.combo_PacketRate->currentText();
            if (PacketRateMap.contains(selectedPacketRate)) {
                data.append(PacketRateMap[selectedPacketRate]);
            }
            else {
                data.append(static_cast<char>(0x09));
            }
        }
        // 计算CRC校验值
        uint16_t crc = crc16_false(reinterpret_cast<uint8_t*>(data.data()), data.length());
        packet.append(data);
        packet.append(static_cast<char>(crc >> 8));
        packet.append(static_cast<char>(crc & 0xFF));

        // 发送数据包
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
    }

    // 发送数据包
    if (!packet.isEmpty()) {
        emit ThreadManager::Instance().m_SerialThread->sgnWrite(packet);
    }
}
