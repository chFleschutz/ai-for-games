#include "imagerendererwidget.h"

ImageRendererWidget::ImageRendererWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void ImageRendererWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	m_imageRenderer.draw(painter);
}
