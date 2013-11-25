#include "devicewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tsunami::DeviceWindow w;
    w.show();

    return a.exec();
}
