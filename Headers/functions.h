#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "log.h"
#include "videolog.h"
#include <QString>
#include "puntos.h"
using namespace std;

vector<QString> getCategories(vector<VideoLog>l);
vector<QColor> getColorCategories(vector<VideoLog>l);
vector<QString> getUsers(vector<Log>l);
vector<QString> getEvents(QString user,vector<Log>l);
Puntos getFeatures(QString user,QString event,vector<Log>l);
vector<Log> setEventsByUser(QString user,QString event_old,QString event_new,vector<Log>l);

#endif // FUNCTIONS_H
