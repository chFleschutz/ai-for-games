#pragma once

#include "graph.h"

#include <QWidget>
#include <QColor>

class GraphWidget : public QWidget
{
	Q_OBJECT

public:
	GraphWidget(QWidget *parent = nullptr);
	~GraphWidget() = default;

	Graph& graph() { return m_graph; }

public slots:
	void addNode();
	void addEdge(uint32_t from, uint32_t to);
	void clear();

signals:
	void nodeAdded();
	void edgeAdded();

protected:
	virtual void paintEvent(QPaintEvent *event) override;

	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	Graph::Node* nodeAt(const QPoint& pos);
	void selectNode(Graph::Node* node);
	void deselectNode();

	Graph m_graph;
	Graph::Node* m_selectedNode = nullptr;

	int m_nodeRadius = 30.0f;
	
	QColor m_nodeColor = QColor(52, 131, 235);
	QColor m_selectedNodeColor = QColor(37, 219, 159);
	QColor m_nodeTextColor = Qt::white;
	QColor m_edgeColor = Qt::darkGray;
};
