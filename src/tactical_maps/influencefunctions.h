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
	};

	InfluenceFunction() = default;
	virtual ~InfluenceFunction() = default;

	virtual float compute(float distance, Limits limits) const = 0;
};

class LinearInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		auto value = limits.maxInfluence - (limits.maxInfluence * (distance / limits.maxDistance));
		return std::max(0.0f, value);
	}
};

class QuadraticInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		auto value = limits.maxInfluence - std::pow(limits.maxInfluence * (distance / limits.maxDistance), 2);
		return std::max(0.0, value);
	}
};

class CubicInfluence : public InfluenceFunction
{
public:
	float compute(float distance, Limits limits) const override
	{
		auto value = limits.maxInfluence - std::pow(limits.maxInfluence * (distance / limits.maxDistance), 3);
		return std::max(0.0, value);
	}
};