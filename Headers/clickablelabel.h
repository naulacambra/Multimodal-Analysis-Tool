#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMediaPlayer>
#include <QColorDialog>
#include "MainApp.h"
#include "customslider.h"

class ClickableLabel : public QLabel
{
Q_OBJECT

public:
    explicit ClickableLabel(MainApp *gl=0, QMediaPlayer* p =0,customslider* s=0, int t = -1,int i=-1, QWidget * parent = 0 );
    ~ClickableLabel();

    //QObjects    
    MainApp *vgl;
    QMediaPlayer* _player;
    customslider* slider;
    int type;
    int timestamp;
    int index;

signals:
    void clicked();

public slots:
    void labelClicked();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
};
#endif // CLICKABLELABEL_H
