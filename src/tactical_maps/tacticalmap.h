#pragma once

#include "utils/cellfield.h"
#include "tactical_maps/influencefunctions.h"

#include <QImage>

#include <vector>
#include <memory>
#include <cmath>

class TacticalMapBase
{
public:
	virtual ~TacticalMapBase() = default;

	virtual float valueAt(uint32_t x, uint32_t y) const = 0;
	virtual bool isObstacle(uint32_t x, uint32_t y) const = 0;

	virtual void addUnit(const CellCoord& coord) = 0;
	virtual void clearUnits() = 0;

	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual void resize(const QImage& image, uint32_t width, uint32_t height) = 0;
};

class TacticalMap : public TacticalMapBase
{
public:
	struct Cell
	{
		float value = 0.0f;
		bool isObstacle = false;
	};

	enum class Type
	{
		Global,
		Proximity,
		LineOfSight,
		Visibility,
		Distance,
	};

	TacticalMap() = default;
	virtual ~TacticalMap() = default;

	/// @brief Returns the value of the cell at the given coordinates.
	/// @note The value is -1 if the cell is an obstacle else it is in the range [0, maxInfluence].
	virtual float valueAt(uint32_t x, uint32_t y) const override;

	/// @brief Returns true if the cell at the given coordinates is an obstacle.
	virtual bool isObstacle(uint32_t x, uint32_t y) const override;

	virtual void addUnit(const CellCoord& coord) override;
	virtual void clearUnits() override;

	virtual void resize(uint32_t width, uint32_t height) override;
	virtual void resize(const QImage& image, uint32_t width, uint32_t height) override;

	uint32_t width() const { return m_field.width(); }
	uint32_t height() const { return m_field.height(); }

	const std::vector<CellCoord>& units() const { return m_units; }

	template <typename T, typename... Args>
	void setInfluenceFunction(Args&&... args)
	{
		m_influenceFunction = std::make_unique<T>(std::forward(args)...);
		updateField();
	}

	void setInfluenceLimits(const InfluenceFunction::Limits& limits);
	const InfluenceFunction::Limits& influenceLimits() const { return m_influenceLimits; }

	void setType(Type type);
	Type type() const { return m_influenceType; }

private:
	void resetField();
	void updateField();
	bool isObstacle(const QImage& image, int cellX, int cellY);
	bool hasLineOfSight(CellCoord start, CellCoord end);
	void updateInfluence(Cell& cell, float distance);

	void globalInfluence();
	void proximityInfluence();
	void lineOfSightInfluence();
	void visibilityInfluence();
	void distanceInfluence();

	CellField<Cell> m_field;
	std::vector<CellCoord> m_units;

	InfluenceFunction::Limits m_influenceLimits;
	std::unique_ptr<InfluenceFunction> m_influenceFunction = std::make_unique<BichlmeierInfluence>();

	Type m_influenceType = Type::Global;
};