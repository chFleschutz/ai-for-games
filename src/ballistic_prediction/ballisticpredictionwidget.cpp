#include "ballisticpredictionwidget.h"

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

BallisticPredictionWidget::BallisticPredictionWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto simWidget = ui.ballisticSim_widget;
	ui.gravity_doubleSpinBox->setValue(simWidget->gravityUnscaled());
	ui.launchSpeed_doubleSpinBox->setValue(simWidget->launchSpeedScaleUnscaled());
	ui.pixelPerMeter_doubleSpinBox->setValue(simWidget->pixelPerMeter());
}
