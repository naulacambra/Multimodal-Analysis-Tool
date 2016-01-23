#include "Headers/log.h"

Log::Log()
{
    this->timestamp = 0;
    this->user = "";
    this->event = "";
    this->posX = -1;
    this->posY = -1;
    this->posZ = -1;
}

Log::Log(float t,QString u,QString e,float x,float y,float z){

    this->timestamp = t;
    this->user = u;
    this->event = e;
    this->posX = x;
    this->posY = y;
    this->posZ = z;
}

//setters
void Log::setTimestamp(float t){

    this->timestamp = t;
}
void Log::setUser(QString u){

    this->user = u;
}
void Log::setEvent(QString e){

    this->event = e;
}
void Log::setPosX(float x){

    this->posX = x;
}
void Log::setPosY(float y){

    this->posY = y;
}
void Log::setPosZ(float z){

    this->posZ = z;
}

//getters
float Log::getTimestamp(){

    return this->timestamp;
}
QString Log::getUser(){

    return this->user;
}
QString Log::getEvent(){

    return this->event;
}
float Log::getPosX(){

    return this->posX;
}
float Log::getPosY(){

    return this->posY;
}
float Log::getPosZ(){

    return this->posZ;
}
