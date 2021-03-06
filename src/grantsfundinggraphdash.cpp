#include "grantsfundinggraphdash.h"



GrantsFundingGraphDash::GrantsFundingGraphDash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GrantsFundingGraphDash)
{
    ui->setupUi(this);

    ui->searchInGrants->setPlaceholderText("Last Name, First Name");
    for(int i = 1975; i <= 2015; i++)
    {
        QString year = QString::number(i);
        ui->fromCBGrants->addItem(year);
        ui->toCBGrants->addItem(year);
    }
    QStringList *list = new QStringList();

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    qry.prepare("SELECT DISTINCT MemberName FROM Grants");
    qry.exec();

    while(qry.next()){
    QString name = QString(qry.record().value(0).toString());
    *list  << name;
    }

    QCompleter* completer = new QCompleter(*list);
    ui->searchInGrants->setCompleter(completer);
}

GrantsFundingGraphDash::~GrantsFundingGraphDash()
{
    delete ui;
}

//draws graph 1
void GrantsFundingGraphDash::make_graph1(int startDate,int endDate)
{

    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QString professor =ui->searchInGrants->text();
    QString strtDate=QString::number(startDate);
    QString edDate=QString::number(endDate);


    /*Populate totals for Number of Attendees*/

    qry.prepare("SELECT SUM(ltrim(TotalAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();

    double result;

    qry.next();
    result = qry.record().value(0).toDouble();
    totalsGrants[0] = result;
    labelsGrants[0] = "Total Amount";

    qry.prepare("SELECT SUM(ltrim(ProratedAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();
    qry.next();
    result = qry.record().value(0).toDouble();
    totalsGrants[1] = result;
    labelsGrants[1] = "Hours";

    /*GET MAX TOTAL FOR Y AXIS*/

    double max=0;
    for(int i=0;i<2; i++){
        if(totalsGrants[i] > max){

            max = totalsGrants[i];
        }

    }

     /*Clear Plots*/
    ui->bar_graphGrants->clearPlottables();

    // create empty bar chart objects:
    QCPBars *postBar = new QCPBars(ui->bar_graphGrants->xAxis, ui->bar_graphGrants->yAxis);

    ui->bar_graphGrants->addPlottable(postBar);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    postBar->setName("Postgraduate Medical Education");
    pen.setColor(QColor(255, 131, 0));
    postBar->setPen(pen);
    postBar->setBrush(QColor(255, 131, 0, 50));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> label;
    ticks << 1 << 2;
    label << labelsGrants[0] << labelsGrants[1];
    ui->bar_graphGrants->xAxis->setAutoTicks(false);
    ui->bar_graphGrants->xAxis->setAutoTickLabels(false);
    ui->bar_graphGrants->xAxis->setTickVector(ticks);
    ui->bar_graphGrants->xAxis->setTickVectorLabels(label);
    ui->bar_graphGrants->xAxis->grid()->setVisible(true);


    // prepare y axis:
    ui->bar_graphGrants->yAxis->setAutoTicks(true);
    ui->bar_graphGrants->yAxis->setAutoTickLabels(true);
    ui->bar_graphGrants->yAxis->setRange(0, max);
    ui->bar_graphGrants->yAxis->setPadding(5); // a bit more space to the left border
    ui->bar_graphGrants->yAxis->setLabel("Total Hours/Students");
    ui->bar_graphGrants->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 50, 25));
    ui->bar_graphGrants->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->bar_graphGrants->yAxis->grid()->setSubGridPen(gridPen);

   QVector<double> graphData;


   graphData << totalsGrants[0] << totalsGrants[1];
   postBar->setData(ticks, graphData);

}

void GrantsFundingGraphDash::on_graphBtn_clicked()
{
    ui->bar_graphGrants->clearPlottables();
    make_graph1(ui->fromCBGrants->currentText().toInt(),ui->toCBGrants->currentText().toInt());
    ui->bar_graphGrants->replot();
    this->setWindowTitle("Pretty Graph");

}

void GrantsFundingGraphDash::on_pieBtn_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QString professor =ui->searchInGrants->text();
    QString strtDate=QString::number(ui->fromCBGrants->currentText().toInt());
    QString edDate=QString::number(ui->toCBGrants->currentText().toInt());


    /*Populate totals for Number of Attendees*/

    qry.prepare("SELECT SUM(ltrim(TotalAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();

    double result;


    if(qry.next()){
    result = qry.record().value(0).toInt();
    }else{
        result = 0;
    }
    totalsGrantsPie[0] = result;
    labelsGrantsPie[0] = "Total Amount";

    qry.prepare("SELECT SUM(ltrim(ProratedAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();
    if(qry.next()){
    result = qry.record().value(0).toInt();
    }else{
        result = 0;
    }

    totalsGrantsPie[1] = qry.record().value(0).toDouble();
    labelsGrantsPie[1] = "Hours";


    QDialog *pieWindow = new PieChart(labelsGrantsPie, totalsGrantsPie, 2, 2, this);
    pieWindow->showMaximized();
}


void GrantsFundingGraphDash::printBarButton(){
    QPrinter printer;
    QPainter painter;
    QPrintDialog *dialog = new QPrintDialog(&printer);

    dialog->setWindowTitle("Print Bar Chart");
    if (dialog->exec() != QDialog::Accepted)
        return;
    painter.begin(&printer);
    painter.scale(0.5,0.5);
    ui->bar_graphGrants->render(&painter);
    painter.end();


}






void GrantsFundingGraphDash::printPieButton(){
    QSqlDatabase db = QSqlDatabase::database("db_connection");
    QSqlQuery qry(db);

    QString professor =ui->searchInGrants->text();
    QString strtDate=QString::number(ui->fromCBGrants->currentText().toInt());
    QString edDate=QString::number(ui->toCBGrants->currentText().toInt());


    /*Populate totals for Number of Attendees*/

    qry.prepare("SELECT SUM(ltrim(TotalAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();

    double result;
    qDebug()<<qry.lastQuery();
    qDebug()<<qry.record();


    if(qry.next()){
    result = qry.record().value(0).toInt();
    }else{
        result = 0;
    }
    totalsGrantsPie[0] = result;
    labelsGrantsPie[0] = "Total Amount";

    qry.prepare("SELECT SUM(ltrim(ProratedAmount, '$')) FROM Grants WHERE MemberName LIKE '"+professor+"%' AND StartDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' AND EndDate BETWEEN '"+strtDate+"%' AND '"+edDate+"%' ");
    qry.exec();
    if(qry.next()){
    result = qry.record().value(0).toInt();
    }else{
        result = 0;
    }

    totalsGrantsPie[1] = qry.record().value(0).toDouble();
    labelsGrantsPie[1] = "Hours";


    QDialog *pieWindow = new PieChart(labelsGrantsPie, totalsGrantsPie, 2, 2, this);



    QPrinter printer;
    QPainter painter;
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Pie Chart");
    if (dialog->exec() != QDialog::Accepted)
        return;
    painter.begin(&printer);
    painter.scale(0.5,0.5);
    pieWindow->render(&painter);
    painter.end();

}

void GrantsFundingGraphDash::on_pushButton_clicked()
{
    printBarButton();
}

void GrantsFundingGraphDash::on_pushButton_2_clicked()
{
    printPieButton();
}
