#pragma once

#include "ui_ballisticpredictionwidget.h"

#include <QWidget>
#include <QVector2D>

class BallisticPredictionWidget : public QWidget
{
	Q_OBJECT

public:
	BallisticPredictionWidget(QWidget* parent = nullptr);
	~BallisticPredictionWidget() = default;

private:
	Ui::BallisticPredictionWidgetClass ui;
};
