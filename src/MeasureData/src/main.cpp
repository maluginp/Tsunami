#include <QApplication>
//#include "measuredatawindow.h"
#include "addmeasureform.h"
//#include "analysisform.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    MeasureDataWindow mainWindow;
//    mainWindow.show();

    addMeasureForm form(1,0);
//    AnalysisForm form;
    form.show();

    return a.exec();
}
