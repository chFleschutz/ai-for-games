#pragma once

#include "flowfield/flowfield.h"
#include "flowfield/flowfieldagent.h"
#include "ui_flowfieldwidget.h"
#include "utils/imagerendererwidget.h"

#include <QPainter>
#include <QWidget>
#include <QElapsedTimer>

class FlowFieldWidget : public QWidget
{
	Q_OBJECT

public:
	enum FlowFieldStats
	{
		TRAVEL_COSTS,
		INTEGRATION_COSTS,
		FLOW_DIRECTION,
		STATS_HIDDEN
	};

	FlowFieldWidget(QWidget *parent = nullptr);
	~FlowFieldWidget() = default;

	FlowField::Coordinate pixelToCellCoord(const QPoint& pos) const;
	FlowField::Coordinate pixelToCellCoord(const QVector2D& pos) const;

	const FlowField& flowField() const { return m_flowField; }

public slots:
	void onSetMap(int mapIndex);
	void onChangeFlowFieldWidth(int width);
	void onChangeFlowFieldHeight(int height);
	void onResetField();
	
	void onShowCells(bool show);
	void onSetCellSize(int size);
	void onShowTravelCosts();
	void onShowIntegrationCosts();
	void onShowFlowDirection();
	void onHideCosts();

	void onSetAgentCount(int count);
	void onSpawnRandomAgents();
	void onClearAgents();
	void onFreezeAgents(bool freeze);

	void onImagePaint(QPainter& painter);
	void onImageDoubleClicked(QMouseEvent* event);
	void onImageResized(QResizeEvent* event);

	void onAgentUpdate();

private:
	void updateFlowField();
	void drawCellValues(QPainter& painter);
	void drawCosts(QPainter& painter, const FlowField::Cell& cell, float cellSize);
	int digits(int x, int base = 10);
	float toAngle(const QVector2D& direction);
	void setPixelPerCell();

	Ui::FlowFieldWidgetClass ui;

	ImageRendererWidget* m_imageWidget = nullptr;

	FlowField m_flowField;
	int m_fieldWidth = 20;
	int m_fieldHeight = 20;
	QVector2D m_pixelPerCell;

	bool m_showCells = false;
	float m_cellSize = 1.0f;
	FlowFieldStats m_showCosts = STATS_HIDDEN;

	AgentManager m_agentManager;
	int m_agentCount = 1000;

	QElapsedTimer m_frameTimer;
	bool m_frozen = false;
};
