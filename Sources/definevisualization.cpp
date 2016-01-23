#include "Headers/definevisualization.h"
#include "ui_definevisualization.h"

DefineVisualization::DefineVisualization(MainApp *v, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DefineVisualization)
{
    ui->setupUi(this);
    this->vgl = v;

    //setColor();
    setPopUp();
}

DefineVisualization::~DefineVisualization()
{
    delete ui;
}

void DefineVisualization::setPopUp()
{

    map_user = new QSignalMapper();

    colors = vgl->gl[0]->colors2D;

    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //this->setFixedHeight(this->height()/2);

    //Menu izquierdo

    sl = new QScrollArea();
    sl->setWidgetResizable(true);
    sl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //sl->setFixedWidth(this->width()/4);
    sl->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setScrollStyle(sl);

    //wl_define =  new QWidget();
    vl_define = new QVBoxLayout();

    lb_define = new QGroupBox();
    lb_define->setLayout(vl_define);

    //wl_define->setLayout(vl_define);

    //Contenido

    mapper_events = new QSignalMapper();
    mapper_lines = new QSignalMapper();

    gl_define = new QGridLayout();

    for(int i=0;i<vgl->gl[0]->features.size();i++){

        QString event = vgl->gl[0]->features[i].name[0];

        //QLabel *c = new QLabel();

        ClickableLabel *c = new ClickableLabel(vgl,0,0,0,i);

        QLabel *l = new QLabel(event);
        c->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        l->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        c->setAutoFillBackground(true);
        c->setPalette(colors[i]);
        c->setFixedWidth(20);

        c_lines = new QCheckBox();
        c_lines->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        c_lines->setChecked(false);
        l_lines = new QLabel("Lines");
        l_lines->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        c_events = new QCheckBox();
        c_events->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        c_events->setChecked(true);
        l_events = new QLabel("Show");
        l_events->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


        //qDebug() << event;
        //gl_define->setColumnMinimumWidth(0,10);
        gl_define->addWidget(c,i,0,1,1);
        gl_define->addWidget(l,i,1,1,1);
        gl_define->addWidget(l_lines,i,2,1,1);
        gl_define->addWidget(c_lines,i,3,1,1);
        gl_define->addWidget(l_events,i,4,1,1);
        gl_define->addWidget(c_events,i,5,1,1);

        //Mapper
        connect(c_events, SIGNAL(clicked(bool)), mapper_events, SLOT(map()));
        connect(c_lines, SIGNAL(clicked(bool)), mapper_lines, SLOT(map()));

        mapper_events->setMapping(c_events, i);
        mapper_lines->setMapping(c_lines, i);

    }

    connect(mapper_events, SIGNAL(mapped(int)), this, SLOT(showEvents(int)));
    connect(mapper_lines, SIGNAL(mapped(int)), this, SLOT(showLines(int)));



    load_image = new QPushButton("Load image");
    load_image->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    setButtonStyle_ok(load_image);

    image_path = new QLabel("");
    image_path->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

//    gl_define->addWidget(load_image,vgl->gl[0]->features.size()+5,0,1,1);
//    gl_define->addWidget(image_path,vgl->gl[0]->features.size()+6,0,1,1);
    vl_define->addLayout(gl_define);
    vl_define->addWidget(load_image);
    vl_define->addWidget(image_path);


    sl->setWidget(lb_define);
    vl_define->addWidget(lb_define);
    ui->horizontalLayout_2->addWidget(sl);


    //Menu derecho
    sr = new QScrollArea();
    sr->setWidgetResizable(true);
    sr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //sr->setFixedHeight(this->height()/2.5);
    //sr->setFixedWidth(this->width()/2);
    sr->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setScrollStyle(sr);

    //wr_define =  new QWidget();
    vr_define = new QVBoxLayout();
    //wr_define->setLayout(vr_define);

    rb_define = new QGroupBox();
    rb_define->setLayout(vr_define);

    //Contenido
    gr_define = new QGridLayout();

    vector<QString> users = vgl->users;

    //qDebug() << users.size();


    for(int i=0;i<users.size();++i){

        QLabel *u = new QLabel(users[i]);
        u->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QLabel *l = new QLabel("");

        if(vgl->gl[0]->heat_users.size() != users.size()){
            l->setText("Path");
            vgl->gl[0]->heat_users.push_back("Path");
        } else {
            l->setText(vgl->gl[0]->heat_users[i]);
        }

        l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labels.push_back(l);

        QPushButton *b = new QPushButton("Change visualization");
        b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setButtonStyle_ok(b);

        gr_define->addWidget(u,i,0,1,1);
        gr_define->addWidget(l,i,1,1,1);
        gr_define->addWidget(b,i,2,1,1);

        connect(b, SIGNAL(clicked()), map_user, SLOT(map()));
        map_user->setMapping(b, i);
    }

    vr_define->addLayout(gr_define);

    sr->setWidget(rb_define);
    vr_define->addWidget(rb_define);
    ui->horizontalLayout_2->addWidget(sr);

    QPushButton *b = new QPushButton("Close");
    b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setButtonStyle_cancel(b);
    ui->verticalLayout->addWidget(b);

    connect(map_user, SIGNAL(mapped(int)), this, SLOT(changeVisualization(int)));
    connect(load_image, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(b, SIGNAL(clicked()), this, SLOT(closePopUp()));
}


void DefineVisualization::showEvents(int i){

    if(vgl->gl[0]->draw_events[i] == true){
        vgl->gl[0]->draw_events[i] = false;
        vgl->gl[0]->update();
    } else {
        vgl->gl[0]->draw_events[i] = true;
        vgl->gl[0]->update();
    }
}

void DefineVisualization::showLines(int i){

    if(vgl->gl[0]->draw_lines[i] == true){
        vgl->gl[0]->draw_lines[i] = false;
        vgl->gl[0]->update();
    } else {
        vgl->gl[0]->draw_lines[i] = true;
        vgl->gl[0]->update();
    }
}


void DefineVisualization::closePopUp(){

//    QMessageBox::StandardButton reply;

//    reply = QMessageBox::question(this, "Close", "Are you sure want to close?",
//    QMessageBox::No|QMessageBox::Yes);

//    if (reply == QMessageBox::Yes) {
//        this->close();
//    }

    this->close();

}

void DefineVisualization::changeVisualization(int i){

    cont = i;

    //Al clicar debe mostrar pop up para seleccionar modo de visualizacion
    QHBoxLayout *h_layout = new QHBoxLayout();
    QVBoxLayout *v_layout = new QVBoxLayout();

    v_layout->addLayout(h_layout);

    widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    widget->setMaximumHeight(widget->minimumHeight());
    widget->setMaximumWidth(widget->minimumWidth());

    c1 = new QCheckBox();
    c2 = new QCheckBox();
    QLabel *l1 = new QLabel("Path");
    QLabel *l2 = new QLabel("Heat Map");
    c1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);           
    c2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    l1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    l2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QPushButton *button = new QPushButton("Apply");
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    setButtonStyle_ok(button);

    //Marcamos a true la correcta
    bool encontrado = false;
    for(int i=0; i<labels.size();i++){
        if(labels[i]->text() != "Path"){
            encontrado = true;
        }
    }

    if(encontrado){
        c1->setChecked(false);
        c2->setChecked(true);
    } else {
        c1->setChecked(true);
        c2->setChecked(false);
    }

    h_layout->addWidget(c1);
    h_layout->addWidget(l1);
    h_layout->addWidget(c2);
    h_layout->addWidget(l2);
    v_layout->addWidget(button);

    widget->setLayout(v_layout);
    widget->show();

    connect(c1, SIGNAL(clicked(bool)), this, SLOT(setCheck1()));
    connect(c2, SIGNAL(clicked(bool)), this, SLOT(setCheck2()));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(changeType()));

}

void DefineVisualization::setCheck1(){

    if(c1->isChecked()) {
        c2->setChecked(false);
    }
}

void DefineVisualization::setCheck2(){

    if(c2->isChecked()) {
        c1->setChecked(false);
    }
}

void DefineVisualization::changeType(){

    if(c1->isChecked()){
        labels[cont]->setText("Path");

        //Pasar la variable a GLWidget
        vgl->gl[0]->vis_type = "Path";
        vgl->gl[0]->heat_users[cont] = "Path";
        vgl->update();
        widget->close();

    } else if(c2->isChecked()){
        labels[cont]->setText("Heat Map");
        vgl->gl[0]->vis_type = "Heat";
        vgl->gl[0]->heat_users[cont] = "Heat Map";
        vgl->gl[0]->heat_user = vgl->users[cont];
        vgl->gl[0]->heat_id = cont;

        for(int i=0;i<labels.size();++i){
            if(i!=cont) {
                labels[i]->setText("Path");
                vgl->gl[0]->heat_users[i] = "Path";
            }
        }

        vgl->gl[0]->getHeatMapData();
        vgl->update();
        //Pasar la variable a GLWidget
        widget->close();
    }
}

void DefineVisualization::loadImage(){

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Abrir archivo"),
                "",
                "All files (*.*);;JPG File (*.jpg);;JPEG File (*.jpeg);;PNG File (*.png);;TIFF File (*.tiff)"
                );

    if(filename != "" && QFileDialog::Accept) {

        //qDebug() << filename;
        image_path->setText(filename);
        load_image->hide();

        //Dibujamos imagen de fondo
        vgl->gl[0]->draw_background = true;
        vgl->gl[0]->background_path = filename;
        vgl->gl[0]->update();
    }
}
