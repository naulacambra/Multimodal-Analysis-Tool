#ifndef VIDEOLOG
#define VIDEOLOG

#include <vector>
#include <QColor>
#include <QString>

using namespace std;

typedef struct {
    int id;
    int timestamp;
    QColor color;
    int x;
    int y;
    QString note;
    QString tipo;
    QString categoria;
} VideoLog;


#endif // VIDEOLOG

