#include "year.h"
#include "entry.h"
#include <QString>
#include <QVector>

Year::Year(){}

Year::Year(int d, double h, int s){
    date=d;
    tothours=h;
    totstudents=s;

}



