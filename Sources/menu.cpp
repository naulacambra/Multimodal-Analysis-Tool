#include "Headers/menu.h"
#include "ui_menu.h"

Menu::Menu(MainApp *gl, QString path,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
//    this->setWindowState(Qt::WindowMaximized);
    //cargamos los datos del log
    loadLog(path);

    vgl = gl;
    filepath = path;

    ui->setupUi(this);

    //Layouts    
    left_layout = new QVBoxLayout();
    right_layout = new QVBoxLayout();


    leftBox = new QGroupBox();
    rightBox = new QGroupBox();

    //Buttons
    btn_continue = new QPushButton("Continue");
    btn_continue->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    connect(btn_continue, SIGNAL(clicked(bool)), this, SLOT(nextPage()));

    btn_back = new QPushButton("Close");
    btn_back->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    connect(btn_back, SIGNAL(clicked(bool)), this, SLOT(prevPage()));

    setButtonStyle_ok(btn_continue);
    setButtonStyle_cancel(btn_back);


    //Labels
    users = getUsers(log);

    //sizes
    size_left = users.size();
    size_right = 0;

    //Mapper
    User_mapper = new QSignalMapper(this);
    User_mapper_right = new QSignalMapper(this);
    Event_mapper = new QSignalMapper(this);

    addLeftContent();

    if(true){
        QScrollArea *sc = new QScrollArea();
        sc->addScrollBarWidget(leftBox,Qt::AlignCenter);
        sc->setWidget(leftBox);
        sc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sc->setFixedWidth(this->width()/3.9);
        sc->setFixedHeight(this->height()/1.2);
        ui->horitzontalLayout->addWidget(sc);

        setScrollStyle(sc);

    } else {
        ui->horitzontalLayout->addWidget(leftBox);
    }

    //Grid para label y bóton
    grid_right = new QGridLayout();
    right_layout->addLayout(grid_right);

    rightBox->setLayout(right_layout);
    rightBox->setFixedWidth(this->width());
    rightBox->setFixedHeight(this->height()/1.2);


    if(true){
        QScrollArea *sc = new QScrollArea();
        sc->addScrollBarWidget(rightBox,Qt::AlignCenter);
        sc->setWidget(rightBox);
        sc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sc->setFixedWidth(this->width()/2.4);
        sc->setFixedHeight(this->height()/1.2);
        ui->horitzontalLayout->addWidget(sc);

        setScrollStyle(sc);

    } else {

        rightBox->setFixedWidth(this->width()/2.4);
        rightBox->setFixedHeight(this->height()/1.2);
        ui->horitzontalLayout->addWidget(rightBox);
    }

    QHBoxLayout *h = new QHBoxLayout();
    ui->verticalLayout_10->addLayout(h);

    h->addWidget(btn_back);
    h->addWidget(btn_continue);

//    ui->verticalLayout_10->addWidget(btn_back);
//    ui->verticalLayout_10->addWidget(btn_continue);

}

Menu::~Menu()
{
    delete ui;
}

void Menu::loadLog(QString path){

    //Leemos linea a linea
    QFile file(path);

    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          QStringList splitline = line.split(";");

          int z;
          if(splitline.size() == 6) z = splitline[5].toInt();
          else z = -1;

          Log l = Log(splitline[0].toFloat(),splitline[1],splitline[2],splitline[3].toFloat(),splitline[4].toFloat(),-1);
          log.push_back(l);
       }
       file.close();
    }
}

void Menu::addLeftContent(){

    for(unsigned int i=0;i<users.size();i++){

        //Añadimos todas las labels al vector de labels
        QLabel *label = new QLabel(users[i]);
        label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        users_labels.push_back(label);         

        //Añadimos todas las labels al vector de labels
        QPushButton *btns = new QPushButton("Add");
        btns->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        users_btns.push_back(btns);

        setButtonStyle_ok(btns);

        //Mapper
        connect(btns, SIGNAL(clicked(bool)), User_mapper, SLOT(map()));
        User_mapper->setMapping(btns, i);
    }

    leftBox->setFixedWidth(this->width()/4);
    leftBox->setFixedHeight(this->height()/1.2);
    leftBox->setLayout(left_layout);

    //Grid para label y bóton
    grid = new QGridLayout();
    left_layout->addLayout(grid);

    //Mapper connector
    connect(User_mapper, SIGNAL(mapped(int)), this, SLOT(addUserToList(int)));

    for(unsigned int i=0;i<users_labels.size();i++){
        grid->addWidget(users_labels[i],i, 0, 1, 1);
        grid->addWidget(users_btns[i], i, 1, 1, 1);

        //left_layout->addWidget(users_labels[i]);
    }

    users_labels.clear();
    users_btns.clear();

    //ui->horitzontalLayout->addWidget(left_layout);

}

void Menu::addRightContent(int id){

    users_labels_right.clear();
    users_btns_right.clear();

    rightBox->setFixedWidth(this->width()/2.5);
    rightBox->setFixedHeight(this->height()/1.2);
    rightBox->setLayout(right_layout);

    int offset = 0;
    contador = 0;

    _area.clear();
    _apply.clear();
    _select.clear();
    events_label_right.clear();

    for(unsigned int i=0;i<users_event.size();i++){

        events = getEvents(users_event[i],log);

        //Añadimos todas las labels al vector de labels
        QLabel *label2 = new QLabel(users_event[i]);
        label2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        users_labels_right.push_back(label2);

        //Añadimos todas las labels al vector de labels
        QPushButton *btns2 = new QPushButton("Return");
        btns2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        users_btns_right.push_back(btns2);
        setButtonStyle_return(btns2);

        //Mapper
        connect(btns2, SIGNAL(clicked(bool)), User_mapper_right, SLOT(map()));
        //User_mapper_right->setMapping(btns2, id);
        User_mapper_right->setMapping(btns2, i);

        //Falla al insertar demasiados
        grid_right->addWidget(users_labels_right[i], i+offset, 0, 1, 1);
        grid_right->addWidget(users_btns_right[i], i+offset, 1, 1, 1);

        //qDebug() << " ini  " << i+offset;

        for(unsigned int j=0;j<events.size();++j){
            //Añadimos todos sus eventos
            QLabel *modify = new QLabel("Modifica");
            modify->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

            /* Create the comboBox */
            QComboBox   *select = new QComboBox();

            /* Create the ComboBox elements list (here we use QString) */
            QList<QString> list;
            list.append("Select one");
            list.append("Movement");
            list.append("Event");
            list.append("ID");

            select->addItems(list);

            QPushButton *apply = new QPushButton("Apply");
            apply->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            QLineEdit *area = new QLineEdit();
            area->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

            _apply.push_back(apply);
            _select.push_back(select);
            _area.push_back(area);

            QLabel *label_event = new QLabel(events[j]);
            label_event->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//            ClickableLabel *label_event = new ClickableLabel(events[j],modify,_area[contador],_select[contador],_apply[contador]);
//            label_event->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            events_label_right.push_back(label_event);

            //Mapper
            connect(apply, SIGNAL(clicked(bool)), Event_mapper, SLOT(map()));

            //concatenamos los strigs con las diferentes id's
            QString data_map = QString::number(contador) + "_" + QString::number(i);
            Event_mapper->setMapping(apply, data_map);

            grid_right->addWidget(events_label_right[contador], i+j+1+offset, 0, 1, 1);
            grid_right->addWidget(modify, i+j+1+offset, 1, 1, 1);
            grid_right->addWidget(_area[contador], i+j+1+offset, 2, 1, 1);
            grid_right->addWidget(_select[contador], i+j+1+offset, 3, 1, 1);
            grid_right->addWidget(_apply[contador], i+j+1+offset, 4, 1, 1);

            //Ocultamos para mostrar al clickar sobre el label
            modify->hide();
            _area[contador]->hide();
            _select[contador]->hide();
            _apply[contador]->hide();

//            features = getFeatures(users_event[i],events[j],log);
//            for(int z=0;z<features.size();++z){
//                //qDebug() << j << events[j] << " - " << features[z];
//            }
            //qDebug() << " cont  " << i+j+1+offset;


            contador++;
        }

        offset+= events.size();
    }

    //Mapper connector
    connect(User_mapper_right, SIGNAL(mapped(int)), this, SLOT(returnUserToList(int)));
    //connect(Event_mapper, SIGNAL(mapped(int)), this, SLOT(applyButton(int)));
    connect(Event_mapper, SIGNAL(mapped(QString)), this, SLOT(applyButton(QString)));
}

void Menu::addUserToList(int id){

    //Aquí deberiamos añadir mediante la id el user y los eventos del mismo en el right_layout
    // (permitir quitar y poner cambiando el botón de add por remove )

        //qDebug() << size_left << " " << size_right;

        --size_left;
        ++size_right;

        users_event.push_back(users[id]);
        User_mapper_right->disconnect();
        Event_mapper->disconnect();
        removeLayout(grid_right);
        addRightContent(id);

        //users_event.clear();

        users.erase(std::remove(users.begin(), users.end(), users[id]), users.end());
        removeLayout(grid);
        User_mapper->disconnect();
        addLeftContent();
}

void Menu::returnUserToList(int id){

    --contador;

    //qDebug() << size_left << " " << size_right;
    ++size_left;
    --size_right;

    //limpiamos toda la parte derecha para añadir uno de nuevo en el vector y volver a dibujar
    users.push_back(users_event[id]);
    removeLayout(grid);
    User_mapper->disconnect();
    addLeftContent();

    users_event.erase(std::remove(users_event.begin(), users_event.end(), users_event[id]), users_event.end());
    removeLayout(grid_right);
    User_mapper_right->disconnect();
    Event_mapper->disconnect();
    addRightContent(id);
}


void Menu::removeLayout(QGridLayout *grid){

    QLayoutItem * item;
    QWidget * widget;
    while ((item = grid->takeAt(0))) {
        if ((widget = item->widget()) != 0) {widget->hide(); delete widget;}
        else {delete item;}
    }
}

void Menu::nextPage(){

    //Cargamos la siguiente pantalla y cerramos la anterior
//    mp = new MainPage(filepath,users_event,log);
//    mp->show();

    vgl->removeLayout(vgl->grid);
    vgl->gl.clear();

    vgl->users.clear();
    vgl->log.clear();
    vgl->events.clear();
    vgl->features.clear();
    vgl->p_sliders.clear();
    vgl->l_sliders.clear();

    vgl->users = users_event;
    vgl->log = log;
    vgl->getMaxTimestamp(0);
    vgl->countData();

    vgl->degrees = 360/vgl->total_datos;
    vgl->h=vgl->degrees/360;
    vgl->s=1;
    vgl->v=1;

    //vgl->loadMenu();
    vgl->loadWidgets();    
    vgl->update();

    this->close();
}

void Menu::prevPage(){

    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Close", "Are you sure want to close?",
    QMessageBox::No|QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        this->close();
    }
}

void Menu::applyButton(QString id){

    //Parseamos el id para obtener tanto el id del evento como el de usuario
    QStringList pieces = id.split("_");
    QString id1 = pieces[0];
    QString id2 = pieces[1];

    int id_event = id1.toInt();
    int id_user = id2.toInt();

    //Actualizar los datos rellenados
    QString text = _area[id_event]->text();
    QString select_text = _select[id_event]->currentText();
    int index = _select[id_event]->currentIndex();

    if(text != ""){
       log = setEventsByUser(users_event[id_user],events_label_right[id_event]->text(),_area[id_event]->text(),log);
       events_label_right[id_event]->setText(text);
    }

    if(index){
        //Aquí deberia guardar la nueva categoria en algun lugar/vector para pasarselo a la nueva siguiente página
        //qDebug() << select_text;
    }
}


vector<Log> Menu::setEventsByUser(QString user,QString event_old,QString event_new,vector<Log>l){

    //qDebug() << user << " - " <<  event_old << " - "<< event_new;

    for(unsigned int i=0;i<l.size();i++){
        QString u = l[i].getUser();
        if(user == u){
            QString e = l[i].getEvent();
            if(e == event_old){
                l[i].setEvent(event_new);
            }
        }
    }

    return l;

}
