//#include "loader.h"
#include "Headers/visualizegl.h"
#include "Headers/log.h"
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Loader loader;
//    loader.show();

    vector<QString> u;
    vector<Log> log;

    visualizeGL * vgl = new visualizeGL("",u,log,"");
    vgl->show();

    return a.exec();
}
