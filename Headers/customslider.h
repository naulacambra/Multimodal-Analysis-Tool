#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QDebug>
#include <QSlider>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <vector>
#include <QColor>

using namespace std;

class customslider: public QSlider
{
    Q_OBJECT
public:
    explicit customslider(QWidget *parent = 0);

    vector <QPoint> points;
    //vector <int> x;
    int size;
    vector <int> timestamps;
    vector <QColor> color;
    QPoint point;

protected:
    void mousePressEvent ( QMouseEvent * event );
    void paintEvent(QPaintEvent *e);
};


#endif // CUSTOMSLIDER_H
