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

	if (m_mousePressed)
		drawLaunchArrow(painter);

	drawBullets(painter);
}

void BallisticSimWidget::mousePressEvent(QMouseEvent* event)
{
	m_mousePressPos = event->pos();
	m_currentMousePos = m_mousePressPos;
	m_mousePressed = true;
}

void BallisticSimWidget::mouseMoveEvent(QMouseEvent* event)
{
	m_currentMousePos = event->pos();
}

void BallisticSimWidget::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;

	auto dir = event->pos() - m_mousePressPos;
	auto velocity = QVector2D(dir) * m_launchSpeedScale;

	m_bullets.push_back({ QVector2D(m_mousePressPos), velocity });
}

void BallisticSimWidget::updateBullets(float deltaTime)
{
	for (auto& bullet : m_bullets)
	{
		bullet.position += bullet.velocity * deltaTime;
		bullet.velocity.setY(bullet.velocity.y() + (m_gravity * deltaTime));
	}

	// Out of bounds check 
	auto isOutOfBounds = [this](const auto& bullet) 
	{ 
		if (bullet.position.x() < 0 || bullet.position.x() > width())
			return true;
		if (bullet.position.y() < 0 || bullet.position.y() > height())
			return true;
		return false;
	};
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), isOutOfBounds), m_bullets.end());

	update();
}

void BallisticSimWidget::drawBullets(QPainter& painter)
{
	for (const auto& bullet : m_bullets)
	{
		painter.drawEllipse(bullet.position.x(), bullet.position.y(), m_bulletSize, m_bulletSize);
	}
}

void BallisticSimWidget::drawLaunchArrow(QPainter& painter)
{
	if (m_mousePressPos == m_currentMousePos)
		return;

	painter.drawLine(m_mousePressPos, m_currentMousePos);
}
