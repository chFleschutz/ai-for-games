#pragma once

#include <QWidget>
#include <QVector2D>

class BallisticSimWidget : public QWidget
{
	Q_OBJECT

public:
	struct Bullet
	{
		QVector2D position;
		QVector2D velocity;
	};

	BallisticSimWidget(QWidget* parent = nullptr);
	~BallisticSimWidget() = default;

	float gravity() const { return m_gravity; }
	float launchSpeedScale() const { return m_launchSpeedScale; }

public slots:
	void setGravity(double gravity) { m_gravity = gravity; }
	void setLaunchSpeedScale(double scale) { m_launchSpeedScale = scale; }

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void updateBullets(float deltaTime);
	void drawBullets(QPainter& painter);
	void drawLaunchArrow(QPainter& painter);

	std::vector<Bullet> m_bullets;
	float m_gravity = 9.81f;
	int m_bulletSize = 5;

	QPoint m_mousePressPos;
	QPoint m_currentMousePos;
	bool m_mousePressed = false;
	float m_launchSpeedScale = 0.2f;
};