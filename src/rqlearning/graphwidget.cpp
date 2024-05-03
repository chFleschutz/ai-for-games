#include "graphwidget.h"

#include <QPainter>
#include <QMouseEvent>

GraphWidget::GraphWidget(QWidget* parent)
	: QWidget(parent)
{
}

void GraphWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setPen(QPen(Qt::white, 1));
	painter.setBrush(Qt::white);
	painter.drawRect(0, 0, width() - 1, height() - 1);

	for (const auto& edge : m_graph.edges())
	{
		auto start = m_graph.node(edge.from).position;
		auto end = m_graph.node(edge.to).position;

		painter.setPen(QPen(Qt::darkGray, 2));
		painter.drawLine(start, end);
	}

	for (const auto& node : m_graph.nodes())
	{
		painter.setPen(QPen(Qt::cyan, 1));
		painter.setBrush(Qt::cyan);
		painter.drawEllipse(node.position, m_nodeRadius, m_nodeRadius);

		painter.setPen(QPen(Qt::black, 1));
		painter.drawText(node.position - QPoint(-5, 0), QString::number(node.id));
	}
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
	auto pos = event->pos();

	auto& nodes = m_graph.nodes();
	for (auto& node : nodes)
	{
		if ((pos - node.position).manhattanLength() <= m_nodeRadius)
		{
			m_selectedNode = &node;
			setCursor(Qt::ClosedHandCursor);
			break;
		}
	}
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_selectedNode)
	{
		m_selectedNode->position = event->pos();
		update();
	}
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_selectedNode)
	{
		m_selectedNode = nullptr;
		setCursor(Qt::ArrowCursor);
	}
}
