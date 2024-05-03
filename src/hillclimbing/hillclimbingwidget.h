#pragma once

#include "hillclimbing/hillclimbing.h"
#include "ui_hillclimbingwidget.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QWidget>

#include <vector>

class HillClimbingWidget : public QWidget
{
	Q_OBJECT

public:
	HillClimbingWidget(QWidget* parent = nullptr);
	~HillClimbingWidget() = default;

public slots:
	void onLoadValues();
	void onRunHillClimbing();
	void onLoadDefaultValues(int index);
	void onStartSliderChanged(int value);
	void onShowMomentum(bool checked);
	void onShowFitnessMomentum(bool checked);
	void updateChart();

private:
	void loadValues(const QString& filename);
	void drawFitness();
	void drawMomentum();
	void drawFitnessMomentum();
	void drawStartLine();
	void updateChartBounds(float value);

	Ui::HillClimbingWidgetClass ui;

	QChart m_chart;
	QLineSeries m_fitnessSeries;
	QLineSeries m_momentumSeries;
	QLineSeries m_fitnessMomentumSeries;
	QScatterSeries m_points;
	QLineSeries m_startLine;

	bool m_showMomentum = false;
	bool m_showFitnessMomentum = false;

	HillClimbing m_hillClimbing;

	int m_startIndex = 0;
	float m_minVal = 0.0f;
	float m_maxVal = 0.0f;
};
