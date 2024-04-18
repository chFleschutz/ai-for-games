#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aimainwindow.h"

class AIMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	AIMainWindow(QWidget* parent = nullptr);
	~AIMainWindow() = default;

public slots:
	void onShowFlowField();
	void onShowDebugOutput();

private:
	void changeCentralWidget(QWidget* widget);

	Ui::AIMainWindowClass ui;
};
