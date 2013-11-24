#include <QApplication>
#include "choiceanalysisform.h"
#include "addmeasureform.h"
//#include "analysisform.h"
//#include "plotdatawindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //    MeasureDataWindow mainWindow;
    //    mainWindow.show();

//    int deviceId = 1;
//    int analysisId = tsunami::ChoiceAnalysisForm::getAnalysisId( deviceId );
//    if( analysisId == -1){
//        Q_ASSERT(false);
//    }

    tsunami::addMeasureForm form(tsunami::addMeasureForm::EDIT,1,0);
    form.show();
    return a.exec();
}
