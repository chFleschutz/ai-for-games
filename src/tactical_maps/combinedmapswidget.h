#pragma once

#include <QWidget>
#include "ui_combinedmapswidget.h"

class CombinedMapsWidget : public QWidget
{
	Q_OBJECT

public:
	CombinedMapsWidget(QWidget *parent = nullptr);
	~CombinedMapsWidget() = default;

private:
	Ui::CombinedMapsWidgetClass ui;
};
