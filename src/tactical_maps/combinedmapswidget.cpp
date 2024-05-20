#include "combinedmapswidget.h"

CombinedMapsWidget::CombinedMapsWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_imageRenderer = ui.imageRenderer;

	ui.mapSize_spinBox->setValue(m_cellCount);
	ui.maxValue_checkBox->setChecked(m_showMaxValue);
	ui.showFriendly_checkBox->setChecked(m_showFriendlyUnits);
	ui.showEnemy_checkBox->setChecked(m_showEnemyUnits);
	ui.combined_radioButton->setChecked(true);
	ui.friendlyUnit_radioButton->setChecked(true);

	connect(m_imageRenderer, &ImageRendererWidget::onPaintEvent, this, &CombinedMapsWidget::onDrawMap, Qt::DirectConnection);
	connect(m_imageRenderer, &ImageRendererWidget::onDoubleClicked, this, &CombinedMapsWidget::onImageDoubleClicked, Qt::DirectConnection);
	connect(m_imageRenderer, &ImageRendererWidget::onResized, this, &CombinedMapsWidget::onImageResize, Qt::DirectConnection);

	setupMaps();
	onMapIndexChanged(0);
}

void CombinedMapsWidget::onMapIndexChanged(int index)
{
	auto& renderer = m_imageRenderer->renderer();

	switch (index)
	{
	case 0:
		renderer.load(":/images/simplemap.png", size());
		break;
	case 1:
		renderer.load(":/images/obstacles.png", size());
		break;
	default:
		return;
	}

	m_combinedMap.resize(renderer.image(), m_cellCount, m_cellCount);
	m_imageRenderer->resize(size());
	update();
}

void CombinedMapsWidget::onCellCountChanged(int value)
{
	m_cellCount = value;
	m_combinedMap.resize(m_imageRenderer->renderer().image(), m_cellCount, m_cellCount);
	update();
}

void CombinedMapsWidget::onShowCellValuesChanged(bool value)
{
	m_showCellValues = value;
	update();
}

void CombinedMapsWidget::onShowMaxValueChanged(bool value)
{
	m_showMaxValue = value;
	update();
}

void CombinedMapsWidget::onShowFriendlyChanged(bool value)
{
	m_showFriendlyUnits = value;
	update();
}

void CombinedMapsWidget::onShowEnemyChanged(bool value)
{
	m_showEnemyUnits = value;
	update();
}

void CombinedMapsWidget::onShowCombinedMap()
{
	m_currentMap = MapType::Combined;
	update();
}

void CombinedMapsWidget::onShowVisibilityMap()
{
	m_currentMap = MapType::Visibility;
	update();
}

void CombinedMapsWidget::onShowDistanceMap()
{
	m_currentMap = MapType::Distance;
	update();
}

void CombinedMapsWidget::onShowSecurityMap()
{
	m_currentMap = MapType::Security;
	update();
}

void CombinedMapsWidget::onShowFriendlyProximityMap()
{
	m_currentMap = MapType::FriendlyProximity;
	update();
}

void CombinedMapsWidget::onShowEnemyProximityMap()
{
	m_currentMap = MapType::EnemyProximity;
	update();
}

void CombinedMapsWidget::onSetFriendlyUnit()
{
	m_currentUnit = UnitType::Friendly;
}

void CombinedMapsWidget::onSetEnemyUnit()
{
	m_currentUnit = UnitType::Enemy;
}

void CombinedMapsWidget::onClearUnits()
{
	m_combinedMap.clearUnits();
	update();
}

void CombinedMapsWidget::onDrawMap(QPainter& painter)
{
	drawCells(painter);
	drawUnits(painter);
}

void CombinedMapsWidget::onImageDoubleClicked(QMouseEvent* event)
{
	auto percentX = event->pos().x() / static_cast<float>(m_imageRenderer->imageWidth());
	auto percentY = event->pos().y() / static_cast<float>(m_imageRenderer->imageHeight());
	CellCoord cellCoord{ static_cast<uint32_t>(percentX * m_cellCount), static_cast<uint32_t>(percentY * m_cellCount) };

	switch (m_currentUnit)
	{
	default:
	case CombinedMapsWidget::UnitType::Friendly:
		m_friendlyProximityMap->addUnit(cellCoord);
		break;
	case CombinedMapsWidget::UnitType::Enemy:
		m_distanceMap->addUnit(cellCoord);
		m_enemyProximityMap->addUnit(cellCoord);
		break;
	}

	update();
}

void CombinedMapsWidget::onImageResize(QResizeEvent* event)
{
	float pixelPerCellX = m_imageRenderer->imageWidth() / static_cast<float>(m_cellCount);
	float pixelPerCellY = m_imageRenderer->imageHeight() / static_cast<float>(m_cellCount);
	m_pixelPerCell = QPointF(pixelPerCellX, pixelPerCellY);
}

QPoint CombinedMapsWidget::pixelPos(CellCoord coord) const
{
	auto cellX = coord.x * m_pixelPerCell.x() + (m_pixelPerCell.x() * 0.5f);
	auto cellY = coord.y * m_pixelPerCell.y() + (m_pixelPerCell.y() * 0.5f);
	return QPoint(cellX, cellY);
}

QRect CombinedMapsWidget::cellRect(CellCoord coord) const
{
	float cellX = coord.x * m_pixelPerCell.x() + (m_pixelPerCell.x() * 0.5f);
	float cellY = coord.y * m_pixelPerCell.y() + (m_pixelPerCell.y() * 0.5f);
	float cellWidth = m_pixelPerCell.x() * m_cellSize;
	float cellHeight = m_pixelPerCell.y() * m_cellSize;
	return QRect(cellX - (cellWidth * 0.5f), cellY - (cellHeight * 0.5f), cellWidth, cellHeight);
}

void CombinedMapsWidget::drawCells(QPainter& painter)
{
	auto map = currentMap();
	auto color = currentColor();

	Q_ASSERT_X(map, "CombinedMapsWidget::onDrawMap", "Current map is not valid");

	CellCoord maxCell{};
	float maxValue = 0.0f;
	for (uint32_t y = 0; y < m_cellCount; ++y)
	{
		for (uint32_t x = 0; x < m_cellCount; ++x)
		{
			CellCoord coord{ x, y };
			auto rect = cellRect(coord);

			if (map->isObstacle(x, y))
			{
				painter.fillRect(rect, Qt::black);
				continue;
			}

			auto cellValue = map->valueAt(x, y);
			if (cellValue > maxValue)
			{
				maxValue = cellValue;
				maxCell = CellCoord{ x, y };
			}

			color.setAlphaF(cellValue);
			painter.fillRect(rect, color);

			if (m_showCellValues)
			{
				painter.setPen(Qt::black);
				painter.drawText(rect, Qt::AlignCenter, QString::number(cellValue, 'f', 2));
			}
		}
	}

	if (m_showMaxValue)
	{
		painter.setRenderHint(QPainter::RenderHint::Antialiasing);
		painter.setPen(QPen(Qt::red, 2));
		painter.drawRect(cellRect(maxCell));
	}
}

void CombinedMapsWidget::drawUnits(QPainter& painter)
{
	int unitSize = 4;
	painter.setRenderHint(QPainter::RenderHint::Antialiasing);
	painter.setPen(QPen(Qt::black));

	if (m_showFriendlyUnits)
	{
		painter.setBrush(QBrush(Qt::green));
		for (const auto& unit : m_friendlyProximityMap->units())
		{
			auto pos = pixelPos(unit);
			painter.drawEllipse(pos, unitSize, unitSize);
		}
	}

	if (m_showEnemyUnits)
	{
		painter.setBrush(QBrush(Qt::red));
		for (const auto& unit : m_enemyProximityMap->units())
		{
			auto pos = pixelPos(unit);
			painter.drawEllipse(pos, unitSize, unitSize);
		}
	}
}

void CombinedMapsWidget::setupMaps()
{
	m_visibilityMap = std::make_shared<TacticalMap>();
	m_visibilityMap->setType(TacticalMap::Type::Visibility);

	m_distanceMap = std::make_shared<TacticalMap>();
	m_distanceMap->setType(TacticalMap::Type::Distance);
	InfluenceFunction::Limits limits{};
	limits.exponent = 3.0f;
	m_distanceMap->setInfluenceLimits(limits);

	m_friendlyProximityMap = std::make_shared<TacticalMap>();
	m_friendlyProximityMap->setType(TacticalMap::Type::Proximity);

	m_enemyProximityMap = std::make_shared<TacticalMap>();
	m_enemyProximityMap->setType(TacticalMap::Type::Proximity);

	m_securityMap = std::make_shared<CombinedTacticalMap>();
	m_securityMap->setCombineFunction<DifferenceCombine>();

	// Order matters because DifferenceCombine uses (a - b)
	m_securityMap->addMap(m_enemyProximityMap);
	m_securityMap->addMap(m_friendlyProximityMap);

	m_combinedMap.addMap(m_securityMap);
	m_combinedMap.addMap(m_visibilityMap);
	m_combinedMap.addMap(m_distanceMap);
	m_combinedMap.setCombineFunction<ProductCombine>();
}

TacticalMapBase* CombinedMapsWidget::currentMap()
{
	switch (m_currentMap)
	{
	default:
	case CombinedMapsWidget::MapType::Combined:
		return &m_combinedMap;
	case CombinedMapsWidget::MapType::Visibility:
		return m_visibilityMap.get();
	case CombinedMapsWidget::MapType::Distance:
		return m_distanceMap.get();
	case CombinedMapsWidget::MapType::Security:
		return m_securityMap.get();
	case CombinedMapsWidget::MapType::FriendlyProximity:
		return m_friendlyProximityMap.get();
	case CombinedMapsWidget::MapType::EnemyProximity:
		return m_enemyProximityMap.get();
	}
}

QColor CombinedMapsWidget::currentColor()
{
	switch (m_currentMap)
	{
	default:
	case CombinedMapsWidget::MapType::Combined:
		return QColor(255, 255, 255);
	case CombinedMapsWidget::MapType::Visibility:
		return QColor(255, 255, 0);
	case CombinedMapsWidget::MapType::Distance:
		return QColor(255, 0, 255);
	case CombinedMapsWidget::MapType::Security:
		return QColor(0, 0, 255);
	case CombinedMapsWidget::MapType::FriendlyProximity:
		return QColor(0, 255, 0);
	case CombinedMapsWidget::MapType::EnemyProximity:
		return QColor(255, 0, 0);
	}
}
