#pragma once

#include <vector>

class RQLearning
{
public:
	RQLearning() = default;

	void init(size_t size);

	size_t size() const { return m_size; }

	const std::vector<float>& rMatrix() const { return m_rMatrix; }
	const std::vector<float>& qMatrix() const { return m_qMatrix; }

	float r(int state, int action) const { return m_rMatrix[state * m_size + action]; }
	float q(int state, int action) const { return m_qMatrix[state * m_size + action]; }

	void setR(int state, int action, float value);

	void train(float alpha, float gamma);
	void train(float alpha, float gamma, int maxIterations);

	void resetQ();

private:
	void setQ(int state, int action, float value) { m_qMatrix[state * m_size + action] = value; }

	size_t m_size = 0;
	std::vector<float> m_rMatrix;
	std::vector<float> m_qMatrix;
};
