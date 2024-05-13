#pragma once

#include "utils/cellfield.h"

#include <cmath>

class InfluenceFunction
{
public:
	InfluenceFunction() = default;
	virtual ~InfluenceFunction() = default;

	virtual float compute(CellCoord cell, CellCoord unit) const = 0;
};

class LinearInfluence : public InfluenceFunction
{
public:
	float compute(CellCoord cell, CellCoord unit) const override
	{
		auto dx = unit.x - cell.x;
		auto dy = unit.y - cell.y;
		auto distance = std::sqrt((dx * dx) + (dy * dy));

		return 1.0f / (distance + 1.0f);
	}
};

class QuadraticInfluence : public InfluenceFunction
{
public:
	float compute(CellCoord cell, CellCoord unit) const override
	{
		auto dx = unit.x - cell.x;
		auto dy = unit.y - cell.y;
		auto distance = std::sqrt((dx * dx) + (dy * dy));

		return 1.0 / std::pow(1.0 + distance, 2);
	}
};

class SqrtInfluence : public InfluenceFunction
{
public:
	float compute(CellCoord cell, CellCoord unit) const override
	{
		auto dx = unit.x - cell.x;
		auto dy = unit.y - cell.y;
		auto distance = std::sqrt((dx * dx) + (dy * dy));

		return 1.0f / std::sqrt(1.0f + distance);
	}
};
