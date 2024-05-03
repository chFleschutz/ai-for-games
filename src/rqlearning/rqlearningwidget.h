#pragma once

#include "rqlearning/rqlearning.h"
#include "ui_rqlearningwidget.h"

#include <QTableWidget>
#include <QTimer>
#include <QWidget>

class RQLearningWidget : public QWidget
{
	Q_OBJECT

public:
	RQLearningWidget(QWidget* parent = nullptr);
	~RQLearningWidget() = default;

public slots:
	void onTrainButtonClicked();
	void onResetButtonClicked();
	void onAlphaChanged(double value);
	void onGammaChanged(double value);
	void onMaxIterationsChanged(int value);
	void onDelayChanged(int value);
	void onTrain();
	void updateLearning();
	void onLoadGraph(int index);

private:
	void initTableWidgets();
	void updateTableWidgets();
	void loadTestGraph();
	void loadLectureGraph();

	Ui::RQLearningWidgetClass ui;
	GraphWidget* m_graphWidget;
	QTableWidget* m_qTableWidget;
	QTableWidget* m_rTableWidget;

	RQLearning m_rqLearning;

	float m_alpha = 0.1f;
	float m_gamma = 0.9f;

	int m_maxIterations = 1000;

	QTimer m_traintimer;
	int m_iteration = 0;
	int m_delay = 5;
};
