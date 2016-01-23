#include "Headers/customslider.h"

customslider::customslider(QWidget *parent) :
    QSlider(parent)
{

}


void customslider::mousePressEvent( QMouseEvent *event )
{
    QSlider::mousePressEvent(event);
}

void customslider::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    float offset = (float)this->width()/(float)size;

    for(int i=0;i<timestamps.size();++i){
        QPen linepen(color[i]);
        linepen.setWidth(10);
        painter.setPen(linepen);

        //qDebug() << offset;

        QPoint p(offset*timestamps[i],0);
        painter.drawPoint(p);
    }

    QSlider::paintEvent(e);
}
