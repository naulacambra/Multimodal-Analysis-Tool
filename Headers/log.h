#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QDebug>
#include <QFile>

class Log
{
public:
    Log();
    Log(float t,QString u,QString e,float x,float y,float sz);

    //setters
    void setTimestamp(float t);
    void setUser(QString u);
    void setEvent(QString e);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);

    //getters
    float getTimestamp();
    QString getUser();
    QString getEvent();
    float getPosX();
    float getPosY();
    float getPosZ();

private:

    float timestamp;
    QString user;
    QString event;
    float posX;
    float posY;
    float posZ;
};

#endif // LOG_H
