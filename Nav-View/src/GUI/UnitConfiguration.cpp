#include "UnitConfiguration.h"
#include "ThreadManager.h"

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

    connect(ThreadManager::Instance().m_DataParser, SIGNAL(sgnUpdateValues(int, QString)), this, SLOT(onUpdateValues(int, QString)), Qt::QueuedConnection);
    connect(ui.getValues, &QPushButton::clicked, this, &UnitConfiguration::on_GetAllValues_clicked);
    connect(ui.setValues, &QPushButton::clicked, this, &UnitConfiguration::on_SetAllValues_clicked);
}

UnitConfiguration::~UnitConfiguration()
{}

void UnitConfiguration::onUpdateValues(int type, QString value)
{
}

void UnitConfiguration::on_GetAllValues_clicked()
{
    if (ui.check_BaudRate->isChecked()) {
        emit ThreadManager::Instance().m_SerialThread->sgnWrite("get BAUDRATE");
    }
}

void UnitConfiguration::on_SetAllValues_clicked()
{
    if (ui.check_BaudRate->isChecked()) {
        emit ThreadManager::Instance().m_SerialThread->sgnWrite("save BAUDRATE " + ui.combo_BaudRatet->currentText().toLocal8Bit());
    }
}
