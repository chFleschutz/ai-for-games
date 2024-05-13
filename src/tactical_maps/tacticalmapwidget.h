#pragma once

#include <QWidget>
#include "ui_tacticalmapwidget.h"
#include "tactical_maps/tacticalmap.h"
#include "utils/imagerendererwidget.h"

class TacticalMapWidget : public QWidget
{
	Q_OBJECT

public:
	TacticalMapWidget(QWidget* parent = nullptr);
	~TacticalMapWidget() = default;

public slots:
	void onMapDoubleClicked(QMouseEvent* event);
	void onDrawMap(QPainter& painter);
	void onCellCountChanged(int value);
	void onCellSizeChanged(int value);
	void onMapIndexChanged(int index);
	void onShowCellValuesChanged(bool value);
	void onInfluenceLinear();
	void onInfluenceQuadratic();
	void onInfluenceSqrt();

private:
	Ui::TacticalMapWidgetClass ui;

	ImageRendererWidget* m_imageRenderer = nullptr;

	TacticalMap m_map;
	int m_cellCount = 20;
	float m_cellSize = 0.9f;
	bool m_showCellValues = false;

	QColor m_color = QColor(125, 232, 93, 150);
};
