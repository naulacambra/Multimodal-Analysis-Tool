#include "Headers/functions.h"

vector<QString> getCategories(vector<VideoLog>l){

    vector<QString> result;

    for(int i=0;i<l.size();i++){
        QString s = l[i].categoria;

        if (!(std::find(result.begin(), result.end(), s ) != result.end()))
        {
          // Añadimos los elementos una única vez
          result.push_back(s);
        }

    }
    return result;
}

vector<QColor> getColorCategories(vector<VideoLog>l){

    vector<QColor> result;

    for(int i=0;i<l.size();i++){
        QString s = l[i].color.name();

        if (!(std::find(result.begin(), result.end(), s ) != result.end()))
        {
          // Añadimos los elementos una única vez

          result.push_back(l[i].color);
        }

    }
    return result;
}

vector<QString> getUsers(vector<Log>l){

    vector<QString> result;

    for(int i=0;i<l.size();i++){
        QString s = l[i].getUser();

        if (!(std::find(result.begin(), result.end(), s ) != result.end()))
        {
          // Añadimos los elementos una única vez
          result.push_back(s);
        }
    }
    return result;
}

vector<QString> getEvents(QString user,vector<Log>l){

    vector<QString> result;

    for(int i=0;i<l.size();i++){
        QString u = l[i].getUser();
        if(user == u){
            QString s = l[i].getEvent();

            if (!(std::find(result.begin(), result.end(), s ) != result.end()))
            {
              // Añadimos los elementos una única vez
              result.push_back(s);
            }
        }
    }
    return result;
}

Puntos getFeatures(QString user,QString event,vector<Log>l){

    //vector<QString> aux;
    vector<Puntos> result;
    Puntos p;

    for(int i=0;i<l.size();i++){
        QString u = l[i].getUser();
        if(user == u){
            QString e = l[i].getEvent();
            if(e==event) {

//                if (!(std::find(aux.begin(), aux.end(), e ) != aux.end()))
//                {
                  // Añadimos los elementos una única vez

                  float t = l[i].getTimestamp();
                  float x = l[i].getPosX();
                  float y = l[i].getPosY();
                  float z = l[i].getPosZ();

//                  QString sx = QString::number(x);
//                  QString sy = QString::number(y);
//                  QString sz = QString::number(z);

                  //aux.push_back(e);
                  p.user = u;
                  p.name.push_back(e);
                  p.timestamp.push_back(t);
                  p.x.push_back(x);
                  p.y.push_back(y);

                  //control para detectar si el log tiene z
                  if(z!=-1){
                    p.z.push_back(z);
                  }


//              }
            }
        }


    }

    return p;
}
