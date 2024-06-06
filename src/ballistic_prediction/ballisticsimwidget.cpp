#include "ballisticsimwidget.h"

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

BallisticSimWidget::BallisticSimWidget(QWidget* parent)
	: QWidget(parent)
{
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [this]() { updateBullets(0.016f); });
	timer->start(16); // 60 FPS
}

void BallisticSimWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	drawFloor(painter);
	drawBullets(painter);

	if (m_launchingBullet.has_value())
	{
		drawLaunchArrow(painter);
		drawPrediction(painter, m_launchingBullet.value());
	}
}

void BallisticSimWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
		m_dragFloor = true;

	if (event->button() == Qt::LeftButton)
		m_launchingBullet = { .position = QVector2D(event->pos()) };
}

void BallisticSimWidget::mouseMoveEvent(QMouseEvent* event)
{
	m_currentMousePos = event->pos();

	if (m_dragFloor)
		m_floorHeight = height() - event->pos().y();

	if (m_launchingBullet.has_value())
	{
		m_launchingBullet->velocity = (QVector2D(event->pos()) - m_launchingBullet->position) * launchSpeedScale();
		updateBulletPrediction();
	}
}

void BallisticSimWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
		m_dragFloor = false;

	if (event->button() == Qt::LeftButton)
	{
		m_bullets.push_back(m_launchingBullet.value());
		m_launchingBullet.reset();
	}
}

void BallisticSimWidget::updateBullets(float deltaTime)
{
	if (m_freezeSimulation)
		return;

	for (auto& bullet : m_bullets)
	{
		bullet.position += bullet.velocity * deltaTime;
		bullet.velocity.setY(bullet.velocity.y() + (gravity() * deltaTime));
	}

	// Out of bounds check 
	auto isOutOfBounds = [this](const auto& bullet)
		{
			if (bullet.position.x() < 0 || bullet.position.x() > width())
				return true;
			// Dont Check if the bullet is above the ceiling, it will fall back down (y is flipped)
			if (bullet.position.y() > height())
				return true;
			return false;
		};
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), isOutOfBounds), m_bullets.end());

	update();
}

void BallisticSimWidget::drawBullets(QPainter& painter) const
{
	for (const auto& bullet : m_bullets)
	{
		drawBullet(painter, bullet);
		drawPrediction(painter, bullet);
	}
}

void BallisticSimWidget::drawLaunchArrow(QPainter& painter) const
{
	painter.setPen(Qt::black);
	painter.drawLine(m_launchingBullet->position.toPoint(), m_currentMousePos);
}

void BallisticSimWidget::drawFloor(QPainter& painter) const
{
	painter.setPen(m_floorColor);
	painter.setBrush(m_floorColor);

	QRect floor(0, height() - m_floorHeight, width(), m_floorThickness);
	painter.drawRect(floor);
}

void BallisticSimWidget::drawBullet(QPainter& painter, const Bullet& bullet) const
{
	QPoint offset(m_bulletSize / 2, m_bulletSize / 2);
	painter.setPen(Qt::transparent);
	painter.setBrush(m_bulletColor);
	painter.drawEllipse(bullet.position.toPoint() - offset, m_bulletSize, m_bulletSize);
}

void BallisticSimWidget::drawPrediction(QPainter& painter, const Bullet& bullet) const
{
	float radius = 5.0f;
	float x1 = bullet.predictedPosition.x() - radius;
	float x2 = bullet.predictedPosition.x() + radius;
	float y1 = bullet.predictedPosition.y() - radius;
	float y2 = bullet.predictedPosition.y() + radius;

	painter.setPen(QPen(m_predictionColor, 3));
	painter.drawLine(x1, y1, x2, y2);
	painter.drawLine(x1, y2, x2, y1);
}

void BallisticSimWidget::updateBulletPrediction()
{
	Q_ASSERT_X(m_launchingBullet.has_value(), "updateBulletPrediction", "No bullet to predict");

	auto startPos = m_launchingBullet->position;
	auto startVel = m_launchingBullet->velocity;
	auto groundY = static_cast<float>(height() - m_floorHeight);

	auto discriminant = std::powf(startVel.y(), 2) - 2 * gravity() * (startPos.y() - groundY);

	if (discriminant < 0)
		return;

	auto t1 = (-startVel.y() + std::sqrtf(discriminant)) / gravity();
	auto t2 = (-startVel.y() - std::sqrtf(discriminant)) / gravity();

	auto t = std::max(t1, t2);
	auto predictedX = startVel.x() * t + startPos.x();

	m_launchingBullet->predictedPosition = { predictedX, groundY };
}
