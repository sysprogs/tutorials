#include "MainWindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    
    QFile file(":test.png");
    int len = file.size();
    
    w.show();
    
    return a.exec();
}
