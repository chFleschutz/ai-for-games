#include "flowfieldagent.h"

#include "flowfieldwidget.h"

#include <random>

void FlowFieldAgent::update(const QVector2D& flowDir, const Limits& limits, float deltaTime)
{
	// Sum of acceleration and deceleration
	QVector2D acceleration;
	if (flowDir.length() > 0.1f)
		acceleration = (flowDir - m_velocity).normalized() * limits.acceleration;

	acceleration -= m_velocity.normalized() * limits.deceleration;

	// Update velocity and position
	m_velocity += acceleration;
	m_position += m_velocity;
}

void AgentManager::spawnRandomAgents(uint32_t count, const QImage& image)
{
	m_agents.clear();
	m_agents.reserve(count);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(0, image.width() - 1);
	std::uniform_real_distribution<float> disY(0, image.height() - 1);

	for (uint32_t i = 0; i < count; ++i)
	{
		float x = disX(gen);
		float y = disY(gen);

		// Make sure the agent is not spawned on a barrier
		while (qRed(image.pixel(x, y)) > 250)
		{
			x = disX(gen);
			y = disY(gen);
		}

		m_agents.emplace_back(x, y);
	}
}

void AgentManager::updateAgents(const FlowFieldWidget& flowFieldWidget, float deltaTime)
{
	auto& flowField = flowFieldWidget.flowField();
	for (auto& agent : m_agents)
	{
		auto cellCoord = flowFieldWidget.pixelToCellCoord(agent.position());
		if (cellCoord.x < 0 || cellCoord.x >= flowField.width() || cellCoord.y < 0 || cellCoord.y >= flowField.height())
			continue;

		auto& cell = flowField.cellAt(cellCoord);
		agent.update(cell.flowDirection, m_limits, deltaTime);
	}
}

void AgentManager::drawAgents(QPainter& painter) const
{
	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::PenStyle::SolidLine);

	QBrush brush;
	brush.setStyle(Qt::BrushStyle::SolidPattern);
	brush.setColor(Qt::cyan);

	painter.setPen(pen);
	painter.setBrush(brush);
	painter.save();

	for (const auto& agent : m_agents)
	{
		painter.drawEllipse(agent.position().x(), agent.position().y(), 4, 4);
	}

	painter.restore();
}
