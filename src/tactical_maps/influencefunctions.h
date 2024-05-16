#pragma once

#include "utils/cellfield.h"

#include <cmath>

class InfluenceFunction
{
public:
	struct Limits
	{
		float maxDistance = 10.0f;
		float maxInfluence = 1.0f;
		float offset = 0.0f;
		float exponent = 1.0f;
	};

	InfluenceFunction() = default;
	virtual ~InfluenceFunction() = default;

	virtual float compute(float distance, Limits limits) const = 0;
};

class MillingtonInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		return limits.maxInfluence / std::pow(1.0f + distance, limits.exponent);
	}
};

class MarkInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		float factor = limits.maxInfluence * (distance / limits.maxDistance);
		float value = limits.maxInfluence - std::pow(factor, limits.exponent);
		return std::max(0.0f, value);
	}
};

class BichlmeierInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		float factor = std::pow(std::abs(limits.offset - distance) / limits.maxDistance, limits.exponent);
		float value = limits.maxInfluence - (limits.maxInfluence * factor);
		return std::max(0.0f, value);
	}
};