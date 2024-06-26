#include "rqlearning.h"

#include <random>

void RQLearning::init(size_t size)
{
	m_size = size;
	m_rMatrix.resize(size * size);
	m_qMatrix.resize(size * size);

	std::fill(m_rMatrix.begin(), m_rMatrix.end(), -1.0f);
	std::fill(m_qMatrix.begin(), m_qMatrix.end(), 0.0f);
}

void RQLearning::setR(int state, int action, float value)
{
	m_rMatrix[state * m_size + action] = value;
}

void RQLearning::train(float alpha, float gamma)
{
	if (m_size == 0)
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, m_size - 1);

	int state = dis(gen);
	int newState = dis(gen);
	float rValue = r(state, newState);
	while (rValue < 0.0f)
	{
		newState = dis(gen);
		rValue = r(state, newState);
	}

	float maxQ = std::numeric_limits<float>::lowest();
	for (int action = 0; action < m_size; action++)
	{
		if (q(newState, action) > maxQ)
			maxQ = q(newState, action);
	}

	float qValue = rValue + gamma * maxQ;							// Standard Q-Learning
	qValue = (1.0f - alpha) * q(state, newState) + alpha * qValue;  // Incremental Learning
	setQ(state, newState, qValue);
}

void RQLearning::train(float alpha, float gamma, int maxIterations)
{
	for (int i = 0; i < maxIterations; i++)
	{
		train(alpha, gamma);
	}
}

void RQLearning::resetQ()
{
	std::fill(m_qMatrix.begin(), m_qMatrix.end(), 0.0f);
}
