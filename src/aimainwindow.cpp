#include "aimainwindow.h"

#include "flowfield/flowfieldwidget.h"
#include "hillclimbing/hillclimbingwidget.h"
#include "rqlearning/rqlearningwidget.h"
#include "tactical_maps/tacticalmapwidget.h"
#include "utils/debugoutputdialog.h"

AIMainWindow::AIMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void AIMainWindow::onShowFlowField()
{
	changeCentralWidget<FlowFieldWidget>();
}

void AIMainWindow::onShowHillClimbing()
{
	changeCentralWidget<HillClimbingWidget>();
}

void AIMainWindow::onShowRQLearning()
{
	changeCentralWidget<RQLearningWidget>();
}

void AIMainWindow::onShowTacticalMaps()
{
	changeCentralWidget<TacticalMapWidget>();
}

void AIMainWindow::onShowDebugOutput()
{
	DebugOutputDialog* dialog = new DebugOutputDialog(this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}
