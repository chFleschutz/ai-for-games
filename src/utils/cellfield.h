#pragma once

#include <vector>

struct CellCoord
{
	uint32_t x;
	uint32_t y;
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

protected:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<T> m_field;
};