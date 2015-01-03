#include "mathparsermainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathParserMainWindow w;
    w.show();

    return a.exec();
}
