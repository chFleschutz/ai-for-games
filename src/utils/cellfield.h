#pragma once

#include <vector>
#include <cmath>

struct CellCoord
{
	uint32_t x;
	uint32_t y;

	CellCoord(uint32_t xCoord, uint32_t yCoord) : x(xCoord), y(yCoord) {}

	static float distance(const CellCoord& a, const CellCoord& b)
	{
		auto dx = static_cast<int>(a.x) - static_cast<int>(b.x);
		auto dy = static_cast<int>(a.y) - static_cast<int>(b.y);
		return std::sqrt((dx * dx) + (dy * dy));
	}

	bool operator == (const CellCoord& other) const = default;
	bool operator != (const CellCoord& other) const = default;
};

// Function to hash a CellCoord (for use in unordered_map or unordered_set)
struct CellCoordHash
{
	std::size_t operator()(const CellCoord& coord) const
	{
		return std::hash<uint32_t>()(coord.x) ^ (std::hash<uint32_t>()(coord.y) << 1);
	}
};


template <typename T>
class CellField
{
public:
	CellField() = default;
	virtual ~CellField() = default;

	void resize(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
		m_field.resize(m_width * m_height);
	}

	// Returns the cell at the given coordinate
	T& cellAt(CellCoord coord) { return cellAt(coord.x, coord.y); }
	const T& cellAt(CellCoord coord) const { return cellAt(coord.x, coord.y); }

	T& cellAt(uint32_t x, uint32_t y) { return m_field[(y * m_height) + x]; }
	const T& cellAt(uint32_t x, uint32_t y) const { return m_field[(y * m_height) + x]; }

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }

	std::vector<T>& field() { return m_field; }

	void fill(const T& value)
	{
		std::fill(m_field.begin(), m_field.end(), value);
	}

protected:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<T> m_field;
};