#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>
#include "log.h"
#include <vector>
#include <QPushButton>
#include <QSignalMapper>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QScrollArea>
#include <QDebug>
#include <QMessageBox>

#include "MainApp.h"

#include "clickablelabel.h"
#include "styles.h"

using namespace std;

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(MainApp *gl, QString path,QWidget *parent = 0);
    ~Menu();

private:
    Ui::Menu *ui;
    QString filepath;    
    MainApp *vgl;

    //Data LOG
    vector<Log> log;

    //Pulls
    vector<QString> users;
    vector<QString> users_event;
    vector<QString> events;
    vector<QString> features;

    //Layouts    
    QVBoxLayout *left_layout;
    QVBoxLayout *right_layout;    
    QGridLayout *grid;
    QGridLayout *grid_right;
    QGroupBox *leftBox;
    QGroupBox *rightBox;

    //Labels and buttons
    vector<QLabel*> users_labels;
    vector<QLabel*> users_labels_right;
    vector<QPushButton*> users_btns;
    vector<QPushButton*> users_btns_right;

    //vector<ClickableLabel*> events_label_right;
    vector<QLabel*> events_label_right;

    vector<QLineEdit*> _area;
    vector<QComboBox*> _select;
    vector<QPushButton*> _apply;

    //Buttons
    QPushButton *btn_back;
    QPushButton *btn_continue;

    //Mapper
    QSignalMapper* User_mapper;
    QSignalMapper* User_mapper_right;
    QSignalMapper* Event_mapper;

    //Functions
    void loadLog(QString path);
    void addLeftContent();
    void addRightContent(int id);
    void removeLayout(QGridLayout *layout);

//    vector<QString> getUsers(vector<Log>l);
//    vector<QString> getEvents(QString user,vector<Log>l);
//    vector<QString> getFeatures(QString user,QString event,vector<Log>l);
    vector<Log> setEventsByUser(QString user,QString event_old,QString event_new,vector<Log>l);

    int contador;
    int size_left;
    int size_right;

private slots:
    void addUserToList(int id);
    void returnUserToList(int id);
    void applyButton(QString id);
    void prevPage();
    void nextPage();
};

#endif // MENU_H
