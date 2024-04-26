#include "aimainwindow.h"

#include "flowfield/flowfieldwidget.h"
#include "hillclimbing/hillclimbingwidget.h"
#include "utils/debugoutputdialog.h"

AIMainWindow::AIMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void AIMainWindow::onShowFlowField()
{
	changeCentralWidget(new FlowFieldWidget(this));
}

void AIMainWindow::onShowHillClimbing()
{
	changeCentralWidget(new HillClimbingWidget(this));
}

void AIMainWindow::onShowDebugOutput()
{
	DebugOutputDialog* dialog = new DebugOutputDialog(this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}

void AIMainWindow::changeCentralWidget(QWidget* widget)
{
	auto oldWidget = centralWidget();
	if (oldWidget)
	{
		oldWidget->hide();
		oldWidget->deleteLater();
	}

	setCentralWidget(widget);
}
