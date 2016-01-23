#ifndef MAINAPP_H
#define MAINAPP_H

#include "styles.h"

#include <QMainWindow>
#include <math.h>
#include <QString>
#include <QDebug>

#include <QDateTime>

#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QPlainTextEdit>

#include <QGLWidget>
#include <QTimer>
#include "log.h"
#include "functions.h"
#include "puntos.h"

#include <vector>

#include <QGridLayout>
#include <QGroupBox>
#include <QBoxLayout>
#include <QLabel>
#include <vector>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QSignalMapper>
#include <QSlider>
#include <QStringList>
#include <QScrollArea>
#include <QScrollBar>
#include <QMediaPlayer>
#include <QSignalMapper>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "videolog.h"
#include "customslider.h"

//#include "videodrawer.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>

#include "log.h"
#include "functions.h"
#include "puntos.h"

using namespace std;

////////////////// VIDEODRAWER ////////////////////

class videodrawer: public QWidget
{
    Q_OBJECT
public:
    explicit videodrawer(class MainApp *v,QWidget *parent = 0);

    //functions
    void showPopUp();

    //variables
    QMediaPlayer *player;
    QVBoxLayout *layout;
    vector <QPoint> points;
    vector <int> timestamp;
    vector <QColor> colors;
    QColor c;

    int minTimestamp,maxTimestamp;
    int index_note;

    class MainApp *vis;
    QWidget * widget;
    QPlainTextEdit *text;

protected:
    void mousePressEvent ( QMouseEvent * event );
    void paintEvent(QPaintEvent *e);

private:
    QPoint point;

private slots:
    void addNote();

};

///////////// OPEN GL  /////////////


class GLwidget : public QWidget
{
    Q_OBJECT
public:
    explicit GLwidget(class MainApp *v, vector<QString> u,vector<Log>l,int i,int s,QWidget *parent = 0);

    void loadData();
    void getMaxVal();
    void setRanges();
    void setColor();
    void getHeatMapData();
    void setColorMap();

    void showPopUp();   

    QString vis_type;

    QImage background;
    bool draw_background;
    QString background_path;

    vector <QColor> colors;
    QColor c;

    QTimer timer;
    vector<bool> draw;
    vector <bool> draw_points;
    vector <bool> draw_lines;
    vector <float> size_points;
    vector <float> size_lines;

    vector <QPoint> points;
    vector <float> timestamp;
    vector <QColor> colors2D;

    vector<int> draw_events;
    int screens;

    QPoint _point;
    float _timestamp;
    QWidget * widget;
    QPlainTextEdit *text;

    class MainApp *vis;

    //vector<QColor> color;
    QColor color;
    float degrees;
    int total_datos;

    vector<int> minTimestamp;
    vector<int> maxTimestamp;
    qreal h;
    qreal s;
    qreal v;

    vector<QString> users;
    vector<QString> events;
    vector<Puntos> features;
    vector<Log>log;
    Puntos puntos;
    int index;

    int heat_id;
    QString heat_user;
    vector <QString> heat_users;
    vector<QColor> colormap;
    std::vector< std::vector<float> > matriz;

    //Variable to draw
    float y;
    float x;
    float offsetY;
    float offsetX;
    float max_valx,max_valy;
    float min_valx,min_valy;
    float rangex,rangey;

    QPoint note_popup;
    int index_note;


protected:
    void mousePressEvent ( QMouseEvent * event );
    //void mouseMoveEvent ( QMouseEvent * event );
    void paintEvent(QPaintEvent *e);

private slots:
    void addNote();

};

////////////////// MainApp ////////////////////

namespace Ui {
    class MainApp;
}

class MainApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainApp(QString filepath,vector<QString> u,vector<Log>l,QString video, QWidget *parent = 0);
    ~MainApp();

    //layouts
    QHBoxLayout *header;
    QVBoxLayout *menu;
    QVBoxLayout *widgets,*widgets1;
    QGridLayout *grid,*grid1;
    vector<QSlider*> p_sliders;
    vector<QSlider*> l_sliders;
    vector<QSlider*> min_sliders;
    vector<QSlider*> max_sliders;
    vector<QLabel*> min_label;
    vector<QLabel*> max_label;
    QGroupBox *box;

    QLabel *videotimer,*datatimer;

    //Menu
    QPlainTextEdit *text;
    QPushButton *save_categories,*load_categories,*add_categories;

    vector<int> indexs;
    vector<QString> category_prev;

    //Mappers
    QSignalMapper *mapper;
    QSignalMapper *mapper_points;
    QSignalMapper *mapper_lines;
    QSignalMapper *mapper_pslider;
    QSignalMapper *mapper_lslider;
    QSignalMapper *mapper_event;

    QSignalMapper *mapper_minslider;
    QSignalMapper *mapper_maxslider;

    QSignalMapper *colorMapper;        

    //functions
    void loadHeader();
    void loadMenu();
    void loadCategories();
    void loadWidgets();
    void loadVideos();
    void removeLayout(QVBoxLayout *area);
    void removeLayout(QGridLayout *area);

    void updateLog(float t,float x,float y,QString text);
    void updateLogGL(float t,float x,float y,QString text);

    void loadCategoryLog(QString path);

    //int getMinTimestamp();
    int getMaxTimestamp(int i);
    int maxTimestamp;
    bool category_load;

    vector <QString> categories;
    bool pick;

    vector<QString> users;
    vector<QString> initial_users;
    vector<QString> events;
    vector<QString> unique_event;
    vector<Log> log;

    int check_static;
    int check_dynamic;
    int screens;
    int index_check_color;
    QString title;
    vector<GLwidget*> gl;
    vector<Puntos> features;
    vector<Puntos> unique_feature;
    Puntos puntos;
    Puntos unique_point;
    QString filepath;
    QString videopath;
    QString videopath2;
    QPushButton *btn_back;
    QPushButton *refresh;
    QPushButton *videoloader,*dataloader,*datavisualization;

    vector<int> pos_timeline;
    vector<QColor> color_timeline;

    QGridLayout * grid_timeline,*grid_category_ctrls;

    vector <QMediaPlayer*> players;
    //vector <QVideoWidget*> videos;
    vector <videodrawer*> videos;
    vector <QPushButton*> pause;
    //vector <QSlider*> frames;
    vector <customslider*> frames;

    QSignalMapper* pause_mapper;
    QSignalMapper* frame_mapper;
    QSignalMapper* stop_mapper;

    QPoint point;

    //Video Drawer
    QGraphicsView *view;
    QGraphicsVideoItem *item;
    QGraphicsScene *scene;
    QMediaPlayer *player;
    videodrawer * video;

    //Color picker
    vector<QCheckBox*> cpickers;
    vector<QLabel*> videoColors;
    vector<QColor> colors;


    QScrollArea *scroll_video,*scroll_datos,*scroll_categorias,*scroll_linea_temporal;
    QWidget *w,*w1,*widget_category;
    QVBoxLayout *m,*m1;
    QVBoxLayout *p,*p1;
    QGridLayout *grid_category;
    QGroupBox * Box,*Box_data,*Box_video,*Box_timeline;

    vector<VideoLog> videolog;

    QColor color;
    float degrees;
    int total_datos;

    qreal h;
    qreal s;
    qreal v;
    QTimer *timer;

private:
    Ui::MainApp *ui;

public slots:
    void showUser(int i);
    void showPoints(QString s);
    void showLines(QString s);
    void showEvents(QString s);
    void setPointSize(QString s);
    void setLineSize(QString s);
    void setMinTimestamp(int i);
    void setMaxTimestamp(int i);    

    void PauseVideo(int i);
    void StopVideo(int i);
    void updateFrame(int d);    

    void LoadVideo();
    void LoadData();
    void DefineVis();

    void setColor(QString s);
    void setColorLabel();
    void countData();
    void refreshPoints();

    void Listener();

    void AddCategory();
    void SaveCategory();
    void LoadCategory();

    void addnewCategory();
    void setVolume(int val);

};


#endif // MainApp_H
