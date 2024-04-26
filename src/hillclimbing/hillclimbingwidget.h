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
	void onResetHillClimbing();
	void onLoadDefaultValues(int index);

private:
	void updateChart();
	void loadValues(const QString& filename);

	Ui::HillClimbingWidgetClass ui;

	std::vector<float> m_values;

	QChart m_chart;
	QLineSeries m_series;
	QScatterSeries m_points;
};
