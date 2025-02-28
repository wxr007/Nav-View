#pragma once

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>

class CustomMdiSubWindow : public QMdiSubWindow {
public:
    using QMdiSubWindow::QMdiSubWindow; // �̳й��캯��

protected:
    void closeEvent(QCloseEvent* event) override {
        // ��д closeEvent�������� QMdiSubWindow �İ汾���Ӷ���ɾ������
        // ���ǽ������ش���
        this->hide();
        event->ignore(); // ����Ĭ�ϵĹر���Ϊ
    }
};