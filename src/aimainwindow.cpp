#include "aimainwindow.h"

#include "flowfield/flowfieldwidget.h"
#include "hillclimbing/hillclimbingwidget.h"
#include "rqlearning/rqlearningwidget.h"
#include "tactical_maps/tacticalmapwidget.h"
#include "utils/debugoutputdialog.h"
#include "tactical_maps/combinedmapswidget.h"
#include "ballistic_prediction/ballisticpredictionwidget.h"

AIMainWindow::AIMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	changeCentralWidget<FlowFieldWidget>();
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

void AIMainWindow::onShowCombinedMaps()
{
	changeCentralWidget<CombinedMapsWidget>();
}

void AIMainWindow::onShowBallisticPrediction()
{
	changeCentralWidget<BallisticPredictionWidget>();
}

void AIMainWindow::onShowDebugOutput()
{
	DebugOutputDialog* dialog = new DebugOutputDialog(this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}
