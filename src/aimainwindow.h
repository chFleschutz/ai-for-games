#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aimainwindow.h"

class AIMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AIMainWindow(QWidget *parent = nullptr);
    ~AIMainWindow();

private:
    Ui::AIMainWindowClass ui;
};
