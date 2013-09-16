#include <QApplication>
#include "measuredatawindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MeasureDataWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
