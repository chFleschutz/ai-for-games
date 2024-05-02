#pragma once

#include <vector>
#include <array>
#include <limits>
#include <QDebug>

class HillClimbing
{
public:
	HillClimbing() = default;
	~HillClimbing() = default;

	const std::vector<float>& fitnessValues() const { return m_fitness; }
	const std::vector<float>& momentumValues() const { return m_momentum; }
	const std::vector<float>& fitnessMomentumValues() const { return m_fitnessMomentum; }

	void setValues(std::vector<float> values)
	{
		m_fitness = std::move(values);
		m_momentum.resize(m_fitness.size(), 0);
		m_fitnessMomentum.resize(m_fitness.size(), 0);
	}

	int run(int startIndex, float alpha)
	{
		if (m_fitness.empty())
			return 0;

		if (startIndex < 0 || startIndex >= m_fitness.size())
			return 0;

		calcMomentum(startIndex);
		calcFitMomentum(startIndex, alpha);

		return run(startIndex);
	}

	int run(int startIndex)
	{
		int index = startIndex;
		int nextIndex = index;
		float value = std::numeric_limits<float>::lowest();
		float maxNextValue = 0.0f;

		while (value < maxNextValue)
		{
			index = nextIndex;
			value = m_fitnessMomentum[index];

			// Find max value of neighbors
			maxNextValue = std::numeric_limits<float>::lowest();
			std::array<int, 2> neighbors = { index - 1, index + 1 };
			for (auto neighbor : neighbors)
			{
				if (neighbor < 0 || neighbor >= m_fitnessMomentum.size())
					continue;

				if (m_fitnessMomentum[neighbor] > maxNextValue)
				{
					maxNextValue = m_fitnessMomentum[neighbor];
					nextIndex = neighbor;
				}
			}
		}

		return index;
	}

private:
	void calcMomentum(int startIndex)
	{
		m_momentum[startIndex] = 0.0f;
		for (int i = startIndex - 1; i >= 0; i--)
		{
			m_momentum[i] = (m_fitness[i] - m_fitness[i + 1]) + m_momentum[i + 1];
		}

		for (int i = startIndex + 1; i < m_fitness.size(); i++)
		{
			m_momentum[i] = (m_fitness[i] - m_fitness[i - 1]) + m_momentum[i - 1];
		}
	}

	void calcFitMomentum(int startIndex, float alpha)
	{
		m_fitnessMomentum[startIndex] = m_fitness[startIndex];
		for (int i = startIndex - 1; i >= 0; i--)
		{
			m_fitnessMomentum[i] = m_momentum[i + 1] * alpha + m_fitness[i];
		}

		for (int i = startIndex + 1; i < m_fitness.size(); i++)
		{
			m_fitnessMomentum[i] = m_momentum[i - 1] * alpha + m_fitness[i];
		}
	}

	std::vector<float> m_fitness;
	std::vector<float> m_momentum;
	std::vector<float> m_fitnessMomentum;
};