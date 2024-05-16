#include "tacticalmapwidget.h"

#include "tactical_maps/influencefunctions.h"

#include <QPainter>

TacticalMapWidget::TacticalMapWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_imageRenderer = ui.imageRenderer;
	Q_ASSERT(m_imageRenderer);

	ui.map_comboBox->setCurrentIndex(0);
	ui.mapSize_spinBox->setValue(m_cellCount);
	ui.cellSize_slider->setValue(m_cellSize * 100);

	ui.global_radioButton->setChecked(true);
	ui.millington_radioButton->setChecked(true);

	auto& limits = m_map.influenceLimits();
	ui.maxDis_spinBox->setValue(limits.maxDistance);
	ui.offset_spinBox->setValue(limits.offset);
	ui.exponent_spinBox->setValue(limits.exponent);

	connect(m_imageRenderer, &ImageRendererWidget::onPaintEvent, this, &TacticalMapWidget::onDrawMap, Qt::DirectConnection);
	connect(m_imageRenderer, &ImageRendererWidget::onDoubleClicked, this, &TacticalMapWidget::onMapDoubleClicked);

	onMapIndexChanged(0);
	onCellCountChanged(m_cellCount);
}

void TacticalMapWidget::onMapDoubleClicked(QMouseEvent* event)
{
	auto pos = event->pos();
	auto percentX = pos.x() / static_cast<float>(m_imageRenderer->imageWidth());
	auto percentY = pos.y() / static_cast<float>(m_imageRenderer->imageHeight());

	auto cellX = static_cast<uint32_t>(percentX * m_map.field().width());
	auto cellY = static_cast<uint32_t>(percentY * m_map.field().height());

	m_map.addUnit(CellCoord{ cellX, cellY });
	update();
}

void TacticalMapWidget::onCellCountChanged(int value)
{
	m_cellCount = value;
	auto& image = m_imageRenderer->renderer().image();
	m_map.resize(image, m_cellCount, m_cellCount);
	update();
}

void TacticalMapWidget::onCellSizeChanged(int value)
{
	m_cellSize = value / 100.0f;
	update();
}

void TacticalMapWidget::onMapIndexChanged(int index)
{
	auto& image = m_imageRenderer->renderer();

	switch (index)
	{
	case 0:
		image.load(":/images/simplemap.png", size());
		break;
	case 1:
		image.load(":/images/obstacles.png", size());
		break;
	default:
		return;
	}

	m_map.resize(image.image(), m_cellCount, m_cellCount);
	update();
}

void TacticalMapWidget::onShowCellValuesChanged(bool value)
{
	m_showCellValues = value;
	update();
}

void TacticalMapWidget::onInfluenceTypeGlobal()
{
	m_map.setInfluenceType(TacticalMap::InfluenceType::Global);
	update();
}

void TacticalMapWidget::onInfluenceTypeProximity()
{
	m_map.setInfluenceType(TacticalMap::InfluenceType::Proximity);
	update();
}

void TacticalMapWidget::onInfluenceTypeLineOfSight()
{
	m_map.setInfluenceType(TacticalMap::InfluenceType::LineOfSight);
	update();
}

void TacticalMapWidget::onMaxDistanceChanged(double value)
{
	InfluenceFunction::Limits limits = m_map.influenceLimits();
	limits.maxDistance = static_cast<float>(value);
	m_map.setInfluenceLimits(limits);
	update();
}

void TacticalMapWidget::onOffsetChanged(double value)
{
	InfluenceFunction::Limits limits = m_map.influenceLimits();
	limits.offset = static_cast<float>(value);
	m_map.setInfluenceLimits(limits);
	update();
}

void TacticalMapWidget::onExponentChanged(double value)
{
	InfluenceFunction::Limits limits = m_map.influenceLimits();
	limits.exponent = static_cast<float>(value);
	m_map.setInfluenceLimits(limits);
	update();
}

void TacticalMapWidget::onInfluenceMillington()
{
	m_map.setInfluenceFunction<MillingtonInfluence>();
	update();
}

void TacticalMapWidget::onInfluenceMark()
{
	m_map.setInfluenceFunction<MarkInfluence>();
	update();
}

void TacticalMapWidget::onInfluenceBichlmeier()
{
	m_map.setInfluenceFunction<BichlmeierInfluence>();
	update();
}

void TacticalMapWidget::onClearUnits()
{
	m_map.clearUnits();
	update();
}

void TacticalMapWidget::onDrawMap(QPainter& painter)
{
	float pixelPerCellX = m_imageRenderer->imageWidth() / static_cast<float>(m_map.field().width());
	float pixelPerCellY = m_imageRenderer->imageHeight() / static_cast<float>(m_map.field().height());

	// Draw the tactical map
	for (uint32_t y = 0; y < m_map.field().height(); ++y)
	{
		for (uint32_t x = 0; x < m_map.field().width(); ++x)
		{
			auto cellX = x * pixelPerCellX + (pixelPerCellX * 0.5f);
			auto cellY = y * pixelPerCellY + (pixelPerCellY * 0.5f);

			auto cellWidth = pixelPerCellX * m_cellSize;
			auto cellHeight = pixelPerCellY * m_cellSize;

			const QRect cellRect(cellX - (cellWidth * 0.5f), cellY - (cellHeight * 0.5f), cellWidth, cellHeight);

			auto& cell = m_map.cellAt(x, y);
			if (!cell.isObstacle)
			{
				QColor color = QColor(0, 255, 0, 255 * cell.value);
				painter.fillRect(cellRect, color);
			}
			else
			{
				painter.fillRect(cellRect, Qt::black);
			}

			if (m_showCellValues)
			{
				painter.setPen(Qt::black);
				painter.drawText(cellRect, Qt::AlignCenter, QString::number(cell.value, 'f', 2));
			}
		}
	}

	// Draw the units
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);
	for (const auto& unit : m_map.units())
	{
		auto x = unit.x * pixelPerCellX + pixelPerCellX * 0.5f;
		auto y = unit.y * pixelPerCellY + pixelPerCellY * 0.5f;

		const QRect unitRect(x - 5, y - 5, 10, 10);
		painter.drawEllipse(unitRect);
	}
}
