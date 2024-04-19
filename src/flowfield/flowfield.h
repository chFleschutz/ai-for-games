#pragma once

#include <QImage>
#include <QVector2D>

#include <vector>
#include <array>

class FlowField
{
public:
	struct Cell
	{
		struct Neighbor
		{
			Cell* cell = nullptr;
			QVector2D direction;
		};

		uint32_t cost;
		uint32_t integrationCost;
		QVector2D flowDirection;

		std::array<Neighbor, 8> neighbors;
	};

	FlowField() = default;
	~FlowField() = default;

	void initialize(QImage& image);
	void initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY);

	void addDestination(int x, int y);

	void calc();

	void resetField();
	void clearDestinations();

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }

	Cell& cell(uint32_t x, uint32_t y) { return m_field[x][y]; }

private:
	void setNeighbors();
	void calcCostField(QImage& image);
	void calcIntegrationField();
	void calcFlowField();
	int mapColorToCost(QRgb color) const;

	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<std::vector<Cell>> m_field;

	std::vector<QPoint> m_destinationPoints;
};