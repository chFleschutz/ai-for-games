#include "tacticalmap.h"

#include <cmath>
#include <deque>

void TacticalMap::addUnit(const CellCoord& coord)
{
	m_units.push_back(coord);
	updateField();
}

void TacticalMap::clearUnits()
{
	m_units.clear();
	resetField();
}

void TacticalMap::resize(uint32_t width, uint32_t height)
{
	m_field.resize(width, height);
	updateField();
}

void TacticalMap::resize(QImage& image, uint32_t width, uint32_t height)
{
	m_field.resize(width, height);
	for (auto& cell : m_field.field())
	{
		cell.obstacle = false;
	}

	auto pixelPerCellX = static_cast<float>(image.width()) / width;
	auto pixelPerCellY = static_cast<float>(image.height()) / height;

	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			m_field.cellAt(x, y).obstacle = obstacle(image, x, y);
		}
	}

	updateField();
}

void TacticalMap::setMaxDistance(float value)
{
	m_influenceLimits.maxDistance = value;
	updateField();
}

void TacticalMap::setMaxInfluence(float value)
{
	m_influenceLimits.maxInfluence = value;
	updateField();
}

void TacticalMap::resetField()
{
	for (auto& cell : m_field.field())
	{
		cell.value = 0.0f;
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
				float distance = CellCoord::distance(CellCoord{ x, y }, unit);
				cell.value += m_influenceFunction->compute(distance, m_influenceLimits);
				cell.value = std::max(0.0f, std::min(1.0f, cell.value));
			}
		}
	}
}

bool TacticalMap::obstacle(QImage& image, int cellX, int cellY)
{
	auto pixelPerCellX = static_cast<float>(image.width()) / m_field.width();
	auto pixelPerCellY = static_cast<float>(image.height()) / m_field.height();

	for (int pixelX = cellX * pixelPerCellX; pixelX < (cellX + 1) * pixelPerCellX; pixelX++)
	{
		for (int pixelY = cellY * pixelPerCellY; pixelY < (cellY + 1) * pixelPerCellY; pixelY++)
		{
			auto color = image.pixel(pixelX, pixelY);
			if (qRed(color) < 250)
				return false;
		}
	}

	return true;
}
