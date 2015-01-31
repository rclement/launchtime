#include <QtGui/QApplication>
#include "LaunchtimeWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LaunchtimeWindow win;
    win.show();

    return app.exec();
}

