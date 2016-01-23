#include "Headers/visualizegl.h"
#include "ui_visualizegl.h"
#include "Headers/clickablelabel.h"
#include "Headers/menu.h"
#include "Headers/definevisualization.h"
//#include "glwidget.h"
//#include <GLUT/glut.h>


GLwidget::GLwidget(class visualizeGL *v, vector<QString> u,vector<Log>l,int i,int s,QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    index_note = -1;

    users = u;
    log = l;
    index = i;
    screens = s;

    this->vis = v;
    c = Qt::red;

    vis_type = "Path";

    draw_background = false;
    background_path = "";    

    for(int j=0;j<users.size();++j){

        bool b = true;
        draw.push_back(b);
    }

    for(int i=0;i<screens;++i){
        minTimestamp.push_back(0);
        maxTimestamp.push_back(0);
    }

    loadData();

    for(int i=0;i<features.size();i++){
        bool b = true;
        bool b2 = false;
        draw_points.push_back(b);
        draw_lines.push_back(b2);

        draw_events.push_back(1);
        size_points.push_back(3);
        size_lines.push_back(0.6);
    }

    matriz.resize(this->width());
    for(int i = 0 ; i < this->width() ; ++i)
    {
        matriz[i].resize(this->height());
    }


    setColor();
    getMaxVal();
    setRanges();
    //setColorMap();

    for(int i=0;i<features.size();i++){
        for(int j = 0;j < (int)features[i].x.size();++j){
            if(min_valx < 0 || max_valx < 0) features[i].timestamp[j] = features[i].timestamp[j] *1000;
        }
    }
}

void GLwidget::setColorMap(){
    colormap.clear();
    degrees = (float)255/64;
    h=0;
    s=1;
    v=1;
    QColor c;

    //Generamos colormap
    for(int i=0;i<64;i++){
        c.setHsvF(h,s,v);
        c.toRgb();
        colormap.push_back(c.toRgb());
        h+=degrees/360;
        if(h > 1) h=1.0;
    }
}


void GLwidget::getHeatMapData(){

    setColorMap();

    //seteamos a cero
    for(int i=0;i<this->width();i++){
        for(int j=0;j<this->height();j++){
            matriz[i][j] = 0;
        }
    }

    int max_occ = -1;
    //Mapa de apariciones (pasar el id correcto que viene desde define vis.)
    for(int i=0;i<this->width();i++){
        for(int j=0;j<this->height();j++){
            for(int z=0;z<features.size();z++){
                if(features[z].user == heat_user){
                    for(int z1 = 0;z1 < (int)features[z].x.size();++z1){
                            float _x,_y;
                            _x = features[z].x[z1]*offsetX;
                            _y = features[z].y[z1]*offsetY;

                            if(min_valx < 0 || min_valy < 0){

                                offsetX = (float)this->width()*1.5/(float)rangex;
                                offsetY = (float)this->height()*1.5/(float)rangey;
                                _x+=this->width();
                                _y+=this->height();
                            }

                        //if(i== (int)features[0].x[z1] && j== (int)features[0].y[z1]){
                        //if(i== (int)_x/2 && j== (int)_y/2){
                        if((i <= ((int)_x/2) +15 && i >= ((int)_x/2) -15 ) && (j <= ((int)_y/2) +15 && j >= ((int)_y/2) -15 )){
                            //Creamos mapa de apariciones
                            matriz[i][j] += 1;
                            if(max_occ < matriz[i][j]) max_occ = matriz[i][j];
                        }

                        //Para coger samples, sino bugea el programa la cantidad de datos a procesar
                        if(features[z].x.size() > 5000 ) z1 += features[z].x.size()*0.005;

                    }
                }
            }
        }
    }


    //Normalizamos los valores
    for(int i=0;i<this->width();i++){
        for(int j=0;j<this->height();j++){
            if(matriz[i][j]!=0){
                matriz[i][j] = (float)matriz[i][j]/max_occ;
                matriz[i][j]*=colormap.size();
                //qDebug() << max_occ << matriz[i][j] << (float)max_occ/matriz[i][j];
            }
        }
    }

//    QTextStream out(stdout);

//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
//                       "matrix.txt",
//                       tr("Log (*.txt)"));

//    if(QFileDialog::Accept) {

//        // Create a new file
//        QFile file(fileName);
//        file.open(QIODevice::WriteOnly | QIODevice::Text);

//        for(int i=0;i<this->width();i++){
//            for(int j=0;j<this->height();j++){
//                QTextStream out(&file);
//                //out << ocurr_map[i][j];
//                out << matriz[i][j];
//                //out << colormap[matriz[i][j]].name();

//                int pos = (colormap.size()-1) - (matriz[i][j]);
//                //out << pos;
//            }
//            out << "\n";
//        }

//        file.close();
//    }


}

void GLwidget::loadData(){

    //Una única screen
    if(screens == 1){

        for (int i=0;i<users.size();i++){
            events = getEvents(users[i],log);
            for(int j=0;j<events.size();j++){

                //Genera features (deberá ser de categories TO DO)
                puntos = getFeatures(users[i],events[j],log);                
                //qDebug() << users[i] << " " << events[j] << " " << puntos.x.size() << " " << puntos.name[j];
                features.push_back(puntos);
            }
        }

    } else {

        //Más de una screen
        if(index < users.size()) {
            //Generar eventos del user
            events = getEvents(users[index],log);

            for(int j=0;j<events.size();j++){
                //Genera features (deberá ser de categories TO DO)
                puntos = getFeatures(users[index],events[j],log);
                features.push_back(puntos);
            }
        }

    }   
}


void GLwidget::getMaxVal(){

    max_valx = -1.0;
    max_valy = -1.0;
    min_valx = 0.0;
    min_valy = 0.0;

    for(int i=0;i<log.size();i++){
        //qDebug() << log[i].getPosX() << " " << log[i].getPosY();
        //Maximo
        if(max_valx < log[i].getPosX() ) max_valx = log[i].getPosX();
        if(max_valy < log[i].getPosY() ) max_valy = log[i].getPosY();
        //Minimo
        if(min_valx > log[i].getPosX() ) min_valx = log[i].getPosX();
        if(min_valy > log[i].getPosY() ) min_valy = log[i].getPosY();
    }


    //qDebug() << max_val;
   //if(max_valx == 0) max_valx = 10;

}

void GLwidget::setRanges(){
//    y = -1*this->width()/150;
//    x = -1*this->height()/150;
//    offsetY = this->height()/(float)((max_val*11)+(max_val*0.1));
//    offsetX = this->width()/(float)((max_val*11)+(max_val*0.1));

   rangex = max_valx + (-1*min_valx);
   rangey = max_valy + (-1*min_valy);

   offsetX = (float)this->width()/(float)rangex;
   offsetY = (float)this->height()/(float)rangey;

   //qDebug() << offsetX << " - " << offsetY;

}

void GLwidget::setColor(){

    total_datos = features.size();

//    if(total_datos == 0){
//        degrees = 0;
//    } else {
//        degrees = 360/total_datos;
//    }


    if(total_datos == 0){
        degrees = 0;
        h=0;
        s=1;
        v=1;
    } else {
        degrees = 360/total_datos;
        h=degrees/360;
        s=1;
        v=1;
    }

    for(int i=0;i<features.size();i++){
        QColor color;
        color.setHsvF(h,s,v);
        color.toRgb();
        colors2D.push_back(color);
        h+=degrees/360;
        if(h > 1) h=1.0;
    }

}

void GLwidget::mousePressEvent( QMouseEvent *event )
{

    QPoint p1 = this->mapFromGlobal(QCursor::pos());

    //qDebug() << p1.x() << " - " << p1.y();

    if(p1.x() > 0 && p1.y() > 0 && vis->players[0]->isVideoAvailable()){

        if(event->button() == Qt::RightButton && vis->categories.size() > 0) {

            _point = this->mapFromGlobal(QCursor::pos());
            //_timestamp = vis->player->position();
            points.push_back(_point);
            colors.push_back(c);
            //timestamp.push_back(_timestamp);
            timestamp.push_back(vis->player->position());
            //this->update();
            showPopUp();
        }

//    } else if(!vis->categories.size()==0 && event->button() == Qt::RightButton){
//        QMessageBox::information(NULL,"Error","Crea categorias antes de añadir comentarios");
//    } else if(!vis->players[0]->isVideoAvailable() && event->button() == Qt::RightButton){
//        QMessageBox::information(NULL,"Error","Carga primero video antes de añadir comentarios");
    }
}

void GLwidget::paintEvent(QPaintEvent *event )
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //Miramos si tenemos que pintar imagen de fondo
    if(draw_background && background_path != ""){
        QImage img;
        img.load(background_path);
        background = img.scaled(this->width(),this->height());
        painter.drawImage(0,0,background);
    }


    //Checkeamos que no se tenga que dibujar el heap map
    int heat_cont = 0;
    for(int i=0;i<heat_users.size();i++){
        if(heat_users[i]=="Path"){
            ++heat_cont;
        }
    }


    //DIBUJADO DE LINEAS Y PUNTOS
    if(heat_cont == heat_users.size() ){

        //PARA DIBUJAR LAS LINEAS

        for(int i=0;i<features.size();i++){

            color = colors2D[i];

            int c = 0;
            if(screens == 1) c = i;

            if(draw_lines[c]){

                for(int j = 1;j< (int)features[i].x.size();++j){

                    if((minTimestamp[index]==0 && maxTimestamp[index]==0) || (minTimestamp[index] <= features[i].timestamp[j] && maxTimestamp[index] >= features[i].timestamp[j] ) ){

                        if(features[i].x.size() > 1 && draw_events[i]) {

                            float _x,_y,_x2,_y2;
                            _x = features[i].x[j-1]*offsetX;
                            _y = features[i].y[j-1]*offsetY;

                            _x2 = features[i].x[j]*offsetX;
                            _y2 = features[i].y[j]*offsetY;

                            if(min_valx < 0 || min_valy < 0){

                                offsetX = (float)this->width()*1.5/(float)rangex;
                                offsetY = (float)this->height()*1.5/(float)rangey;

                                _x+=this->width();
                                _y+=this->height();

                                _x2+=this->width();
                                _y2+=this->height();
                            }


                              QPen linepen(color);
                              linepen.setWidth(size_lines[c]);
                              painter.setPen(linepen);
                              //painter.drawLine(features[i].x[j-1]*offsetX/2,features[i].y[j-1]*offsetY/2,features[i].x[j]*offsetX/2,features[i].y[j]*offsetY/2);
                              painter.drawLine(_x/2,_y/2,_x2/2,_y2/2);
                        }
                    }

                    if(features[i].x.size() > 5000 ) i += features[i].x.size()*0.005;
                 }
            }
        }

        //PARA DIBUJAR PUNTOS
        for(int i=0;i<features.size();i++){

            color = colors2D[i];

            int c = 0;
            if(screens == 1) c = i;

            if(draw_points[c]){

                for(int j = 0;j < (int)features[i].x.size();++j){

                    if((minTimestamp[index]==0 && maxTimestamp[index]==0) || (minTimestamp[index] <= features[i].timestamp[j] && maxTimestamp[index] >= features[i].timestamp[j] ) ){

                        if( (int)features[i].y.size()!= 0 && draw_events[i]){
                            QPen linepen(color);
                            //linepen.setWidth(1);
                            linepen.setWidth(size_points[c]/2);
                            painter.setPen(linepen);  

                            float _x,_y;

                            _x = features[i].x[j]*offsetX;
                            _y = features[i].y[j]*offsetY;

                            if(min_valx < 0 || min_valy < 0){

                                offsetX = (float)this->width()*1.5/(float)rangex;
                                offsetY = (float)this->height()*1.5/(float)rangey;

                                _x+=this->width();
                                _y+=this->height();
                            }

                            painter.setBrush(QBrush(color));
                            //painter.drawEllipse(QPointF(_x/2,_y/2), size_points[c]/4,size_points[c]/4);
                            painter.drawEllipse(QPointF(_x/2,_y/2), size_points[c],size_points[c]);

                        }

                    }
                }

            }
        }

    } // fin paths

    //Dibuja puntos del sistema si los hay
    for(int i=0;i<points.size();++i){

        //Idealmente pasarle el color desde visualizegl.cpp

        if((minTimestamp[index] == 0 && maxTimestamp[index] == 0) ||( timestamp.size() > 0 && (minTimestamp[index] <= timestamp[i] && maxTimestamp[index] >= timestamp[i] ) ) ){

            QPen linepen(colors[i]);
            linepen.setWidth(10);
            painter.setPen(linepen);
            painter.drawPoint(points[i]);
        }

        QPoint p1 = this->mapFromGlobal(QCursor::pos());

        if( (p1.x() - points[i].x() < 5 && p1.x() - points[i].x() > -5) && (p1.y() - points[i].y() < 5 && p1.y() - points[i].y() > -5)  ){
            index_note = i;
            this->update();
        } else {
            index_note = -1;
            this->update();
        }

        if(index_note != -1){

             QPen font_color(Qt::black);
             QFont font=painter.font() ;
             font.setPointSize(18);
             painter.setPen(font_color);
             painter.setFont(font);

             painter.drawText(QPoint(points[index_note].x(),points[index_note].y()-5), vis->videolog[index_note].note);
        }

    }


    //Heat Map si esta seleccionada la acción
    if(heat_cont != heat_users.size()){

        //getHeatMapData();

        for(int i=0;i<this->width();i++){
            for(int j=0;j<this->height();j++){
                int pos = (colormap.size()-1) - (matriz[i][j]);
                QPen linepen(colormap[pos]);
                painter.setPen(linepen);
                painter.drawPoint(QPoint(i,j));
            }
        }
   }
}

void GLwidget::showPopUp(){

    QVBoxLayout *layout = new QVBoxLayout();
    widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    text = new QPlainTextEdit();
    text->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    text->setPlaceholderText("ADD NOTES");
    text->setFixedHeight(widget->height()/6);
    //text->setFixedWidth(widget->width()/5);

    QPushButton *button = new QPushButton("Add Note");
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    setButtonStyle_ok(button);

    layout->addWidget(text);
    layout->addWidget(button);

    widget->setLayout(layout);
    widget->show();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(addNote()));
}

void GLwidget::addNote(){
    //vis->updateLog();
    setMouseTracking(false);
    vis->updateLogGL(vis->players[0]->position(),_point.x(),_point.y(),text->toPlainText());
    widget->close();
    setMouseTracking(true);
}

/////////// VISUALIZE GL /////////


visualizeGL::visualizeGL(QString path,vector<QString> u,vector<Log>l,QString video,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::visualizeGL)
{

    this->setWindowState(Qt::WindowMaximized);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Listener()));
    timer->start();

    ui->setupUi(this);

    filepath = path;
    videopath = video;
    //users = u;
    log = l;

    category_load = false;

    //loadHeader();

    countData();


    if(total_datos == 0){
        degrees = 0;
        h=0;
        s=1;
        v=1;
    } else {
        degrees = 360/total_datos;
        h=degrees/360;
        s=1;
        v=1;
    }

    scroll_linea_temporal = new QScrollArea();
    scroll_linea_temporal->setWidgetResizable(true);
    //scroll_linea_temporal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_linea_temporal->setFixedHeight(this->height()/3);
    scroll_linea_temporal->setFixedWidth(this->width());
    scroll_linea_temporal->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScrollStyle(scroll_linea_temporal);

    //w = new QWidget();
    p = new QVBoxLayout();    
    //w->setLayout(p);

    Box_timeline = new QGroupBox();
    Box_timeline->setLayout(p);

    loadMenu();


    scroll_linea_temporal->setWidget(Box_timeline);
    ui->horizontalLayout_2->addWidget(scroll_linea_temporal);

    scroll_categorias = new QScrollArea();
    scroll_categorias->setWidgetResizable(true);
    scroll_categorias->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_categorias->setFixedHeight(this->height()/3);
    scroll_categorias->setFixedWidth(this->width()/2);
    scroll_categorias->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScrollStyle(scroll_categorias);

    w1 = new QWidget;
    p1 = new QVBoxLayout();
    m1 = new QVBoxLayout();

    Box = new QGroupBox();
    Box->setFixedWidth(this->width()/2);
    Box->setFixedHeight(this->height()/3);
    Box->setLayout(m1);

    loadCategories();

    scroll_categorias->setWidget(Box);
    ui->horizontalLayout_2->addWidget(scroll_categorias);

    loadVideos();

    scroll_datos = new QScrollArea();
    scroll_datos->setWidgetResizable(true);
    scroll_datos->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_datos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scroll_datos->setMinimumHeight(this->height()/1.5);
    setScrollStyle(scroll_datos);

    //QWidget *w = new QWidget;
    widgets = new QVBoxLayout();
    grid = new QGridLayout();

    Box_data = new QGroupBox();
    Box_data->setLayout(widgets);

    //w->setLayout(widgets);
    widgets->addLayout(grid);

    loadWidgets();

    scroll_datos->setWidget(Box_data);
    ui->horizontalLayout_1->addWidget(scroll_datos);

    maxTimestamp = 0;
    int index_check_color = 0;
}

visualizeGL::~visualizeGL()
{
    delete ui;
}

void visualizeGL::loadHeader(){

//    header = new QHBoxLayout();
//    QLabel *l = new QLabel(title);
//    l->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    l->setAlignment(Qt::AlignCenter);
//    header->addWidget(l);
//    ui->verticalLayout->addLayout(header);

    btn_back = new QPushButton("Go Back");
    btn_back->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    connect(btn_back, SIGNAL(clicked(bool)), this, SLOT(prevPage()));
    //ui->verticalLayout->addWidget(btn_back);
    ui->horizontalLayout_3->addWidget(btn_back);

}

void visualizeGL::loadMenu(){

    mapper_minslider = new QSignalMapper();
    mapper_maxslider = new QSignalMapper();

    QLabel *t = new QLabel("Timeline");
    t->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    p->addWidget(t);

    grid_timeline = new QGridLayout();
    p->addLayout(grid_timeline);

    QLabel *label = new QLabel("Start: 0");
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSlider *slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(10);
    slider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    QLabel *label2 = new QLabel("End: 0");
    label2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QSlider *slider2 = new QSlider();
    slider2->setOrientation(Qt::Horizontal);
    slider2->setMaximum(10);
    slider2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    min_sliders.push_back(slider);
    max_sliders.push_back(slider2);

    min_label.push_back(label);
    max_label.push_back(label2);

    //Mapper
    connect(slider, SIGNAL(valueChanged(int)), mapper_minslider, SLOT(map()));
    mapper_minslider->setMapping(slider, 0);

    connect(slider2, SIGNAL(valueChanged(int)), mapper_maxslider, SLOT(map()));
    mapper_maxslider->setMapping(slider2, 0);


//    for(int i=0; i< 15;i++){
//        QString name = QString::number(i) + " - " + QString::number(i+1);
//        QLabel *t = new QLabel(name);
//        t->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//        QLabel *l = new QLabel();
//        l->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//        l->setAutoFillBackground(true);
//        l->setFixedWidth(this->width()/30);
//        //grid_timeline->setColumnMinimumWidth(i,5);
//        grid_timeline->setColumnStretch(i,10);
//        grid_timeline->addWidget(t,1,i,1,4);
//        grid_timeline->addWidget(l,2,3,1,4);
//        l->setPalette(Qt::red);
//    }

    //Prueba de linea temporal

    if(players.size()>0){        

        int timeline_range = ceil((float)players[0]->duration()/1000 );

        //qDebug() << timeline_range;

        for(int i=0;i<timeline_range;i++){

            QString name = QString::number(i) + " - " + QString::number(i+1);
            //QString name = " | "+QString::number(i) +"|";

            QLabel *t = new QLabel(name);
            t->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            //t->setFixedWidth(this->width()/20);

            if(timeline_range < 30) {
                grid_timeline->addWidget(t,1,i,1,4);
                grid_timeline->setColumnStretch(i,10);
            } else {
                grid_timeline->addWidget(t,1,i,1,1);
                grid_timeline->setColumnMinimumWidth(i,50);
                grid_timeline->setColumnStretch(i,10);
            }

        }        

        int min,max,value;

        for(int j=0;j<videolog.size();j++) {

            min = floor((float) players[0]->position()/1000);
            max = ceil((float) videolog[j].timestamp/1000);
            value = (float)videolog[j].timestamp/1000;

            if(category_load){
                min = floor((float) videolog[j].timestamp/1000);
                max = ceil((float) videolog[j].timestamp/1000);
                value = (float)videolog[j].timestamp/1000;

                //qDebug() << min << " " << max << " " << value << j;
            }

            if(min <= value && max >= value){
                //qDebug() << min << " " << max << " " << value << j;

                bool encontrado = false;

                for(int s=0;s<pos_timeline.size();++s){

                    if(!category_load && (pos_timeline[s] == min && category_prev[s] == videolog[j].categoria)){
                        encontrado = true;
                    }
                }

                if(!encontrado) {
                    pos_timeline.push_back(min);

                    //qDebug() << min;

                    QColor _c;
                    _c = videolog[j].color;
                    color_timeline.push_back(_c);

                    for(int i=0;i<categories.size();i++){
                        if(categories[i] == videolog[j].categoria){
                            indexs.push_back(i);
                            //qDebug() << i;
                            category_prev.push_back(videolog[j].categoria);
                        }
                    }

                }
            }

        }

        if(pos_timeline.size()>0) {

            //qDebug() << pos_timeline.size();

            for(int s=0;s<pos_timeline.size();++s){

                QLabel *l = new QLabel();
                l->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                l->setAutoFillBackground(true);

                //qDebug() << timeline_range;

                if(timeline_range < 100) l->setFixedWidth(this->width()/(timeline_range*2));
                else l->setFixedWidth(this->width()/((timeline_range/20)));
                l->setPalette(color_timeline[s]);

                //qDebug() << "adwidget " <<pos_timeline[s] << indexs[s] << indexs.size() << category_prev.size();
                grid_timeline->addWidget(l,indexs[s]+2,pos_timeline[s],1,4);

            }

        }

    }

    p->addWidget(label);
    p->addWidget(slider);
    p->addWidget(label2);
    p->addWidget(slider2);

    connect(mapper_minslider, SIGNAL(mapped(int)), this, SLOT(setMinTimestamp(int)));
    connect(mapper_maxslider, SIGNAL(mapped(int)), this, SLOT(setMaxTimestamp(int)));

    //qDebug() << "--------------";

}

void visualizeGL::loadCategories(){

    colorMapper = new QSignalMapper();

    QLabel *t = new QLabel("Categories");
    t->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    m1->addWidget(t);

    grid_category = new QGridLayout();
    grid_category_ctrls = new QGridLayout();
    m1->addLayout(grid_category);
    m1->addLayout(grid_category_ctrls);

    int cont = 0;

    //recorrer numero de categorias creadas (creada en un vector qLabel y QCheckbox)
    for(int j=0;j<categories.size();j++){

        //Colores
        QLabel *cpicker = new QLabel();
        cpicker->setAutoFillBackground(true);
        cpicker->setPalette(colors[j]);
        cpicker->setFixedWidth(20);
        cpicker->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

        //Labels
        QLabel *l = new QLabel();
        l->setText(categories[j]);

        QCheckBox *c = new QCheckBox();

        if(index_check_color == j) {
            c->setChecked(true);
        } else {
            c->setChecked(false);
        }
        cpickers.push_back(c);

        //Mapper
        connect(c, SIGNAL(clicked(bool)), colorMapper, SLOT(map()));
        QString data_map = QString::number(0) + "_" + QString::number(j);
        colorMapper->setMapping(c, data_map);

        //Añadimos al grid
        grid_category->addWidget(cpicker,cont+1,1,1,1);
        grid_category->addWidget(l,cont+1,2,1,1);
        grid_category->addWidget(c,cont+1,3,1,1);

        ++cont;
    }

    save_categories = new QPushButton("Save");
    save_categories->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    add_categories = new QPushButton("+");
    add_categories->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    load_categories = new QPushButton("Load");
    load_categories->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    setButtonStyle_save(save_categories);
    setButtonStyle_ok(load_categories);
    setButtonStyle_plus(add_categories);

    //Connect
    connect(colorMapper, SIGNAL(mapped(QString)), this, SLOT(setColor(QString)));
    connect(save_categories, SIGNAL(clicked(bool)), this, SLOT(SaveCategory()));
    connect(add_categories, SIGNAL(clicked(bool)), this, SLOT(AddCategory()));
    connect(load_categories, SIGNAL(clicked(bool)), this, SLOT(LoadCategory()));

    grid_category_ctrls->addWidget(add_categories,1,1,1,1);
    grid_category_ctrls->addWidget(load_categories,1,2,1,1);
    grid_category_ctrls->addWidget(save_categories,1,3,1,1);
}

void visualizeGL::loadWidgets(){

    int col = 0;
    int row = 0;

    if(total_datos == 0) maxTimestamp = 10;
    else maxTimestamp = getMaxTimestamp(0);

    GLwidget *g = new GLwidget(this,users,log,0,1);
//    g->setAutoFillBackground(true);
//    g->setPalette(Qt::red);
    gl.push_back(g);

//    QLabel *label = new QLabel("Start: 0");
//    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    QSlider *slider = new QSlider();
//    slider->setOrientation(Qt::Horizontal);
//    slider->setMaximum(maxTimestamp);
//    slider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

//    QLabel *label2 = new QLabel("End: 0");
//    label2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//    QSlider *slider2 = new QSlider();
//    slider2->setOrientation(Qt::Horizontal);
//    slider2->setMaximum(maxTimestamp);
//    slider2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

//    min_sliders.push_back(slider);
//    max_sliders.push_back(slider2);

//    min_label.push_back(label);
//    max_label.push_back(label2);

//    //Mapper
//    connect(slider, SIGNAL(valueChanged(int)), mapper_minslider, SLOT(map()));
//    mapper_minslider->setMapping(slider, 0);

//    connect(slider2, SIGNAL(valueChanged(int)), mapper_maxslider, SLOT(map()));
//    mapper_maxslider->setMapping(slider2, 0);


    //g->setMinimumSize(300,300);

    col = 0;

    QLabel *t = new QLabel("Data");
    t->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    dataloader = new QPushButton("Load Data");
    datavisualization = new QPushButton("Define Visualization");
    datatimer = new QLabel("");
    setButtonStyle_ok(dataloader);
    setButtonStyle_cancel(datavisualization);

    g->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    grid->setRowMinimumHeight(5+1,300);

    grid->addWidget(t,4,1,1,1);
    grid->addWidget(dataloader,5,1,1,1);
    grid->addWidget(datavisualization,5,2,1,1);
    grid->addWidget(datatimer,5,4,1,1);
    grid->addWidget(g,5+1,1,1,4);
//    grid->addWidget(label,5+2,1,1,4);
//    grid->addWidget(slider,5+3,1,1,4);
//    grid->addWidget(label2,5+4,1,1,4);
//    grid->addWidget(slider2,5+5,1,1,4);

    connect(dataloader, SIGNAL(clicked()), this, SLOT(LoadData()));
    connect(datavisualization, SIGNAL(clicked()), this, SLOT(DefineVis()));
//    connect(mapper_minslider, SIGNAL(mapped(int)), this, SLOT(setMinTimestamp(int)));
//    connect(mapper_maxslider, SIGNAL(mapped(int)), this, SLOT(setMaxTimestamp(int)));
}

void visualizeGL::loadVideos(){

    scroll_video = new QScrollArea();
    scroll_video->setWidgetResizable(true);
    scroll_video->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    scroll_video->setFixedHeight(this->height()/1.5);
//    scroll_video->setFixedWidth(this->width()/2);
    scroll_video->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScrollStyle(scroll_video);

    //QWidget *w1 = new QWidget;
    widgets1 = new QVBoxLayout();
    grid1 = new QGridLayout();

    Box_video = new QGroupBox();
    Box_video->setLayout(widgets1);

    //w1->setLayout(widgets1);
    widgets1->addLayout(grid1);

    pause_mapper = new QSignalMapper(this);
    stop_mapper = new QSignalMapper(this);
    frame_mapper = new QSignalMapper(this);

    //
    view = new QGraphicsView;
    item = new QGraphicsVideoItem;
    scene = new QGraphicsScene(view);
    //

    player = new QMediaPlayer;

    QPushButton *p = new QPushButton("Play");
    p->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    QPushButton *stop = new QPushButton("Stop");
    stop->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    QSlider *volume = new QSlider();
    volume->setOrientation(Qt::Horizontal);
    volume->setValue(100);
    volume->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    setButtonStyle_ok(p);
    setButtonStyle_cancel(stop);

//    customslider *spin = new customslider();
//    spin->setOrientation(Qt::Horizontal);
//    spin->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    QVideoWidget *videowidget = new QVideoWidget();
    videowidget->setFixedHeight(this->height()/1.5);
    videowidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    //player->setMedia(QUrl::fromLocalFile("/Users/kasta/Documents/TFG doc/H264_test8_voiceclip_mp4_480x320.mp4"));
    //player->setMedia(QUrl::fromLocalFile("/Users/kasta/Documents/TFG doc/H264_test2_Talkinghead_mp4_480x320.mp4"));
    //player->setVideoOutput(item);
    //player->play();

    //video = new videodrawer(this);
    view->setScene(scene);    
    item->setTransform(QTransform::fromScale(1.85,2.01), true);
    scene->addItem(item);

    video = new videodrawer(this,view);
    video->player = player;

    QLabel *t = new QLabel("Video");
    t->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);


    videoloader = new QPushButton("Load Video");
    videoloader->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    setButtonStyle_ok(videoloader);

    videotimer = new QLabel("");

    //grid1->addWidget(videowidget,i*3+1,1,1,2);
    grid1->addWidget(t,3,1,1,1);
    grid1->addWidget(videoloader,3+1,1,1,1);
    grid1->addWidget(videotimer,3+1,4,1,1);
    grid1->addWidget(view,3+2,1,1,4);
    grid1->addWidget(video,3+2,1,1,4);
    grid1->addWidget(p,3+3,1,1,1);
    grid1->addWidget(stop,3+3,2,1,1);
    grid1->addWidget(volume,3+3,4,1,1);

    //player->setMedia(QUrl::fromLocalFile(videopath));

    if(videos.size() > 0){
        videos.pop_back();
    }

    videos.push_back(video);


    players.push_back(player);
    pause.push_back(p);
    //frames.push_back(spin);

    connect(p, SIGNAL(clicked(bool)), pause_mapper, SLOT(map()));
    pause_mapper->setMapping(p, 0);

//    connect(spin, SIGNAL(valueChanged(int)), frame_mapper, SLOT(map()));
//    frame_mapper->setMapping(spin, 0);

    connect(stop, SIGNAL(clicked(bool)), stop_mapper, SLOT(map()));
    stop_mapper->setMapping(stop, 0);

    connect(videoloader, SIGNAL(clicked()), this, SLOT(LoadVideo()));
    connect(pause_mapper, SIGNAL(mapped(int)), this, SLOT(PauseVideo(int)));
    connect(stop_mapper, SIGNAL(mapped(int)), this, SLOT(StopVideo(int)));
    connect(volume, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    //connect(frame_mapper, SIGNAL(mapped(int)), this, SLOT(updateFrame(int)));

    scroll_video->setWidget(Box_video);
    ui->horizontalLayout_1->addWidget(scroll_video);

}

void visualizeGL::setVolume(int val){
    players[0]->setVolume(val);
}

void visualizeGL::AddCategory(){

    QHBoxLayout *h = new QHBoxLayout();
    widget_category = new QWidget();
    widget_category->setFixedHeight(100);
    widget_category->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    pick = false;
    ClickableLabel *lab = new ClickableLabel(this,0,0,-1,-1);
    lab->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    lab->setFixedWidth(20);
    lab->setAutoFillBackground(true);
    lab->setPalette(Qt::red);

    text = new QPlainTextEdit();
    text->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    text->setPlaceholderText("category descrption");
    text->setFixedHeight(35);
    text->setFixedWidth(widget_category->width()/3);

    QPushButton *button = new QPushButton("Add Category");
    button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setButtonStyle_ok(button);

    h->addWidget(lab);
    h->addWidget(text);
    h->addWidget(button);

    widget_category->setLayout(h);
    widget_category->show();

    connect(button, SIGNAL(clicked()), this, SLOT(addnewCategory()));
}

void visualizeGL::addnewCategory(){

    removeLayout(grid_category);
    removeLayout(grid_category_ctrls);
    removeLayout(m1);

    categories.push_back(text->toPlainText());

    if(!pick){
        colors.push_back(Qt::red);
    }

    pick = 0;

    cpickers.clear();
    loadCategories();

    widget_category->close();
}

void visualizeGL::SaveCategory(){


    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                       "dataLog.txt",
                       tr("Log (*.txt)"));


    //qDebug() << fileName;

    if(QFileDialog::Accept) {

        // Create a new file
        QFile file(fileName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);

        for(int i=0;i<videolog.size();++i){
            QTextStream out(&file);
            out  << videolog[i].timestamp << ";" <<  videolog[i].x << ";" << videolog[i].y << ";" << videolog[i].color.name() << ";" << videolog[i].note << ";" << videolog[i].tipo << ";" << videolog[i].categoria << "\n";
            //qDebug() << videolog[i].timestamp << " " << videolog[i].x << " " << videolog[i].y << " " << videolog[i].note;
        }

        file.close();

    }

}

void visualizeGL::LoadCategory(){

    //qDebug() << "load";

    videolog.clear();    

    //Borramos todos los puntos de categoria del video
    videos[0]->points.clear();
    videos[0]->colors.clear();
    videos[0]->update();

    //Borramos todos los puntos de los datos de categoria
    gl[0]->points.clear();
    gl[0]->colors.clear();
    gl[0]->update();

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Abrir archivo"),
                "",
                "Log File (*.txt)"
                );

    if(QFileDialog::Accept && filename != "") {

        loadCategoryLog(filename);

        categories.clear();
        cpickers.clear();

        categories = getCategories(videolog);
        colors = getColorCategories(videolog);

        min_sliders.clear();
        max_sliders.clear();
        min_label.clear();
        max_label.clear();
        removeLayout(grid_category);
        removeLayout(grid_category_ctrls);
        removeLayout(grid_timeline);
        removeLayout(p);
        loadCategories();

        category_load = true;
        loadMenu();
    }    
}

void visualizeGL::loadCategoryLog(QString path){

    //Leemos linea a linea
    QFile file(path);

    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          QStringList splitline = line.split(";");

          //qDebug() << splitline.size() << players[0]->isVideoAvailable();

          if(splitline.size() == 7 && players[0]->isVideoAvailable() && players[0]->state() == QMediaPlayer::StoppedState && players[0]->position() == 0 ){

              VideoLog vl;
              vl.timestamp = splitline[0].toFloat();
              vl.x = splitline[1].toFloat();
              vl.y = splitline[2].toFloat();
              QColor c(splitline[3]);
              vl.color = c;
              vl.note = splitline[4];
              vl.tipo = splitline[5];
              vl.categoria = splitline[6];

              videolog.push_back(vl);

              QPoint p(vl.x,vl.y);
              //qDebug() << p.x() << " - " << p.y();

              if(vl.tipo == "video"){
                  videos[0]->points.push_back(p);
                  videos[0]->colors.push_back(vl.color);
                  videos[0]->timestamp.push_back(vl.timestamp);

              } else {
                  gl[0]->points.push_back(p);
                  gl[0]->colors.push_back(vl.color);
                  gl[0]->timestamp.push_back(vl.timestamp);
              }

          } else if(!players[0]->isVideoAvailable()){
              QMessageBox::information(NULL,"Error","Load the video to load configuration");
              break;

          } else if(players[0]->position() != 0){
              QMessageBox::information(NULL,"Error","You can only load the configuration before playing video");
              break;
          } else {
              QMessageBox::information(NULL,"Format error","Format error");
              break;
          }
       }
       file.close();

       videos[0]->update();
       if(gl[0]->features.size() > 0) gl[0]->update();
    }
}


void visualizeGL::showUser(int i){


    if(gl[0]->draw[i] == true){
        gl[0]->draw[i] = false;
        gl[0]->update();
    } else {
        gl[0]->draw[i] = true;
        gl[0]->update();
    }

}

void visualizeGL::showLines(QString s){

    QStringList pieces = s.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];
    QString id3 = pieces[2];

    int id_user = id1.toInt();
    int id_event = id2.toInt();
    int cont = id3.toInt();

    if(gl[0]->draw_lines[cont] == true){
        gl[0]->draw_lines[cont] = false;
        gl[0]->update();
    } else {
        gl[0]->draw_lines[cont] = true;
        gl[0]->update();
    }
}


void visualizeGL::showPoints(QString s){

    //qDebug() << s;

    QStringList pieces = s.split("_");

    QString id3 = pieces[2];

    int cont = id3.toInt();

    if(gl[0]->draw_points[cont] == true){
        gl[0]->draw_points[cont] = false;
        gl[0]->update();
    } else {
        gl[0]->draw_points[cont] = true;
        gl[0]->update();
    }
}

void visualizeGL::showEvents(QString s){

    //qDebug() << s;

    QStringList pieces = s.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];
    QString id3 = pieces[2];

    int cont = id3.toInt();

    if(gl[0]->draw_events[cont] == true){
        gl[0]->draw_events[cont] = false;
        gl[0]->update();
    } else {
        gl[0]->draw_events[cont] = true;
        gl[0]->update();
    }
}

void visualizeGL::setPointSize(QString s){
    QStringList pieces = s.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];
    QString id3 = pieces[2];

    int cont = id3.toInt();

    float value = p_sliders[cont]->value();
    float val = (value/10.0)+10;
    gl[0]->size_points[cont] = val;
    gl[0]->update();
}

void visualizeGL::setLineSize(QString s){

    QStringList pieces = s.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];
    QString id3 = pieces[2];

    int cont = id3.toInt();

    float value = l_sliders[cont]->value();
    float val = (value/10.0)+1;
    gl[0]->size_lines[cont] = val;
    gl[0]->update();
}

int visualizeGL::getMaxTimestamp(int index){

    int max = -1;

    if(screens == 1){
        for(int i=0;i<features.size();++i){
            for(int j = 0;j < (int)features[i].x.size();++j){
                if(max < features[i].timestamp[j]) max = features[i].timestamp[j];
                 //qDebug() << features[i].timestamp[j]  << " - " << features[i].name[j];
            }
        }
    } else {
        // getEvent de user[index] y recorrer features igual que el anterior
        //qDebug() << "index" << index;
        unique_event = getEvents(users[index],log);

        for(int i=0;i<unique_event.size();++i){
            //Genera features (deberá ser de categories TO DO)
            unique_point = getFeatures(users[index],unique_event[i],log);
            //qDebug() << users[i] << " " << events[j] << " " << puntos.x.size() << " " << puntos.name[j];
            unique_feature.push_back(unique_point);
        }

        for(int i=0;i<unique_feature.size();++i){
            for(int j = 0;j < (int)unique_feature[i].x.size();++j){
                if(max < unique_feature[i].timestamp[j]) max = unique_feature[i].timestamp[j];
                 //qDebug() << unique_feature[i].timestamp[j]  << " - " << unique_feature[i].name[j];
            }
        }
    }

    return max;
}

void visualizeGL::setMinTimestamp(int i){

    int value = 0;

    if(players[0]->duration() > 300000 ){
        value = min_sliders[i]->value()*1000;
    } else {
        value = min_sliders[i]->value();
    }

    gl[i]->minTimestamp[i] = value;
    videos[i]->minTimestamp = value;
    QString l = "Start: "+ QString::number(value);
    min_label[i]->setText(l);
    gl[i]->update();
    videos[i]->update();



    if(players[0]->duration() > 300000 ){
        min_sliders[0]->setMaximum(player->duration()/1000);
        max_sliders[0]->setMaximum(player->duration()/1000);
    } else {
        min_sliders[0]->setMaximum(player->duration());
        max_sliders[0]->setMaximum(player->duration());
    }

}

void visualizeGL::setMaxTimestamp(int i){

    int value = 0;

    if(players[0]->duration() > 300000 ){
        value = max_sliders[i]->value()*1000;
    } else {
        value = max_sliders[i]->value();
    }

    gl[i]->maxTimestamp[i] = value;
    videos[i]->maxTimestamp = value;
    QString l = "End: "+ QString::number(value);
    max_label[i]->setText(l);
    gl[i]->update();

    if(players[i]->state() == QMediaPlayer::PausedState || players[i]->state() == QMediaPlayer::StoppedState) {

        if(players[0]->duration() > 300000 ){
            player->setPosition(value*1000);
        } else {
            player->setPosition(value);
        }
    }
    videos[i]->update();

//    QString time = QDateTime::fromTime_t(value).toString("hh:mm:ss");
//    datatimer->setText(time);

//    min_sliders[0]->setMaximum(player->duration());
//    max_sliders[0]->setMaximum(player->duration());

}

void visualizeGL::PauseVideo(int i){
    if(players[i]->state() == QMediaPlayer::PlayingState) {
        players[i]->pause();        
        pause[i]->setText("Play");
    }

   else if(players[i]->state() == QMediaPlayer::PausedState || players[i]->state() == QMediaPlayer::StoppedState) {        
        players[i]->play();
        pause[i]->setText("Pause");

        min_sliders.clear();
        max_sliders.clear();
        min_label.clear();
        max_label.clear();
        removeLayout(grid_timeline);
        removeLayout(p);
        loadMenu();


        if(players[0]->duration() > 300000 ){
            min_sliders[0]->setMaximum(player->duration()/1000);
            max_sliders[0]->setMaximum(player->duration()/1000);
        } else {
            min_sliders[0]->setMaximum(player->duration());
            max_sliders[0]->setMaximum(player->duration());
        }

    }
}


void visualizeGL::StopVideo(int i){
   pause[i]->setText("Play");
   players[i]->stop();
}

void visualizeGL::updateFrame(int i){
    frames[i]->setMaximum(players[i]->duration());
    //frames[i]->setRange(0, players[i]->duration());
    players[i]->setPosition(frames[i]->value());

    //qDebug() << QSlider::TicksAbove;
    //frames[i]->setTickPosition(QSlider::TicksRight);
}


void visualizeGL::setColorLabel(){

    color.setHsvF(h,s,v);
    color.toRgb();
    h+=degrees/360;
    if(h > 1) h=1.0;
}

void visualizeGL::setColor(QString s){

    QStringList pieces = s.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];    

    //qDebug() << "video " << id1 << "check " << id2;

    int video_id = id1.toInt();
    int color_id = id2.toInt();

    index_check_color = color_id;

    int inicio = 0;
    int fin = video_id+categories.size();

    //qDebug() << inicio << "  " << fin;

    for(int i=inicio;i<fin;i++){

        //qDebug() << "i: " << i <<  " c: "  << color_id << "form: "  << color_id+(video_id*4);

        if(i!=color_id){
            cpickers[i]->setChecked(false);
        } else {
            cpickers[i]->setChecked(true);
        }
    }

    //Asignamos color a videodrawer.cpp
    videos[video_id]->c = colors[color_id];
    gl[video_id]->c = colors[color_id];



    //Append del contenido
    //borramos contenido y cargamos de nuevo
    //removeLayout(p);
    //loadCategories();
}


void visualizeGL::removeLayout(QVBoxLayout *area){

    QLayoutItem * item;
    QWidget * widget;
    while ((item = area->takeAt(0))) {
        if ((widget = item->widget()) != 0) {widget->hide(); delete widget;}
        else {delete item;}
    }
}

void visualizeGL::removeLayout(QGridLayout *area){

    QLayoutItem * item;
    QWidget * widget;
    while ((item = area->takeAt(0))) {
        if ((widget = item->widget()) != 0) {widget->hide(); delete widget;}
        else {delete item;}
    }
}


void visualizeGL::refreshPoints()
{

    //removeLayout(m);
    removeLayout(grid_category);
    removeLayout(p);

    colors.clear();
    cpickers.clear();
    videolog.clear();
    frames.clear();



    //qDebug() << videos[s]->points.size();

    frames[0]->size = players[0]->duration();

    for(int i=0;i<videos[0]->points.size();++i){
        //qDebug() << "tstamp: " << videos[s]->timestamp[i] << "point: " << videos[s]->points[i].x() << " : " << videos[s]->points[i].y();

        VideoLog vl;
        vl.timestamp = videos[0]->timestamp[i];
        vl.x = videos[0]->points[i].x();
        vl.y =videos[0]->points[i].y();
        vl.color = videos[0]->colors[i];
        vl.id = 0;        
        videolog.push_back(vl);

        //Actualizamos datos del custom slider
        frames[0]->timestamps.push_back(videos[0]->timestamp[i]);
        //frames[s]->x.push_back(videos[s]->points[i].x());
        frames[0]->color.push_back(videos[0]->colors[i]);

        frames[0]->update();

        //qDebug() << frames[i]->timestamps[i];
        //qDebug() << videos[s]->colors[i];

    }


   loadCategories();
}

void visualizeGL::LoadVideo(){

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Abrir archivo"),
                "",
                "Video Files (*.mp4 *.avi *.mov *.mpeg *.mpg )"
                );

    if(QFileDialog::Accept) {

        //player->setMedia(QUrl::fromLocalFile("/Users/kasta/Documents/TFG doc/H264_test8_voiceclip_mp4_480x320.mp4"));
        //player->setMedia(QUrl::fromLocalFile("/Users/kasta/Documents/TFG doc/H264_test2_Talkinghead_mp4_480x320.mp4"));
        player->setMedia(QUrl::fromLocalFile(filename));
        player->setVideoOutput(item);


        min_sliders.clear();
        max_sliders.clear();
        min_label.clear();
        max_label.clear();
        removeLayout(grid_timeline);
        removeLayout(p);
        loadMenu();

        //player->play();
    }

    if(filename!="") QMessageBox::information(NULL,"Carga correcta","Video loaded");
    else QMessageBox::information(NULL,"Error","No video loaded");
}


void visualizeGL::LoadData(){

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Abrir archivo"),
                "",
                "Log File (*.txt)"
                );

    if(QFileDialog::Accept && filename != "" ) {

        //Cargamos contenido del widget
//        removeLayout(grid);

//        gl.clear();
//        min_sliders.clear();
//        max_sliders.clear();
//        min_label.clear();
//        max_label.clear();

        Menu *m = new Menu(this,filename);
        m->show();
        //this->close();

    }
}

void visualizeGL::DefineVis(){

    DefineVisualization *def_vis = new DefineVisualization(this);
    def_vis->show();
}

void visualizeGL::countData(){

    total_datos = 0;

    for (int i=0;i<users.size();i++){
        events = getEvents(users[i],log);
        for(int j=0;j<events.size();j++){
            ++total_datos;
        }
    }
}


void visualizeGL::Listener(){

    QString time = QDateTime::fromTime_t(players[0]->position()).toString("hh:mm:ss");
    videotimer->setText(time);

    //qDebug() << gl[0]->features.size() << " - " << player->position();    

    if( players[0]->position() != 0 ) {

        if(players[0]->state() == players[0]->PlayingState) {
            videos[0]->update();

            if(players[0]->duration() > 300000 ){
                max_sliders[0]->setSliderPosition(players[0]->position()/1000);
            } else {
                max_sliders[0]->setSliderPosition(players[0]->position());
            }


            //qDebug() << players[0]->position();

        }else {
            videos[0]->update();
            //player->setPosition(max_sliders[0]->value());
        }

        if(gl[0]->features.size() > 0){
            gl[0]->update();
        }
    }
}

void visualizeGL::updateLog(float t,float x,float y,QString text){

    //qDebug() << t << " " << x << " " << y << " " << text << " video";

    //removeLayout(p);

    //colors.clear();
    //cpickers.clear();
    //videolog.clear();
//    frames.clear();

//    frames[0]->size = players[0]->duration();

    VideoLog vl;
    vl.timestamp = t;
    vl.x = x;
    vl.y = y;
    vl.color = videos[0]->c;
    vl.id = 0;
    vl.note = text;
    vl.tipo = "video";
    vl.categoria = categories[index_check_color];

    //qDebug() << categories[index_check_color];

    videolog.push_back(vl);

    //Actualizamos datos del custom slider
//    frames[0]->timestamps.push_back(t);
//    frames[0]->color.push_back(videos[0]->c);

//    frames[0]->update();

   cpickers.clear();


   removeLayout(grid_category);
   removeLayout(grid_category_ctrls);
   removeLayout(m1);
   loadCategories();


   min_sliders.clear();
   max_sliders.clear();
   min_label.clear();
   max_label.clear();
   removeLayout(grid_timeline);
   removeLayout(p);
   loadMenu();

   if(players[0]->duration() > 300000 ){
      min_sliders[0]->setMaximum(player->duration()/1000);
      max_sliders[0]->setMaximum(player->duration()/1000);
      max_sliders[0]->setValue(player->position());
   } else {
       min_sliders[0]->setMaximum(player->duration());
       max_sliders[0]->setMaximum(player->duration());
       max_sliders[0]->setValue(player->position());
   }


}


void visualizeGL::updateLogGL(float t,float x,float y,QString text){

    //qDebug() << t << " " << x << " " << y << " " << text << " data 2D";

    //removeLayout(p);

    //colors.clear();
    cpickers.clear();
    //videolog.clear();

    VideoLog vl;
    vl.timestamp = t;
    vl.x = x;
    vl.y = y;
    vl.color = videos[0]->c;
    //vl.color = Qt::yellow;
    vl.id = 0;
    vl.note = text;
    vl.tipo = "data";
    vl.categoria = categories[index_check_color];
    videolog.push_back(vl);


    removeLayout(grid_category);
    removeLayout(grid_category_ctrls);
    removeLayout(m1);

    loadCategories();

    min_sliders.clear();
    max_sliders.clear();
    min_label.clear();
    max_label.clear();
    removeLayout(grid_timeline);
    removeLayout(p);
    loadMenu();



    if(players[0]->duration() > 300000 ){
       min_sliders[0]->setMaximum(player->duration()/1000);
       max_sliders[0]->setMaximum(player->duration()/1000);
       max_sliders[0]->setValue(player->position());
    } else {
        min_sliders[0]->setMaximum(player->duration());
        max_sliders[0]->setMaximum(player->duration());
        max_sliders[0]->setValue(player->position());
    }


}


videodrawer::videodrawer(class visualizeGL *v,QWidget *parent) :
    QWidget(parent)
{
    //inicializamos los valores
    setMouseTracking(true);
    this->vis = v;
    c = Qt::red;

    maxTimestamp = 0;
    minTimestamp = 0;
}


void videodrawer::mousePressEvent( QMouseEvent *event )
{
    QPoint p1 = this->mapFromGlobal(QCursor::pos());
    //qDebug() << p1.x() << " - " << p1.y();

    if(p1.x() > 0 && p1.y() > 0 && vis->players[0]->isVideoAvailable() ){
        if(event->button() == Qt::RightButton && vis->categories.size() > 0) {
            point = this->mapFromGlobal(QCursor::pos());
            points.push_back(point);
            colors.push_back(c);
            timestamp.push_back(player->position());
            //this->update();
            showPopUp();
        }

    }

//    else if(!vis->categories.size()==0 && event->button() == Qt::RightButton){
//        QMessageBox::information(NULL,"Error","Crea categorias antes de añadir comentarios");
//    } else if(!vis->players[0]->isVideoAvailable() && event->button() == Qt::RightButton){
//        QMessageBox::information(NULL,"Error","Carga primero video antes de añadir comentarios");
//    }

}

void videodrawer::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //omitimos la iteración 0 porque es el punto inicial que se ejecuta sin hacer click
    for(int i=0;i<points.size();++i){

        //qDebug() << minTimestamp << " - " << maxTimestamp << " - " << timestamp[i];

        if((minTimestamp == 0 && maxTimestamp == 0) ||( timestamp.size() > 0 && (minTimestamp <= timestamp[i] && maxTimestamp >= timestamp[i] ) ) ){

            //Idealmente pasarle el color desde visualizegl.cpp
            QPen linepen(colors[i]);
            linepen.setWidth(10);
            painter.setPen(linepen);
            painter.drawPoint(points[i]);
        }

        QPoint p1 = this->mapFromGlobal(QCursor::pos());
        if( (p1.x() - points[i].x() < 5 && p1.x() - points[i].x() > -5) && (p1.y() - points[i].y() < 5 && p1.y() - points[i].y() > -5)  ){
            index_note = i;
            this->update();
        } else {
            index_note = -1;
            this->update();
        }

        //qDebug() << points.size() << vis->videolog.size();

        if(index_note != -1){
             QPen font_color(Qt::black);
             QFont font=painter.font() ;
             font.setPointSize(18);
             painter.setPen(font_color);
             painter.setFont(font);
             painter.drawText(QPoint(points[index_note].x(),points[index_note].y()-5), vis->videolog[index_note].note);
        }

    }
}


void videodrawer::showPopUp(){

    QVBoxLayout *layout = new QVBoxLayout();
    widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    text = new QPlainTextEdit();
    text->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    text->setPlaceholderText("ADD NOTES");
    text->setFixedHeight(widget->height()/6);
    //text->setFixedWidth(widget->width()/5);

    QPushButton *button = new QPushButton("Add Note");
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    setButtonStyle_ok(button);

    layout->addWidget(text);
    layout->addWidget(button);

    widget->setLayout(layout);
    widget->show();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(addNote()));
}

void videodrawer::addNote(){

    setMouseTracking(false);
    vis->updateLog(player->position(),point.x(),point.y(),text->toPlainText());
    widget->close();
    setMouseTracking(true);
}

