#pragma once

#include <vector>
#include <array>
#include <limits>
#include <QDebug>

class HillClimbing
{
public:
	static int findMaxValue(const std::vector<float>& values, int startIndex)
	{
		int index = startIndex;
		int nextIndex = index;
		float value = std::numeric_limits<float>::lowest();
		float maxNextValue = 0.0f;

		while (value < maxNextValue)
		{
			index = nextIndex;
			value = values[index];

			// Find max value of neighbors
			maxNextValue = std::numeric_limits<float>::lowest();
			std::array<int, 2> neighbors = { index - 1, index + 1 };
			for (auto neighbor : neighbors)
			{
				if (neighbor < 0 || neighbor >= values.size())
					continue;

				if (values[neighbor] > maxNextValue)
				{
					maxNextValue = values[neighbor];
					nextIndex = neighbor;
				}
			}
		}

		return index;
	}
};