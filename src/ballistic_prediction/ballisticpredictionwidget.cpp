#include "ballisticpredictionwidget.h"

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

BallisticPredictionWidget::BallisticPredictionWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto simWidget = ui.ballisticSim_widget;
	ui.gravity_doubleSpinBox->setValue(simWidget->gravity());
	ui.launchSpeed_doubleSpinBox->setValue(simWidget->launchSpeedScale());
}
