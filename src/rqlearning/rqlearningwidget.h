#pragma once

#include <QWidget>
#include "ui_rqlearningwidget.h"
#include "rqlearning/rqlearning.h"

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

private:
	void initTableWidgets();
	void updateTableWidgets();
	void initTestGraph();

	Ui::RQLearningWidgetClass ui;
	GraphWidget* m_graphWidget;
	QTableWidget* m_qTableWidget;
	QTableWidget* m_rTableWidget;

	RQLearning m_rqLearning;

	float m_alpha = 0.1f;
	float m_gamma = 0.9f;

	int m_maxIterations = 1000;
};
