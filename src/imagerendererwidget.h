#pragma once

#include <QWidget>
#include "ui_imagerendererwidget.h"
#include "imagerenderer.h"

class ImageRendererWidget : public QWidget
{
	Q_OBJECT

public:
	ImageRendererWidget(QWidget *parent = nullptr);
	~ImageRendererWidget() = default;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::ImageRendererWidgetClass ui;

	ImageRenderer m_imageRenderer;
};
