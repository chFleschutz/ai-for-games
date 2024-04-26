#include "hillclimbingwidget.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

HillClimbingWidget::HillClimbingWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// set up chart
	m_series.append(0, 0);
	m_series.append(1, 2);
	m_series.append(2, 1);
	m_series.append(3, 3);
	m_series.append(4, 2);
	m_series.append(5, 4);

	m_points.setMarkerSize(15.0f);
	m_points.setMarkerShape(QScatterSeries::MarkerShapeCircle);
	m_points.setColor(Qt::red);

	m_chart.addSeries(&m_series);
	m_chart.addSeries(&m_points);
	m_chart.createDefaultAxes();
	m_chart.legend()->hide();

	ui.chartView->setChart(&m_chart);
	ui.chartView->setRenderHint(QPainter::Antialiasing);

	ui.defaultSet_comboBox->setCurrentIndex(0);
	onLoadDefaultValues(0);
}

void HillClimbingWidget::onLoadValues()
{
	auto path = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt)");
	if (path.isEmpty())
		return;

	loadValues(path);
	updateChart();
}

void HillClimbingWidget::onRunHillClimbing()
{
	if (m_values.empty())
	{
		QMessageBox::warning(this, "Error", "No values loaded");
		return;
	}

	auto startIndex = ui.startPos_spinBox->value();
	auto result = HillClimbing::findMaxValue(m_values, startIndex);
	qDebug() << "Result: " << result << " " << m_values[result];

	m_points.clear();
	m_points.append(startIndex, m_values[startIndex]);
	m_points.append(result, m_values[result]);
}

void HillClimbingWidget::onResetHillClimbing()
{
	m_points.clear();
}

void HillClimbingWidget::onLoadDefaultValues(int index)
{
	switch (index)
	{
	case 0:
		loadValues(":/assets/data_sets/random_set.txt");
		break;
	case 1:
		loadValues(":/assets/data_sets/data_set_1.txt");
		break;
	case 2:
		loadValues(":/assets/data_sets/data_set_2.txt");
		break;
	case 3:
		loadValues(":/assets/data_sets/data_set_3.txt");
		break;
	case 4:
		loadValues(":/assets/data_sets/data_set_4.txt");
		break;
	default:
		return;
	}

	updateChart();
}

void HillClimbingWidget::updateChart()
{
	m_points.clear();
	m_series.clear();

	if (m_values.empty())
		return;

	float minVal = 0.0f;
	float maxVal = 0.0f;
	for (int i = 0; i < m_values.size(); ++i)
	{
		auto value = m_values[i];
		m_series.append(i, value);

		if (value < minVal)
			minVal = value;

		if (value > maxVal)
			maxVal = value;
	}

	m_chart.axisX()->setRange(0, m_values.size());
	m_chart.axisY()->setRange(minVal, maxVal);
	ui.chartView->update();
}

void HillClimbingWidget::loadValues(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Error", "Could not open file");
		return;
	}

	QTextStream in(&file);
	if (in.atEnd())
	{
		QMessageBox::warning(this, "Error", "File is empty");
		file.close();
		return;
	}

	m_values.clear();
	while (!in.atEnd())
	{
		auto line = in.readLine();
		if (line.isEmpty())
			continue;

		m_values.push_back(line.toFloat());
	}

	file.close();
}
