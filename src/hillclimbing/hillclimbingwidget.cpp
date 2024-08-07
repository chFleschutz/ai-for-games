#include "hillclimbingwidget.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

HillClimbingWidget::HillClimbingWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// setup default chart
	m_fitnessSeries.append(0, 0);
	m_fitnessSeries.append(1, 2);
	m_fitnessSeries.append(2, 1);
	m_fitnessSeries.append(3, 3);
	m_fitnessSeries.append(4, 2);
	m_fitnessSeries.append(5, 4);

	m_points.setMarkerSize(15.0f);
	m_points.setMarkerShape(QScatterSeries::MarkerShapeCircle);
	m_points.setColor(Qt::red);

	m_startLine.setColor(Qt::red);

	m_chart.addSeries(&m_fitnessSeries);
	m_chart.addSeries(&m_momentumSeries);
	m_chart.addSeries(&m_fitnessMomentumSeries);
	m_chart.addSeries(&m_points);
	m_chart.addSeries(&m_startLine);
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
	const auto& values = m_hillClimbing.fitnessValues();
	if (values.empty())
	{
		QMessageBox::warning(this, "Error", "No values loaded");
		return;
	}

	float alpha = ui.alpha_spinBox->value();
	auto result = m_hillClimbing.run(m_startIndex, alpha);

	qDebug() << "Result: " << result << " " << values[result];
	ui.endIndex_label->setText(QString::number(result));

	m_points.clear();
	m_points.append(m_startIndex, values[m_startIndex]);
	m_points.append(result, values[result]);

	updateChart();
}

void HillClimbingWidget::onLoadDefaultValues(int index)
{
	switch (index)
	{
	case 0:
		loadValues(":/data_sets/random_set.txt");
		break;
	case 1:
		loadValues(":/data_sets/data_set1.txt");
		break;
	case 2:
		loadValues(":/data_sets/data_set2.txt");
		break;
	case 3:
		loadValues(":/data_sets/data_set3.txt");
		break;
	case 4:
		loadValues(":/data_sets/data_set4.txt");
		break;
	default:
		return;
	}

	if (m_startIndex >= m_hillClimbing.fitnessValues().size())
		m_startIndex = m_hillClimbing.fitnessValues().size() - 1;

	onRunHillClimbing();
}

void HillClimbingWidget::onStartSliderChanged(int value)
{
	m_startIndex = value;
	ui.startIndex_label->setText(QString::number(m_startIndex));

	onRunHillClimbing();
}

void HillClimbingWidget::onShowMomentum(bool checked)
{
	m_showMomentum = checked;
	if (!checked)
		m_momentumSeries.clear();

	updateChart();
}

void HillClimbingWidget::onShowFitnessMomentum(bool checked)
{
	m_showFitnessMomentum = checked;
	if (!checked)
		m_fitnessMomentumSeries.clear();
	
	updateChart();
}

void HillClimbingWidget::updateChart()
{
	m_minVal = 0.0f;
	m_maxVal = 0.0f;

	drawFitness();

	if (m_showMomentum)
		drawMomentum();
	if (m_showFitnessMomentum)
		drawFitnessMomentum();
	
	const auto& values = m_hillClimbing.fitnessValues();
	m_chart.axisX()->setRange(0, values.size() - 1);
	m_chart.axisY()->setRange(m_minVal, m_maxVal);

	drawStartLine();
	
	ui.chartView->update();
}

void HillClimbingWidget::loadValues(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Error", "Could not open file: " + filename);
		return;
	}

	QTextStream in(&file);
	if (in.atEnd())
	{
		QMessageBox::warning(this, "Error", "File is empty");
		file.close();
		return;
	}

	std::vector<float> values;
	while (!in.atEnd())
	{
		auto line = in.readLine();
		if (line.isEmpty())
			continue;

		values.push_back(line.toFloat());
	}
	m_hillClimbing.setValues(std::move(values));
	file.close();

	ui.start_slider->setMaximum(m_hillClimbing.fitnessValues().size() - 1);
}

void HillClimbingWidget::drawFitness()
{
	m_fitnessSeries.clear();

	const auto& values = m_hillClimbing.fitnessValues();
	for (int i = 0; i < values.size(); i++)
	{
		m_fitnessSeries.append(i, values[i]);

		updateChartBounds(values[i]);
	}
}

void HillClimbingWidget::drawMomentum()
{
	m_momentumSeries.clear();

	const auto& values = m_hillClimbing.momentumValues();
	for (int i = 0; i < values.size(); i++)
	{
		m_momentumSeries.append(i, values[i]);

		updateChartBounds(values[i]);
	}
}

void HillClimbingWidget::drawFitnessMomentum()
{
	m_fitnessMomentumSeries.clear();

	const auto& values = m_hillClimbing.fitnessMomentumValues();
	for (int i = 0; i < values.size(); i++)
	{
		m_fitnessMomentumSeries.append(i, values[i]);

		updateChartBounds(values[i]);
	}
}

void HillClimbingWidget::drawStartLine()
{
	m_startLine.clear();
	m_startLine.append(m_startIndex, m_minVal);
	m_startLine.append(m_startIndex, m_maxVal);
}

void HillClimbingWidget::updateChartBounds(float value)
{
	if (value < m_minVal)
		m_minVal = value;

	if (value > m_maxVal)
		m_maxVal = value;
}
