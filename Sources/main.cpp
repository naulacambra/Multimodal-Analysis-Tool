//#include "loader.h"
#include "Headers/MainApp.h"
#include "Headers/log.h"
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filepath = "";
    QString videopath = "";

    MainApp * app = new MainApp(filepath, videopath);
    app->show();

    return a.exec();
}
