#pragma once

#include <QPoint>

#include <vector>
#include <unordered_map>

class Graph
{
public:
	struct Node
	{
		uint32_t id;
		QPoint position;
	};

	struct Edge
	{
		uint32_t from;
		uint32_t to;
	};

	uint32_t addNode(QPoint pos = QPoint());
	void addEdge(uint32_t from, uint32_t to);

	size_t nodeCount() const { return m_nodes.size(); }
	std::vector<Node>& nodes() { return m_nodes; }

	size_t edgeCount() const { return m_edges.size(); }
	std::vector<Edge>& edges() { return m_edges; }

	Node& node(uint32_t id) { return m_nodes[id]; }
	const Node& node(uint32_t id) const { return m_nodes[id]; }

	void clear();

private:
	std::vector<Node> m_nodes;
	std::vector<Edge> m_edges;
};