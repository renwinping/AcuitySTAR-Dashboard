#include "teachingtable.h"
#include "ui_teachingtable.h"

#include <QtWidgets>
#include <QtSql>


TeachingTable::TeachingTable(const QString &tableName,const QString &deleteAction,const QString &filter, QWidget *parent) :
//TeachingTable::TeachingTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeachingTable)
{
    //ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath() + QDir::separator() + "database.sqlite");  //without all the QDir stuff, will only look in current working directory
    db.open();

    //QSqlQueryModel *model = new QSqlQueryModel;
    model = new QSqlTableModel(this);
    //model->setQuery("SELECT * from teaching");
    //model->setTable("teaching");
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    if (deleteAction == "RemoveError"){
        QSqlQuery qry(db);
        qry.prepare("SELECT rowid FROM "+tableName+ " WHERE " + filter);
        qry.exec();
        while(qry.next()){
            model->database().transaction();
            if (model->removeRow(qry.value(0).toInt()-3)) {
                model->database().commit();
            } else {
                model->database().rollback();
                QMessageBox::warning(this, tr("Table"),
                                     tr("The database reported an error: %1")
                                     .arg(model->lastError().text()));
            }
        }

    }
    if (filter  != ""){
           model->setFilter(filter);
    }

   // model->setFilter("(MemberName = '') OR (PrimaryDomain = '') OR (StartDate = '') OR (EndDate ='')");
    QTableView *view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();
    //view->show();
    //db.close();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));

    //deleteButton = new QPushButton(tr("&Delete"));
    quitButton = new QPushButton(tr("Quit"));
    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);

    //buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    //connect(deleteButton, SIGNAL(clicked()), this, SLOT(remove()));
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Teaching Table"));

}

void TeachingTable::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Teaching Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}
/**
void TeachingTable::remove()
{
    model->setFilter("(MemberName = '') OR (PrimaryDomain = '') OR (StartDate = '') OR (EndDate ='')");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath() + QDir::separator() + "database.sqlite");  //without all the QDir stuff, will only look in current working directory
    db.open();
    QSqlQuery qry(db);
    qry.prepare("SELECT rowid FROM Teaching WHERE (MemberName = '') OR (PrimaryDomain = '') OR (StartDate = '') OR (EndDate ='')");
    qry.exec();
    while(qry.next()){
      //model->selectRow(qry.value(0).toInt()-3);
     //model->selectRow(qry.value(0).toInt()-3);
     model->removeRow(qry.value(0).toInt()-3);
    }
    model->database().commit();
}
**/
TeachingTable::~TeachingTable()
{
    delete ui;
}
