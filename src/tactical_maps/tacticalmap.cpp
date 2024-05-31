#include "tacticalmap.h"

#include <cmath>
#include <deque>
#include <unordered_set>
#include <array>
#include <numbers>
#include <limits>

float TacticalMap::valueAt(uint32_t x, uint32_t y) const
{
	if (x >= m_field.width() || y >= m_field.height())
		return 0.0f;

	if (m_field.cellAt(x, y).isObstacle)
		return -1.0f;

	return m_field.cellAt(x, y).value;
}

bool TacticalMap::isObstacle(uint32_t x, uint32_t y) const
{
	if (x >= m_field.width() || y >= m_field.height())
		return true;

	return m_field.cellAt(x, y).isObstacle;
}

void TacticalMap::addUnit(const CellCoord& coord)
{
	if (coord.x >= m_field.width() || coord.y >= m_field.height())
		return;

	if (m_field.cellAt(coord).isObstacle)
		return;

	if (std::find(m_units.begin(), m_units.end(), coord) != m_units.end())
		return;

	m_units.push_back(coord);
	updateField();
}

void TacticalMap::clearUnits()
{
	m_units.clear();
	updateField();
}

void TacticalMap::resize(uint32_t width, uint32_t height)
{
	m_units.clear();
	m_field.resize(width, height);
	updateField();
}

void TacticalMap::resize(const QImage& image, uint32_t width, uint32_t height)
{
	m_units.clear();
	m_field.resize(width, height);
	for (auto& cell : m_field.field())
	{
		cell.isObstacle = false;
	}

	auto pixelPerCellX = static_cast<float>(image.width()) / width;
	auto pixelPerCellY = static_cast<float>(image.height()) / height;

	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			m_field.cellAt(x, y).isObstacle = isObstacle(image, x, y);
		}
	}

	updateField();
}

void TacticalMap::setInfluenceLimits(const InfluenceFunction::Limits& limits)
{
	m_influenceLimits = limits;
	updateField();
}

void TacticalMap::setType(Type type)
{
	m_influenceType = type;
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

	switch (m_influenceType)
	{
	case TacticalMap::Type::Global:
		globalInfluence();
		break;
	case TacticalMap::Type::Proximity:
		proximityInfluence();
		break;
	case TacticalMap::Type::LineOfSight:
		lineOfSightInfluence();
		break;
	case TacticalMap::Type::Visibility:
		visibilityInfluence();
		break;
	case TacticalMap::Type::Distance:
		distanceInfluence();
		break;
	default:
		break;
	}
}

bool TacticalMap::isObstacle(const QImage& image, int cellX, int cellY)
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

bool TacticalMap::hasLineOfSight(CellCoord start, CellCoord end)
{
	// Bresenham's line algorithm

	int dx = static_cast<int>(end.x) - static_cast<int>(start.x);
	int dy = static_cast<int>(end.y) - static_cast<int>(start.y);

	int sx = (dx > 0) ? 1 : -1;
	int sy = (dy > 0) ? 1 : -1;

	dx = std::abs(dx);
	dy = std::abs(dy);

	int err = dx - dy;

	while (start != end)
	{
		if (m_field.cellAt(start).isObstacle)
			return false;

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			start.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			start.y += sy;
		}
	}

	return true;
}

void TacticalMap::updateInfluence(Cell& cell, float distance)
{
	cell.value += m_influenceFunction->compute(distance, m_influenceLimits);
	cell.value = std::max(0.0f, std::min(m_influenceLimits.maxInfluence, cell.value));
}

void TacticalMap::globalInfluence()
{
	for (const auto& unit : m_units)
	{
		for (uint32_t y = 0; y < m_field.height(); y++)
		{
			for (uint32_t x = 0; x < m_field.width(); x++)
			{
				float distance = CellCoord::distance(unit, { x, y });
				if (distance > m_influenceLimits.maxDistance + m_influenceLimits.offset)
					continue;

				updateInfluence(m_field.cellAt(x, y), distance);
			}
		}
	}
}

void TacticalMap::proximityInfluence()
{
	for (const auto& unit : m_units)
	{
		// Flood fill algorithm

		std::deque<std::pair<CellCoord, float>> queue;
		queue.push_back({ unit, 0.0f });

		std::unordered_set<CellCoord, CellCoordHash> visited;

		while (!queue.empty())
		{
			auto [coord, distance] = queue.front();
			queue.pop_front();

			if (visited.find(coord) != visited.end())
				continue;

			if (distance > m_influenceLimits.maxDistance + m_influenceLimits.offset)
				continue;

			auto& cell = m_field.cellAt(coord);
			if (cell.isObstacle)
				continue;

			updateInfluence(cell, distance);
			visited.insert(coord);

			// Add neighbors
			std::array<int, 8> dx = { 0, 1, 0, -1, 1, -1, 1, -1 };
			std::array<int, 8> dy = { 1, 0, -1, 0, 1, 1, -1, -1 };
			for (uint32_t i = 0; i < 8; i++)
			{
				uint32_t x = coord.x + dx[i];
				uint32_t y = coord.y + dy[i];

				if (x >= m_field.width() || y >= m_field.height()) // Check < 0 unnecessary because of unsigned int
					continue;

				float newDistance = distance + 1.0f;
				if (dx[i] != 0 && dy[i] != 0)
					newDistance = distance + std::numbers::sqrt2; // Diagonal distance = sqrt(2)

				queue.push_back({ { x, y }, newDistance });
			}
		}
	}
}

void TacticalMap::lineOfSightInfluence()
{
	for (const auto& unit : m_units)
	{
		for (uint32_t y = 0; y < m_field.height(); y++)
		{
			for (uint32_t x = 0; x < m_field.width(); x++)
			{
				float distance = CellCoord::distance(unit, { x, y });
				if (distance > m_influenceLimits.maxDistance + m_influenceLimits.offset)
					continue;

				if (!hasLineOfSight(unit, { x, y }))
					continue;

				updateInfluence(m_field.cellAt(x, y), distance);
			}
		}
	}
}

void TacticalMap::visibilityInfluence()
{
	int maxVisibleCells = std::numeric_limits<int>::lowest();
	int minVisibleCells = std::numeric_limits<int>::max();

	// Count visible cells from each cell
	for (uint32_t startY = 0; startY < m_field.height(); startY++)
	{
		for (uint32_t startX = 0; startX < m_field.width(); startX++)
		{
			if (isObstacle(startX, startY))
				continue;

			CellCoord start{ startX, startY };
			int visibleCells = 0;
			for (uint32_t endY = 0; endY < m_field.height(); endY++)
			{
				for (uint32_t endX = 0; endX < m_field.width(); endX++)
				{
					CellCoord end{ endX, endY };
					if (CellCoord::distance(start, end) > m_influenceLimits.maxDistance)
						continue;

					if (hasLineOfSight({ startX, startY }, { endX, endY }))
						visibleCells++;
				}
			}

			m_field.cellAt(startX, startY).value = visibleCells;
			
			maxVisibleCells = std::max(maxVisibleCells, visibleCells);
			minVisibleCells = std::min(minVisibleCells, visibleCells);
		}
	}

	// Normalize value 
	for (auto& cell : m_field.field())
	{
		cell.value = (cell.value - static_cast<float>(minVisibleCells)) / static_cast<float>(maxVisibleCells - minVisibleCells);
	}
}

void TacticalMap::distanceInfluence()
{
	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			float minDistance = std::numeric_limits<float>::max();
			for (const auto& unit : m_units)
			{
				float distance = CellCoord::distance(unit, { x, y });
				minDistance = std::min(minDistance, distance);
			}

			auto& cell = m_field.cellAt(x, y);
			updateInfluence(cell, minDistance);
			// Invert Influence
			cell.value = m_influenceLimits.maxInfluence - cell.value; 
		}
	}
}
