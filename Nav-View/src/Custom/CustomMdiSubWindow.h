#pragma once

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>

class CustomMdiSubWindow : public QMdiSubWindow {
public:
    using QMdiSubWindow::QMdiSubWindow; // 继承构造函数

protected:
    void closeEvent(QCloseEvent* event) override {
        // 重写 closeEvent，不调用 QMdiSubWindow 的版本，从而不删除窗口
        // 而是仅仅隐藏窗口
        this->hide();
        event->ignore(); // 忽略默认的关闭行为
    }
};