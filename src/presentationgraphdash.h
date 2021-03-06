#ifndef PRESENTATIONGRAPHDASH_H
#define PRESENTATIONGRAPHDASH_H

#include <QDialog>
#include "ui_presentationgraphdash.h"
#include <summary.h>
#include <qcustomplot.h>
#include <db.h>
#include <QCoreApplication>
#include <QVector>
#include <QString>
#include <QtSql/QtSql>
#include <QDebug>
#include <iostream>
#include <algorithm>
#include <piechart.h>
#include <QCompleter>
#include <QStringList>

namespace Ui {
class PresentationGraphDash;
}

class PresentationGraphDash : public QDialog
{
    Q_OBJECT

public:
    explicit PresentationGraphDash(QWidget *parent = 0);
    ~PresentationGraphDash();
private slots:
     void make_graph1(int startDate, int endDate);

     void on_barBtn_clicked();

     void on_pieBtn_clicked();

     void printPieButton();
     void printBarButton();

     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

private:
    Ui::PresentationGraphDash *ui;
    double totalsPres[2];
    QString labelsPres[2];
    QDialog *pieWindow;

};

#endif // PRESENTATIONGRAPHDASH_H
