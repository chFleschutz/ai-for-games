#include "combinedtacticalmap.h"

float CombinedTacticalMap::valueAt(uint32_t x, uint32_t y) const
{
	float value = m_combineFunction->startValue();
	for (const auto& map : m_maps)
	{
		value = m_combineFunction->combine(map->valueAt(x, y), value);
	}
	return value;
}

bool CombinedTacticalMap::isObstacle(uint32_t x, uint32_t y) const
{
	for (const auto& map : m_maps)
	{
		if (map->isObstacle(x, y))
			return true;
	}
	return false;
}

void CombinedTacticalMap::addUnit(const CellCoord& coord)
{
	for (const auto& map : m_maps)
	{
		map->addUnit(coord);
	}
}

void CombinedTacticalMap::clearUnits()
{
	for (auto& map : m_maps)
	{
		map->clearUnits();
	}
}

void CombinedTacticalMap::addMap(std::shared_ptr<TacticalMapBase> map)
{
	m_maps.push_back(map);
}

void CombinedTacticalMap::resize(uint32_t width, uint32_t height)
{
	for (const auto& map : m_maps)
	{
		map->resize(width, height);
	}
}

void CombinedTacticalMap::resize(const QImage& image, uint32_t width, uint32_t height)
{
	for (const auto& map : m_maps)
	{
		map->resize(image, width, height);
	}
}
