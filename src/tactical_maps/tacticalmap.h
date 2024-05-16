#pragma once

#include "utils/cellfield.h"
#include "tactical_maps/influencefunctions.h"

#include <QImage>

#include <vector>
#include <memory>
#include <cmath>

class TacticalMap
{
public:
	struct Cell
	{
		float value = 0.0f;
		bool isObstacle = false;
	};

	enum class InfluenceType
	{
		Global,
		Proximity,
		LineOfSight
	};

	TacticalMap() = default;
	virtual ~TacticalMap() = default;

	float valueAt(uint32_t x, uint32_t y) const { return m_field.cellAt(x, y).value; }
	const TacticalMap::Cell& cellAt(uint32_t x, uint32_t y) { return m_field.cellAt(x, y); }

	void addUnit(const CellCoord& coord);
	void clearUnits();

	const std::vector<CellCoord>& units() const { return m_units; }
	const CellField<Cell>& field() { return m_field; }

	void resize(uint32_t width, uint32_t height);
	void resize(QImage& image, uint32_t width, uint32_t height);

	template <typename T, typename... Args>
	void setInfluenceFunction(Args&&... args)
	{
		m_influenceFunction = std::make_unique<T>(std::forward(args)...);
		updateField();
	}

	void setInfluenceLimits(const InfluenceFunction::Limits& limits);
	const InfluenceFunction::Limits& influenceLimits() const { return m_influenceLimits; }

	void setInfluenceType(InfluenceType type);
	InfluenceType influenceType() const { return m_influenceType; }

private:
	void resetField();
	void updateField();
	bool isObstacle(QImage& image, int cellX, int cellY);
	bool hasLineOfSight(CellCoord start, CellCoord end);

	void globalInfluence(CellCoord start);
	void proximityInfluence(CellCoord start);
	void lineOfSightInfluence(CellCoord start);
	void updateInfluence(Cell& cell, float distance);

	CellField<Cell> m_field;
	std::vector<CellCoord> m_units;

	InfluenceFunction::Limits m_influenceLimits;
	std::unique_ptr<InfluenceFunction> m_influenceFunction = std::make_unique<MillingtonInfluence>();

	InfluenceType m_influenceType = InfluenceType::Global;
};