#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QSharedMemory sharedMemory;
    sharedMemory.setKey("avrDomotic");
    if(sharedMemory.attach()) {
        printf("Only one instance of avrDomotic at time!");
        return 0;
    }

    if (!sharedMemory.create(1)) {
        printf("Only one instance of avrDomotic at time!");
        return 0;
    }

    QApplication a(argc, argv);
    MainWindow w;
    //w.showFullScreen();
    w.show();
    return a.exec();
}
