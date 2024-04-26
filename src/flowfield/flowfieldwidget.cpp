#include "flowfieldwidget.h"

#include "utils/drawshape.h"

#include <QTimer>
#include <random>

FlowFieldWidget::FlowFieldWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_imageWidget = ui.imageRenderer;
	Q_ASSERT(m_imageWidget);

	ui.map_comboBox->setCurrentIndex(0);
	ui.width_spinbox->setValue(m_fieldWidth);
	ui.height_spinbox->setValue(m_fieldHeight);
	ui.showCells_checkbox->setChecked(m_showCells);
	ui.cellSize_slider->setValue(m_cellSize * 100);

	ui.agentCount_spinBox->setValue(m_agentCount);
	ui.frozen_checkBox->setChecked(m_frozen);

	connect(m_imageWidget, &ImageRendererWidget::onPaintEvent, this, &FlowFieldWidget::onImagePaint, Qt::DirectConnection);
	connect(m_imageWidget, &ImageRendererWidget::onDoubleClicked, this, &FlowFieldWidget::onImageDoubleClicked, Qt::DirectConnection);
	connect(m_imageWidget, &ImageRendererWidget::onResized, this, &FlowFieldWidget::onImageResized, Qt::DirectConnection);

	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &FlowFieldWidget::onAgentUpdate);
	timer->start(1000 / 60); // 60 FPS
	m_frameTimer.start();
}

FlowField::Coordinate FlowFieldWidget::pixelToCellCoord(const QPoint& pos) const
{
	auto posX = pos.x() / m_pixelPerCell.x();
	auto posY = pos.y() / m_pixelPerCell.y();
	return FlowField::Coordinate{ static_cast<uint32_t>(posX), static_cast<uint32_t>(posY) };
}

FlowField::Coordinate FlowFieldWidget::pixelToCellCoord(const QVector2D& pos) const
{
	auto posX = pos.x() / m_pixelPerCell.x();
	auto posY = pos.y() / m_pixelPerCell.y();
	return FlowField::Coordinate{ static_cast<uint32_t>(posX), static_cast<uint32_t>(posY) };
}

void FlowFieldWidget::onSetMap(int mapIndex)
{
	switch (mapIndex)
	{
	case 0:
		m_imageWidget->renderer().load(":/assets/images/FlowfieldMap.png", size());
		break;
	default:
		// Invalid index 
		return;
	}

	updateFlowField();
}

void FlowFieldWidget::onChangeFlowFieldWidth(int width)
{
	m_fieldWidth = width;
	updateFlowField();
}

void FlowFieldWidget::onChangeFlowFieldHeight(int height)
{
	m_fieldHeight = height;
	updateFlowField();
}

void FlowFieldWidget::onResetField()
{
	m_flowField.clearDestinations();
	m_flowField.resetField();
	update();
}

void FlowFieldWidget::onShowCells(bool show)
{
	m_showCells = show;
	update();
}

void FlowFieldWidget::onSetCellSize(int size)
{
	m_cellSize = size / 100.0f;
	update();
}

void FlowFieldWidget::onShowIntegrationCosts()
{
	m_showCosts = INTEGRATION_COSTS;
	update();
}

void FlowFieldWidget::onShowFlowDirection()
{
	m_showCosts = FLOW_DIRECTION;
	update();
}

void FlowFieldWidget::onShowTravelCosts()
{
	m_showCosts = TRAVEL_COSTS;
	update();
}

void FlowFieldWidget::onHideCosts()
{
	m_showCosts = STATS_HIDDEN;
	update();
}

void FlowFieldWidget::onSetAgentCount(int count)
{
	m_agentCount = count;
}

void FlowFieldWidget::onSpawnRandomAgents()
{
	m_agentManager.spawnRandomAgents(m_agentCount, m_imageWidget->renderer().image());
}

void FlowFieldWidget::onClearAgents()
{
	m_agentManager.clearAgents();
}

void FlowFieldWidget::onFreezeAgents(bool freeze)
{
	m_frozen = freeze;
}

void FlowFieldWidget::onImagePaint(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);

	if (m_showCells)
		drawCellValues(painter);

	m_agentManager.drawAgents(painter);
}

void FlowFieldWidget::onImageDoubleClicked(QMouseEvent* event)
{
	m_flowField.addDestination(pixelToCellCoord(event->pos()));
	m_flowField.calc();
	update();
}

void FlowFieldWidget::onImageResized(QResizeEvent* event)
{
	setPixelPerCell();
	update();
}

void FlowFieldWidget::onAgentUpdate()
{
	auto deltaTime = m_frameTimer.elapsed() / 1000.0f;
	m_frameTimer.restart();

	ui.fps->setText(QString::number(1.0f / deltaTime, 'f', 2));

	if (m_frozen)
		return;

	m_agentManager.updateAgents(*this, deltaTime); 
	update();
}

void FlowFieldWidget::updateFlowField()
{
	m_flowField.initialize(m_imageWidget->renderer().originalImage(), m_fieldWidth, m_fieldHeight);
	setPixelPerCell();
	update();
}

void FlowFieldWidget::drawCellValues(QPainter& painter)
{
	auto offset = m_pixelPerCell * 0.5f;
	auto size = std::min(m_pixelPerCell.x(), m_pixelPerCell.y());
	auto shapeSize = (m_pixelPerCell * m_cellSize).toPointF();

	QPen pen(Qt::GlobalColor::green);
	pen.setWidth(1);
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter.setPen(pen);

	for (uint32_t y = 0; y < m_flowField.height(); y++)
	{
		for (uint32_t x = 0; x < m_flowField.width(); x++)
		{
			const auto& cell = m_flowField.cellAt(x, y);
			QColor color(cell.cost, cell.cost, cell.cost);
			QRectF pointShape(-0.5f * shapeSize, 0.5f * shapeSize);

			painter.save();

			auto posX = (x * m_pixelPerCell.x()) + offset.x();
			auto posY = (y * m_pixelPerCell.y()) + offset.y();
			painter.translate(posX, posY);
			painter.drawEllipse(pointShape);
			painter.restore();

			drawCosts(painter, cell, size, posX, posY);
		}
	}
}

void FlowFieldWidget::drawCosts(QPainter& painter, const FlowField::Cell& cell, float cellSize, float posX, float posY)
{
	uint32_t displayValue = 0;
	switch (m_showCosts)
	{
	case FlowFieldWidget::TRAVEL_COSTS:
		displayValue = cell.cost;
		break;
	case FlowFieldWidget::INTEGRATION_COSTS:
		displayValue = cell.integrationCost;
		break;
	case FlowFieldWidget::FLOW_DIRECTION:
		if (cell.flowDirection.length() > 0.0f)
		{
			painter.translate(posX, posY);
			DrawShape::Arrow(painter, 0.7f * cellSize, toAngle(cell.flowDirection));
			painter.translate(-posX, -posY);
		}
		return;
	case FlowFieldWidget::STATS_HIDDEN:
		// Return early if costs are hidden
		return;
	default:
		break;
	}

	auto shift = 2.0f * digits(displayValue);
	painter.drawText(QPointF(posX - shift, posY + 4), QString::number(displayValue));
}

int FlowFieldWidget::digits(int x, int base)
{
	int digits = 1;
	while (x > 0)
	{
		x /= base;
		digits++;
	}
	return digits;
}

float FlowFieldWidget::toAngle(const QVector2D& direction)
{
	return std::atan2(direction.y(), direction.x()) * 180.0f / static_cast<float>(M_PI);
}

void FlowFieldWidget::setPixelPerCell()
{
	auto pixelPerCellX = static_cast<float>(m_imageWidget->imageWidth()) / static_cast<float>(m_flowField.width());
	auto pixelPerCellY = static_cast<float>(m_imageWidget->imageHeight()) / static_cast<float>(m_flowField.height());
	m_pixelPerCell = QVector2D(pixelPerCellX, pixelPerCellY);
}
