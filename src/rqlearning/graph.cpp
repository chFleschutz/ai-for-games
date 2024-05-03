#include "graph.h"

uint32_t Graph::addNode(QPoint pos)
{
	uint32_t id = m_nodes.size();
	m_nodes.push_back({ id, pos });
	return id;
}

void Graph::addEdge(uint32_t from, uint32_t to)
{
	m_edges.push_back({ from, to });
}

void Graph::clear()
{
	m_nodes.clear();
	m_edges.clear();
}
