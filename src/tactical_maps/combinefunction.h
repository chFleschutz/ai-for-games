#pragma once

#include <algorithm>

class CombineFunction
{
public:
	CombineFunction() = default;
	virtual ~CombineFunction() = default;

	virtual float combine(float a, float b) const = 0;
	virtual constexpr float startValue() const { return 0.0f; };
};

class MaxCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return std::max(a, b);
	}
};

class MinCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return std::min(a, b);
	}
};

class SumCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return a + b;
	}
};

class DifferenceCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return std::max(0.0f, a - b);
	}
};

class ProductCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return a * b;
	}

	constexpr float startValue() const override { return 1.0f; }
};

class AverageCombine : public CombineFunction
{
public:
	float combine(float a, float b) const override
	{
		return (a + b) / 2.0f;
	}
};
