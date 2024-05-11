#include "graphwidget.h"

#include <QPainter>
#include <QMouseEvent>

GraphWidget::GraphWidget(QWidget* parent)
	: QWidget(parent)
{
}

void GraphWidget::addNode()
{
	QPoint pos(width() / 2, height() / 2);
	m_graph.addNode(pos);
	emit nodeAdded();
	update();
}

void GraphWidget::addEdge(uint32_t from, uint32_t to)
{
	m_graph.addEdge(from, to);
	emit edgeAdded();
	update();
}

void GraphWidget::clear()
{
	m_graph.clear();
	emit cleared();
	update();
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

		painter.setPen(QPen(m_edgeColor, 2));
		painter.drawLine(start, end);
	}

	for (const auto& node : m_graph.nodes())
	{
		auto color = m_nodeColor;
		if (&node == m_selectedNode)
			color = m_selectedNodeColor;

		painter.setPen(QPen(color, 1));
		painter.setBrush(color);
		painter.drawEllipse(node.position, m_nodeRadius, m_nodeRadius);

		painter.setPen(QPen(m_nodeTextColor, 1));
		painter.drawText(node.position, QString::number(node.id));
	}
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
	auto node = nodeAt(event->pos());
	if (!node)
		return;

	selectNode(node);
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_selectedNode)
		return;

	m_selectedNode->position = event->pos();
	update();
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
	deselectNode();
}

Graph::Node* GraphWidget::nodeAt(const QPoint& pos)
{
	auto& nodes = m_graph.nodes();
	for (auto& node : nodes)
	{
		if ((pos - node.position).manhattanLength() <= m_nodeRadius)
			return &node;
	}

	return nullptr;
}

void GraphWidget::selectNode(Graph::Node* node)
{
	m_selectedNode = node;
	setCursor(Qt::ClosedHandCursor);
	update();
}

void GraphWidget::deselectNode()
{
	m_selectedNode = nullptr;
	setCursor(Qt::ArrowCursor);
	update();
}
