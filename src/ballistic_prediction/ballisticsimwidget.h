#pragma once

#include <QWidget>
#include <QVector2D>

#include <optional>

class BallisticSimWidget : public QWidget
{
	Q_OBJECT

public:
	struct Bullet
	{
		QVector2D position;
		QVector2D velocity;
		QVector2D predictedPosition;
	};

	BallisticSimWidget(QWidget* parent = nullptr);
	~BallisticSimWidget() = default;

	float gravity() const { return m_gravity * m_pixelPerMeter; }
	float gravityUnscaled() const { return m_gravity; }
	float launchSpeedScale() const { return m_launchSpeedScale * m_pixelPerMeter; }
	float launchSpeedScaleUnscaled() const { return m_launchSpeedScale; }
	float pixelPerMeter() const { return m_pixelPerMeter; }

public slots:
	void setGravity(double gravity) { m_gravity = gravity; }
	void setLaunchSpeedScale(double scale) { m_launchSpeedScale = scale; }
	void setPixelPerMeter(double pixelPerMeter) { m_pixelPerMeter = pixelPerMeter; }
	void setFreezeSimulation(bool freeze) { m_freezeSimulation = freeze; }

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void updateBullets(float deltaTime);
	void drawBullets(QPainter& painter) const;
	void drawLaunchArrow(QPainter& painter) const;
	void drawFloor(QPainter& painter) const;
	void drawBullet(QPainter& painter, const Bullet& bullet) const;
	void drawPrediction(QPainter& painter, const Bullet& bullet) const;
	void updateBulletPrediction();

	float m_pixelPerMeter = 10.0f;
	// Use getter to scale values by pixelPerMeter
	float m_gravity = 9.81f;
	float m_launchSpeedScale = 0.1f;

	int m_bulletSize = 5;
	int m_floorHeight = 50;
	int m_floorThickness = 5;

	std::vector<Bullet> m_bullets;
	std::optional<Bullet> m_launchingBullet = std::nullopt;
	QPoint m_currentMousePos;
	bool m_dragFloor = false;
	bool m_freezeSimulation = false;

	QColor m_bulletColor = Qt::red;
	QColor m_floorColor = Qt::gray;
	QColor m_predictionColor = Qt::blue;
};