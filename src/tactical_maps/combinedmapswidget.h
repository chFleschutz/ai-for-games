#pragma once

#include "tactical_maps/combinedtacticalmap.h"
#include "ui_combinedmapswidget.h"
#include "utils/imagerendererwidget.h"

#include <QWidget>

class CombinedMapsWidget : public QWidget
{
	Q_OBJECT

public:
	enum class MapType
	{
		Combined,
		Visibility,
		Distance,
		Security,
		FriendlyProximity,
		EnemyProximity
	};

	enum class UnitType
	{
		Friendly,
		Enemy
	};

	CombinedMapsWidget(QWidget* parent = nullptr);
	~CombinedMapsWidget() = default;

public slots:
	void onMapIndexChanged(int index);
	void onCellCountChanged(int value);
	void onShowCellValuesChanged(bool value);
	void onShowMaxValueChanged(bool value);
	void onShowFriendlyChanged(bool value);
	void onShowEnemyChanged(bool value);

	void onShowCombinedMap();
	void onShowVisibilityMap();
	void onShowDistanceMap();
	void onShowSecurityMap();
	void onShowFriendlyProximityMap();
	void onShowEnemyProximityMap();

	void onSetFriendlyUnit();
	void onSetEnemyUnit();
	void onClearUnits();

	void onDrawMap(QPainter& painter);
	void onImageDoubleClicked(QMouseEvent* event);

private:
	QPoint pixelPos(CellCoord coord, float pixelPerCellX, float pixelPerCellY) const;
	QRect cellRect(CellCoord coord, float pixelPerCellX, float pixelPerCellY) const;
	void drawUnits(QPainter& painter, float pixelPerCellX, float pixelPerCellY);
	void setupMaps();
	TacticalMapBase* currentMap();
	QColor currentColor();

	Ui::CombinedMapsWidgetClass ui;

	ImageRendererWidget* m_imageRenderer = nullptr;
	int m_cellCount = 30;
	float m_cellSize = 0.9f;
	bool m_showCellValues = false;
	bool m_showMaxValue = false;
	bool m_showFriendlyUnits = true;
	bool m_showEnemyUnits = true;
	MapType m_currentMap = MapType::Combined;
	UnitType m_currentUnit = UnitType::Friendly;

	// Map containing all maps
	CombinedTacticalMap m_combinedMap;

	// Pointer to the individual maps
	std::shared_ptr<CombinedTacticalMap> m_securityMap;
	std::shared_ptr<TacticalMap> m_friendlyProximityMap;
	std::shared_ptr<TacticalMap> m_enemyProximityMap;
	std::shared_ptr<TacticalMap> m_visibilityMap;
	std::shared_ptr<TacticalMap> m_distanceMap;

	QColor m_colorSecurity = QColor(0, 0, 255);
	QColor m_colorVisibility = QColor(255, 0, 0);
	QColor m_colorDistance = QColor(0, 255, 0);
};
