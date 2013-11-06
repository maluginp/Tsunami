#include <QApplication>
//#include "measuredatawindow.h"
#include "addmeasureform.h"
#include "analysisform.h"
#include "choiceanalysisdialog.h"
#include "plotdatawindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //    MeasureDataWindow mainWindow;
    //    mainWindow.show();


//    PlotDataWindow plot;
//    plot.show();

    int analysisId = choiceAnalysisDialog::openDialog();
    qDebug() << analysisId;
    if(analysisId == -1){ return 0;}

    addMeasureForm form(analysisId,0);
    form.show();

//    AnalysisForm form;
//    form.openAnalysis( 1 );
//    form.show();

    return a.exec();
}
