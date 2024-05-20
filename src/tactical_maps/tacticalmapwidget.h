#pragma once

#include "tactical_maps/tacticalmap.h"
#include "ui_tacticalmapwidget.h"
#include "utils/imagerendererwidget.h"

#include <QWidget>

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
	
	void onInfluenceTypeGlobal();
	void onInfluenceTypeProximity();
	void onInfluenceTypeLineOfSight();

	void onMaxDistanceChanged(double value);
	void onMaxInfluenceChanged(double value);
	void onOffsetChanged(double value);
	void onExponentChanged(double value);

	void onInfluenceMillington();
	void onInfluenceMark();
	void onInfluenceBichlmeier();

	void onClearUnits();

private:
	Ui::TacticalMapWidgetClass ui;

	ImageRendererWidget* m_imageRenderer = nullptr;

	TacticalMap m_map;
	int m_cellCount = 20;
	float m_cellSize = 0.9f;
	bool m_showCellValues = false;

	QColor m_color = QColor(125, 232, 93, 150);
};
