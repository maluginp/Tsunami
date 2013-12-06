#include "devicewindow.h"
#include <QApplication>
#include "AnalysisWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);

    tsunami::AnalysisWindow w(1);
    w.show();
//    w.close();

//    tsunami::DeviceWindow w;
//    w.show();
    return a.exec();
}
