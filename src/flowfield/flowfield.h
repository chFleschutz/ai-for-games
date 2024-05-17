#pragma once

#include "utils/cellfield.h"

#include <QImage>
#include <QVector2D>

#include <vector>
#include <array>

struct FlowFieldCell
{
	struct Neighbor
	{
		FlowFieldCell* cell = nullptr;
		QVector2D direction;
	};

	uint32_t cost;
	uint32_t integrationCost;
	QVector2D flowDirection;

	std::array<Neighbor, 8> neighbors;
};

class FlowField : public CellField<FlowFieldCell>
{
public:
	FlowField() = default;
	~FlowField() = default;

	void initialize(QImage& image);
	void initialize(QImage& image, uint32_t cellCountX, uint32_t cellCountY);

	void addDestination(uint32_t x, uint32_t y);
	void addDestination(const CellCoord& coordinate);

	void calc();

	void resetField();
	void clearDestinations();

private:
	void setNeighbors();
	void calcCostField(QImage& image);
	void calcIntegrationField();
	void calcFlowField();
	int mapColorToCost(QRgb color) const;

	std::vector<CellCoord> m_destinationPoints;
};