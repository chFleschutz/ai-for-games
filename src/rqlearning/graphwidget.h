#pragma once

#include "graph.h"

#include <QWidget>

class GraphWidget : public QWidget
{
	Q_OBJECT

public:
	GraphWidget(QWidget *parent = nullptr);
	~GraphWidget() = default;

	Graph& graph() { return m_graph; }

protected:
	virtual void paintEvent(QPaintEvent *event) override;

	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	Graph m_graph;

	Graph::Node* m_selectedNode = nullptr;

	int m_nodeRadius = 30.0f;
};
