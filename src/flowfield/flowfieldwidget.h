#pragma once

#include "flowfield.h"
#include "ui_flowfieldwidget.h"
#include "utils/imagerendererwidget.h"

#include <QPainter>
#include <QWidget>

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

public slots:
	void onChangeFlowFieldWidth(int width);
	void onChangeFlowFieldHeight(int height);
	void onShowCells(bool show);
	void onSetCellSize(int size);
	void onSetMap(int mapIndex);
	void onShowTravelCosts();
	void onShowIntegrationCosts();
	void onShowFlowDirection();
	void onHideCosts();
	void onResetField();

	void onImagePaint(QPainter& painter);
	void onImageDoubleClicked(QMouseEvent* event);

private:
	void drawCellValues(QPainter& painter);
	void drawCosts(QPainter& painter, FlowField::Cell& cell, float cellSize, float posX, float posY);
	int digits(int x, int base = 10);
	float toAngle(const QVector2D& direction);

	Ui::FlowFieldWidgetClass ui;

	ImageRendererWidget* m_imageWidget = nullptr;

	FlowField m_cellField;
	int m_fieldWidth = 20;
	int m_fieldHeight = 20;

	bool m_showCells = false;
	float m_cellSize = 1.0f;
	FlowFieldStats m_showCosts = STATS_HIDDEN;
};
