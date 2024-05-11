#pragma once

#include <QWidget>
#include "ui_tacticalmapwidget.h"

class TacticalMapWidget : public QWidget
{
	Q_OBJECT

public:
	TacticalMapWidget(QWidget *parent = nullptr);
	~TacticalMapWidget();

private:
	Ui::TacticalMapWidgetClass ui;
};
