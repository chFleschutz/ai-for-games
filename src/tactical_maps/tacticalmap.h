#pragma once

#include "utils/cellfield.h"
#include "tactical_maps/influencefunctions.h"

#include <vector>
#include <memory>
#include <cmath>

class TacticalMap
{
public:
	TacticalMap() = default;
	virtual ~TacticalMap() = default;

	float valueAt(uint32_t x, uint32_t y) const { return m_field.cellAt(x, y); }
	
	void addUnit(const CellCoord& coord);

	const std::vector<CellCoord>& units() const { return m_units; }
	const CellField<float>& field() { return m_field; }

	void resize(uint32_t width, uint32_t height) { m_field.resize(width, height); }

	template <typename T>
	void setInfluenceFunction()
	{
		m_influenceFunction = std::make_unique<T>();
		updateField();
	}

private:
	void resetField();
	void updateField();
	
	CellField<float> m_field;
	std::vector<CellCoord> m_units;
	std::unique_ptr<InfluenceFunction> m_influenceFunction = std::make_unique<LinearInfluence>();
};