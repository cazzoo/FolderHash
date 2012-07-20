#include <QApplication>
#include "mainWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icons/link"));

    MainWindow window;
    window.show();

    return app.exec();
}
