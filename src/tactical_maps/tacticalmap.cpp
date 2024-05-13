#include "tacticalmap.h"

#include <cmath>

void TacticalMap::addUnit(const CellCoord& coord)
{
	m_units.push_back(coord);
	updateField();
}

void TacticalMap::resetField()
{
	for (int y = 0; y < m_field.height(); y++)
	{
		for (int x = 0; x < m_field.width(); x++)
		{
			m_field.cellAt(x, y) = 0.0f;
		}
	}
}

void TacticalMap::updateField()
{
	resetField();

	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			for (const auto& unit : m_units)
			{
				auto& cell = m_field.cellAt(x, y);
				cell += m_influenceFunction->compute(CellCoord{ x, y }, unit);
				cell = std::max(0.0f, std::min(1.0f, cell));
			}
		}
	}
}
