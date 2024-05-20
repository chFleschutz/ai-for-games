#pragma once

#include "tactical_maps/tacticalmap.h"
#include "tactical_maps/combinefunction.h"

class CombinedTacticalMap : public TacticalMapBase
{
public:
	CombinedTacticalMap() = default;
	~CombinedTacticalMap() = default;

	virtual float valueAt(uint32_t x, uint32_t y) const override;
	virtual bool isObstacle(uint32_t x, uint32_t y) const override;

	virtual void addUnit(const CellCoord& coord) override;
	virtual void clearUnits() override;

	void addMap(std::shared_ptr<TacticalMapBase> map);

	template<typename T>
	void setCombineFunction()
	{
		m_combineFunction = std::make_unique<T>();
	}

	virtual void resize(uint32_t width, uint32_t height) override;
	virtual void resize(const QImage& image, uint32_t width, uint32_t height) override;

private:
	std::vector<std::shared_ptr<TacticalMapBase>> m_maps;

	std::unique_ptr<CombineFunction> m_combineFunction;
};