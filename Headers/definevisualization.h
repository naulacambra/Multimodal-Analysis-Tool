#ifndef DEFINEVISUALIZATION_H
#define DEFINEVISUALIZATION_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QScrollArea>
#include <QSignalMapper>
#include <QFileDialog>
#include "MainApp.h"
#include "clickablelabel.h"
#include "styles.h"
#include <QMessageBox>
#include <QSignalMapper>
#include <QCheckBox>

namespace Ui {
class DefineVisualization;
}

class DefineVisualization : public QMainWindow
{
    Q_OBJECT

public:
    explicit DefineVisualization(MainApp *v, QWidget *parent = 0);
    ~DefineVisualization();

private:
    Ui::DefineVisualization *ui;

public:

    MainApp * vgl;

    QWidget * widget;
    QWidget *wl_define,*wr_define;
    QVBoxLayout *vl_define,*vr_define,*v_define;
//    QHBoxLayout * h_define;
    QGridLayout *gl_define,*gr_define;
    QGroupBox *rb_define, *lb_define;
    QPushButton *close_define,*continue_define,*image_define;
    QScrollArea *sl,*sr;
    ClickableLabel *c_label;

    QCheckBox *c_events,*c_lines;
    QLabel *l_events,*l_lines;

    QSignalMapper *map_user, *map_color,*mapper_events,*mapper_lines;
    QPushButton * load_image;
    QLabel *image_path;

    vector <QColor> colors;
    vector <QLabel*> labels;
    QCheckBox *c1,*c2;
    int cont;

    qreal h,s,v;
    int total_datos;
    float degrees;

    //Functions
    void setPopUp();

private slots:
    void changeVisualization(int i);
    void loadImage();
    void closePopUp();
    void setCheck1();
    void setCheck2();
    void changeType();
    void showEvents(int i);
    void showLines(int i);

};

#endif // DEFINEVISUALIZATION_H
