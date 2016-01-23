#ifndef PUNTOS
#define PUNTOS

#include <vector>
#include <QString>

using namespace std;

typedef struct {
    QString user;
    vector<QString> name;
    vector<float> timestamp;
    vector<float> x;
    vector<float> y;
    vector<float> z;


} Puntos;

#endif // PUNTOS

