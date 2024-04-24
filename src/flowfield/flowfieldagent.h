#pragma once

#include "flowfield/flowfield.h"

class FlowFieldWidget;

class FlowFieldAgent
{
public:
	struct Limits
	{
		float maxSpeed = 10.0f;
		float acceleration = 0.05f;
		float deceleration = 0.02f;
	};

	 FlowFieldAgent() = default;
	 FlowFieldAgent(float x, float y) : m_position(x, y) {}
	 ~FlowFieldAgent() = default;

	 void update(const QVector2D& flowDir, const Limits& limits, float deltaTime);

	 const QVector2D& position() const { return m_position; }

private:
	QVector2D m_position;
	QVector2D m_velocity;
};


class AgentManager
{
public:
	AgentManager() = default;
	~AgentManager() = default;

	void clearAgents() { m_agents.clear(); }
	void spawnRandomAgents(uint32_t count, const QImage& image);

	void updateAgents(const FlowFieldWidget& flowField, float deltaTime);
	void drawAgents(QPainter& painter) const;

	void setMaxSpeed(float maxSpeed) { m_limits.maxSpeed = maxSpeed; }
	void setAcceleration(float acceleration) { m_limits.acceleration = acceleration; }

	const FlowFieldAgent::Limits& limits() const { return m_limits; }

private:
	std::vector<FlowFieldAgent> m_agents;
	FlowFieldAgent::Limits m_limits;
	QVector2D m_pixelPerCell;
};