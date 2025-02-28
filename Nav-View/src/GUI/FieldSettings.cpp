#include "FieldSettings.h"
#include <QDebug>
#include "ThreadManager.h"

FieldSettings::FieldSettings(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Field Settings");
    mainlayout = new QVBoxLayout(this);
    headerLayout = new QHBoxLayout(this);
    gridLayout = new QGridLayout(this);
    footLayout = new QVBoxLayout(this);

    mainlayout->addLayout(headerLayout);
    mainlayout->addLayout(gridLayout);
    mainlayout->addLayout(footLayout);

    // EEPROM 相关设置
    readWriteEepromCheckbox = new QCheckBox("Read/Write EEPROM");
    statusLabel = new QLabel("Status:");
    statusEdit = new QLineEdit();
    headerLayout->addWidget(readWriteEepromCheckbox);
    headerLayout->addStretch();
    headerLayout->addWidget(statusLabel);
    headerLayout->addWidget(statusEdit);

    // 表头
    QLabel* useLabel = new QLabel("Use");
    QLabel* fieldLabel = new QLabel("Field");
    useLabel->setAlignment(Qt::AlignCenter);
    fieldLabel->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(useLabel, 0, 0);
    gridLayout->addWidget(fieldLabel, 0, 1);

    getFieldButton = new QPushButton("Get Field");
    setFieldButton = new QPushButton("Set Field");
    gridLayout->addWidget(getFieldButton, 0, 2);
    gridLayout->addWidget(setFieldButton, 0, 3);

    // 字段行
    for (int i = 0; i < MAX_FIELDS; ++i) {
        useCheckboxes[i] = new QCheckBox();
        fieldEdits[i] = new QLineEdit();
        getFieldEdits[i] = new QLineEdit();
        getFieldEdits[i]->setReadOnly(true);
        getFieldEdits[i]->setEnabled(false);
        setFieldEdits[i] = new QLineEdit();

        gridLayout->addWidget(useCheckboxes[i], i + 1, 0);
        gridLayout->addWidget(fieldEdits[i], i + 1, 1);
        gridLayout->addWidget(getFieldEdits[i], i + 1, 2);
        gridLayout->addWidget(setFieldEdits[i], i + 1, 3);

        // 连接 checkbox 的状态改变信号到槽函数
        connect(useCheckboxes[i], &QCheckBox::stateChanged, [this, i](int state) {
            this->on_checkbox_state_changed(state, i);
            });
        if (i == 0) {
            useCheckboxes[i]->setChecked(true);
            fieldEdits[i]->setEnabled(true);
            setFieldEdits[i]->setEnabled(true);
        }
        else {
            useCheckboxes[i]->setChecked(false);
            fieldEdits[i]->setEnabled(false);
            setFieldEdits[i]->setEnabled(false);
        }
    }

    // TX 和 RX 区域
    QLabel* txLabel = new QLabel("TX");
    QPlainTextEdit* txTextEdit = new QPlainTextEdit();
    QLabel* rxLabel = new QLabel("RX");
    QPlainTextEdit* rxTextEdit = new QPlainTextEdit();
    footLayout->addWidget(txLabel);
    footLayout->addWidget(txTextEdit);
    footLayout->addWidget(rxLabel);
    footLayout->addWidget(rxTextEdit);

    connect(ThreadManager::Instance().m_DataParser, SIGNAL(sgnUpdateValues(int, QString)), this, SLOT(onUpdateValues(int, QString)), Qt::QueuedConnection);
    // 连接按钮的点击信号到对应的槽函数
    connect(getFieldButton, &QPushButton::clicked, this, &FieldSettings::on_getFieldButton_clicked);
    connect(setFieldButton, &QPushButton::clicked, this, &FieldSettings::on_setFieldButton_clicked);
    connect(readWriteEepromCheckbox, &QCheckBox::stateChanged, this, &FieldSettings::on_writeEeprom_state_changed);
}

FieldSettings::~FieldSettings()
{
    // 清理动态创建的控件
    delete readWriteEepromCheckbox;
    delete statusLabel;
    delete statusEdit;
    delete getFieldButton;
    delete setFieldButton;
    for (int i = 0; i < MAX_FIELDS; ++i) {
        delete useCheckboxes[i];
        delete fieldEdits[i];
        delete getFieldEdits[i];
        delete setFieldEdits[i];
    }
    delete gridLayout;
}

void FieldSettings::onUpdateValues(int type, QString value)
{
}

void FieldSettings::on_getFieldButton_clicked()
{
    // 这里实现获取字段的逻辑，示例中只是简单打印
    qDebug() << "Get Field button clicked";
    // 你可能需要从相关设备或数据源获取值，并设置到对应的文本框中
    for (int i = 0; i < MAX_FIELDS; ++i) {
        // 示例：简单设置一个值
        if (useCheckboxes[i]->isChecked()) {
            getFieldEdits[i]->setText("Sample Value");
        }
    }
}

void FieldSettings::on_setFieldButton_clicked()
{
    // 这里实现设置字段的逻辑，示例中只是简单打印
    qDebug() << "Set Field button clicked";
    // 你可能需要从文本框获取值，并发送到相关设备或数据源进行设置
    for (int i = 0; i < MAX_FIELDS; ++i) {
        if (useCheckboxes[i]->isChecked()) {
            QString value = setFieldEdits[i]->text();
            qDebug() << "Setting value:" << value << " for row " << i;
        }
    }
}

void FieldSettings::on_checkbox_state_changed(int state, int row)
{
    bool enabled = (state == Qt::Checked);
    fieldEdits[row]->setEnabled(enabled);
    setFieldEdits[row]->setEnabled(enabled);
}

void FieldSettings::on_writeEeprom_state_changed(int state)
{
    getFieldButton->setText(state == Qt::Checked ? "Read Field" : "Get Field");
    setFieldButton->setText(state == Qt::Checked ? "Write Field" : "Set Field");
}
