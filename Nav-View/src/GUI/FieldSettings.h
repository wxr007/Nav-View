#ifndef FIELDSETTINGS_H
#define FIELDSETTINGS_H

#include <QWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

#define MAX_FIELDS 7

class FieldSettings : public QWidget
{
    Q_OBJECT

public:
    FieldSettings(QWidget* parent = nullptr);
    ~FieldSettings();

private slots:
    void onUpdateValues(int type, QString value);
    void on_getFieldButton_clicked();
    void on_setFieldButton_clicked();
    void on_checkbox_state_changed(int state, int row);
    void on_writeEeprom_state_changed(int state);
private:
    QVBoxLayout* mainlayout;
    QHBoxLayout* headerLayout;
    QGridLayout* gridLayout;
    QVBoxLayout* footLayout;
    QCheckBox* readWriteEepromCheckbox;
    QLabel* statusLabel;
    QLineEdit* statusEdit;
    QPushButton* getFieldButton, * setFieldButton;
    QCheckBox* useCheckboxes[MAX_FIELDS];
    QLineEdit* fieldEdits[MAX_FIELDS];
    QLineEdit* getFieldEdits[MAX_FIELDS];
    QLineEdit* setFieldEdits[MAX_FIELDS];
};

#endif // FIELDSETTINGS_H