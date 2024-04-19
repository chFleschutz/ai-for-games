#include "flowfield.h"

#include <QDebug>
#include <deque>

void FlowField::initialize(QImage& image)
{
	Q_ASSERT_X(m_width > 0 && m_height > 0, "CellField::initialize", "Width and height must be greater than 0");
	initialize(image, m_width, m_height);
}

void FlowField::initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY)
{
	m_width = cellCountX;
	m_height = cellCountY;

	m_field.resize(cellCountX);

	for (auto& column : m_field)
		column.resize(cellCountY);

	clearDestinations();
	resetField();

	setNeighbors();
	calcCostField(image);
}

void FlowField::addDestination(uint32_t x, uint32_t y)
{
	addDestination(Coordinate{ x, y });
}

void FlowField::addDestination(const Coordinate& coordinate)
{
	if (coordinate.x >= m_width || coordinate.y >= m_height)
	{
		qDebug() << "Could not add destination to flowfield, coordinate out of bounds";
		return;
	}

	m_destinationPoints.emplace_back(coordinate);
}

void FlowField::calc()
{
	if (m_destinationPoints.empty())
		return;

	resetField();

	calcIntegrationField();
	calcFlowField();
}

void FlowField::resetField()
{
	for (auto& column : m_field)
	{
		for (auto& cell : column)
		{
			cell.integrationCost = 255;
			cell.flowDirection = QVector2D(0.0f, 0.0f);
		}
	}
}

void FlowField::clearDestinations()
{
	m_destinationPoints.clear();
}

void FlowField::setNeighbors()
{
	std::array<int, 8> dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
	std::array<int, 8> dy = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			auto& cell = m_field[x][y];

			for (int i = 0; i < 8; i++)
			{
				auto neighborX = x + dx[i];
				auto neighborY = y + dy[i];

				Cell* neighbor = nullptr;
				if (neighborX >= 0 && neighborX < m_width && neighborY >= 0 && neighborY < m_height)
					neighbor = &m_field[neighborX][neighborY];

				cell.neighbors[i].cell = neighbor;
				cell.neighbors[i].direction = QVector2D(dx[i], dy[i]);
			}
		}
	}
}

void FlowField::calcCostField(QImage& image)
{
	auto pixelPerCellX = static_cast<float>(image.width()) / static_cast<float>(m_width);
	auto pixelPerCellY = static_cast<float>(image.height()) / static_cast<float>(m_height);

	for (uint32_t cellY = 0; cellY < m_height; cellY++)
	{
		for (uint32_t cellX = 0; cellX < m_width; cellX++)
		{
			int maxValue = 0;
			auto pixelOffsetX = cellX * pixelPerCellX;
			auto pixelOffsetY = cellY * pixelPerCellY;
			for (uint32_t pixelY = pixelOffsetY; pixelY < pixelOffsetY + pixelPerCellY; pixelY++)
			{
				for (uint32_t pixelX = pixelOffsetX; pixelX < pixelOffsetX + pixelPerCellX; pixelX++)
				{
					auto color = image.pixel(pixelX, pixelY);
					maxValue = std::max(maxValue, mapColorToCost(color)); 
				}
			}

			auto& cell = m_field[cellX][cellY];
			cell.cost = maxValue;
			cell.integrationCost = 255;
		}
	}
}

void FlowField::calcIntegrationField()
{
	// Breadth first search to calculate the cost to reach each cell from the start cell

	std::deque<Cell*> openList;
	for (auto& dest : m_destinationPoints)
	{
		auto& destCell = m_field[dest.x][dest.y];
		destCell.integrationCost = 0;
		openList.push_back(&destCell);
	}

	while (!openList.empty())
	{
		auto currentCell = openList.front();
		openList.pop_front();

		for (auto& neighbor : currentCell->neighbors)
		{
			if (neighbor.cell == nullptr)
				continue;

			auto newCost = currentCell->integrationCost + neighbor.cell->cost;
			if (newCost < neighbor.cell->integrationCost)
			{
				neighbor.cell->integrationCost = newCost;
				openList.push_back(neighbor.cell);
			}
		}
	}
}

void FlowField::calcFlowField()
{
	for (auto& column : m_field)
	{
		for (auto& cell : column)
		{
			int minCost = 255;
			Cell::Neighbor* bestNeighbor = nullptr;

			for (auto& neighbor : cell.neighbors)
			{
				if (neighbor.cell == nullptr)
					continue;

				if (neighbor.cell->integrationCost < minCost)
				{
					minCost = neighbor.cell->integrationCost;
					bestNeighbor = &neighbor;
				}
			}

			// No reachable neighbors found
			if (minCost == 255)
			{
				cell.flowDirection = QVector2D(0.0f, 0.0f);
				continue;
			}

			if (bestNeighbor != nullptr)
			{
				cell.flowDirection = bestNeighbor->direction;
			}
		}
	}

	// Disable flow direction for destination cells
	for (auto& dest : m_destinationPoints)
	{
		auto& destCell = m_field[dest.x][dest.y];
		destCell.flowDirection = QVector2D(0.0f, 0.0f);
	}
}

int FlowField::mapColorToCost(QRgb color) const
{
	auto value = qRed(color);
	if (value < 250)
		value /= 50.0f;
	return value;
}
