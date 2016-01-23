//#include "loader.h"
#include "Headers/MainApp.h"
#include "Headers/log.h"
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Loader loader;
//    loader.show();

    vector<QString> users;
    vector<Log> log;

    MainApp * app = new MainApp("", users, log, "");
    //MainApp * vgl = new MainApp("", users, log, "");
    app->show();

    return a.exec();
}
