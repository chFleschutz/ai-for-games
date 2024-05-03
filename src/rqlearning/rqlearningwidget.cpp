#include "rqlearningwidget.h"

RQLearningWidget::RQLearningWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.alpha_spinBox->setValue(m_alpha);
	ui.gamma_spinBox->setValue(m_gamma);
	ui.iterations_spinBox->setValue(m_maxIterations);
	ui.delay_spinBox->setValue(m_delay);

	m_graphWidget = ui.graph_widget;

	m_rTableWidget = ui.rMatrix_tableWidget;
	m_rTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_rTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	m_qTableWidget = ui.qMatrix_tableWidget;
	m_qTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_qTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(&m_traintimer, &QTimer::timeout, this, &RQLearningWidget::onTrain);
	connect(m_graphWidget, &GraphWidget::nodeAdded, this, &RQLearningWidget::updateLearning);
	connect(m_graphWidget, &GraphWidget::edgeAdded, this, &RQLearningWidget::updateLearning);

	initTestGraph();
	initTableWidgets();
	updateTableWidgets();
}

void RQLearningWidget::onTrainButtonClicked()
{
	if (m_delay > 2) // Avoid blocking the thread
	{
		m_iteration = m_maxIterations;
		m_traintimer.start(m_delay);
		return;
	}

	m_rqLearning.train(m_alpha, m_gamma, m_maxIterations);
	updateTableWidgets();
}

void RQLearningWidget::onResetButtonClicked()
{
	m_traintimer.stop();

	m_rqLearning.resetQ();
	updateTableWidgets();
}

void RQLearningWidget::onAlphaChanged(double value)
{
	m_alpha = value;
}

void RQLearningWidget::onGammaChanged(double value)
{
	m_gamma = value;
}

void RQLearningWidget::onMaxIterationsChanged(int value)
{
	m_maxIterations = value;
}

void RQLearningWidget::onDelayChanged(int value)
{
	m_delay = value;
}

void RQLearningWidget::onTrain()
{
	m_rqLearning.train(m_alpha, m_gamma);
	m_iteration--;

	if (m_iteration <= 0)
		m_traintimer.stop();

	updateTableWidgets();
}

void RQLearningWidget::initTableWidgets()
{
	m_rTableWidget->clear();
	m_qTableWidget->clear();

	auto size = m_rqLearning.size();
	m_rTableWidget->setRowCount(size);
	m_rTableWidget->setColumnCount(size);
	m_qTableWidget->setRowCount(size);
	m_qTableWidget->setColumnCount(size);

	// Set table headers
	for (int i = 0; i < size; i++)
	{
		m_rTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
		m_rTableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));

		m_qTableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
		m_qTableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
	}

	// Set table items
	for (int from = 0; from < size; from++)
	{
		for (int to = 0; to < size; to++)
		{
			auto item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignCenter);
			m_rTableWidget->setItem(from, to, item);

			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignCenter);
			m_qTableWidget->setItem(from, to, item);
		}
	}
}

void RQLearningWidget::updateTableWidgets()
{
	// Set table items
	auto size = m_rqLearning.size();
	for (int from = 0; from < size; from++)
	{
		for (int to = 0; to < size; to++)
		{
			// Set R matrix
			auto item = m_rTableWidget->item(from, to);
			if (item)
			{
				auto r = m_rqLearning.r(from, to);
				QString text;
				if (r >= 0.0f)
					text = QString::number(r);

				item->setText(text);
			}

			// Set Q matrix
			item = m_qTableWidget->item(from, to);
			if (item)
			{
				auto q = m_rqLearning.q(from, to);
				item->setText(QString::number(q, 'g', 3));
			}
		}
	}
}

void RQLearningWidget::updateLearning()
{
	auto& graph = m_graphWidget->graph();
	m_rqLearning.init(graph.nodeCount());

	// Set all possible transitions to 0
	for (const auto& edge : graph.edges())
	{
		m_rqLearning.setR(edge.from, edge.to, 0.0f);
	}

	initTableWidgets();
	updateTableWidgets();
}

void RQLearningWidget::initTestGraph()
{
	auto& graph = m_graphWidget->graph();

	graph.addNode(QPoint(100, 100));
	graph.addNode(QPoint(200, 100));
	graph.addNode(QPoint(100, 200));
	graph.addNode(QPoint(200, 200));
	graph.addNode(QPoint(300, 100));

	graph.addEdge(0, 1);
	graph.addEdge(0, 2);
	graph.addEdge(0, 3);
	graph.addEdge(1, 0);
	graph.addEdge(1, 2);
	graph.addEdge(1, 3);
	graph.addEdge(2, 0);
	graph.addEdge(2, 1);
	graph.addEdge(2, 3);
	graph.addEdge(3, 1);
	graph.addEdge(3, 2);
	graph.addEdge(3, 4);
	graph.addEdge(4, 0);

	updateLearning();

	// Set rewards for some transitions
	m_rqLearning.setR(4, 4, 1.0f);
	m_rqLearning.setR(3, 4, 1.0f);

	update();
}
