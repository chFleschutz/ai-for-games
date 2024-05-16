#include "tacticalmap.h"

#include <cmath>
#include <deque>
#include <unordered_set>
#include <array>
#include <numbers>

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
	resetField();
}

void TacticalMap::resize(uint32_t width, uint32_t height)
{
	m_units.clear();
	m_field.resize(width, height);
	updateField();
}

void TacticalMap::resize(QImage& image, uint32_t width, uint32_t height)
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

void TacticalMap::setInfluenceType(InfluenceType type)
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

	for (const auto& unit : m_units)
	{
		switch (m_influenceType)
		{
		case TacticalMap::InfluenceType::Global:
			globalInfluence(unit);
			break;
		case TacticalMap::InfluenceType::Proximity:
			proximityInfluence(unit);
			break;
		case TacticalMap::InfluenceType::LineOfSight:
			lineOfSightInfluence(unit);
			break;
		default:
			break;
		}
	}
}

bool TacticalMap::isObstacle(QImage& image, int cellX, int cellY)
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

void TacticalMap::globalInfluence(CellCoord start)
{
	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			float distance = CellCoord::distance(start, { x, y });
			if (distance > m_influenceLimits.maxDistance + m_influenceLimits.offset)
				continue;

			updateInfluence(m_field.cellAt(x, y), distance);
		}
	}
}

void TacticalMap::proximityInfluence(CellCoord start)
{
	// Flood fill algorithm

	std::deque<std::pair<CellCoord, float>> queue;
	queue.push_back({ start, 0.0f });

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

void TacticalMap::lineOfSightInfluence(CellCoord start)
{
	for (uint32_t y = 0; y < m_field.height(); y++)
	{
		for (uint32_t x = 0; x < m_field.width(); x++)
		{
			float distance = CellCoord::distance(start, { x, y });
			if (distance > m_influenceLimits.maxDistance + m_influenceLimits.offset)
				continue;

			if (!hasLineOfSight(start, { x, y }))
				continue;

			updateInfluence(m_field.cellAt(x, y), distance);
		}
	}
}

void TacticalMap::updateInfluence(Cell& cell, float distance)
{
	cell.value += m_influenceFunction->compute(distance, m_influenceLimits);
	cell.value = std::max(0.0f, std::min(1.0f, cell.value));
}
