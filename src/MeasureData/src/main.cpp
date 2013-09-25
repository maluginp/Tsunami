#include <QApplication>
//#include "measuredatawindow.h"
#include "addmeasureform.h"
#include "analysisform.h"
#include "choiceanalysisdialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //    MeasureDataWindow mainWindow;
    //    mainWindow.show();



    int analysisId = choiceAnalysisDialog::open();
    qDebug() << analysisId;
    if(analysisId == -1){ return 0;}

    addMeasureForm form(analysisId,0);
    form.show();


    return a.exec();
}
