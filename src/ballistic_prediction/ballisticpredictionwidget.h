#pragma once

#include "ui_ballisticpredictionwidget.h"

#include <QWidget>

class BallisticPredictionWidget : public QWidget
{
	Q_OBJECT

public:
	BallisticPredictionWidget(QWidget *parent = nullptr);
	~BallisticPredictionWidget() = default;

private:
	Ui::BallisticPredictionWidgetClass ui;
};
