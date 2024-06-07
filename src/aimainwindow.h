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
	void onShowHillClimbing();
	void onShowRQLearning();
	void onShowTacticalMaps();
	void onShowCombinedMaps();
	void onShowBallisticPrediction();

	void onShowDebugOutput();

private:
	template <typename T>
	void changeCentralWidget()
	{
		static_assert(std::is_base_of_v<QWidget, T>, "T must be a QWidget or derived from QWidget");
		auto oldWidget = centralWidget();
		if (oldWidget)
		{
			oldWidget->hide();
			oldWidget->deleteLater();
		}

		setCentralWidget(new T(this));
	}

	Ui::AIMainWindowClass ui;
};
