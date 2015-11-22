#ifndef TEACHINGTABLE_H
#define TEACHINGTABLE_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTableView>
#include <QSqlTableModel>
class QDialogButtonBox;
class QPushButton;
class QSqlTableModel;


namespace Ui {
class TeachingTable;
}

class TeachingTable : public QDialog
{
    Q_OBJECT

public:
    //explicit TeachingTable(QWidget *parent = 0);
    explicit TeachingTable(const QString &tableName,const QString &deleteAction,const QString &filter,QWidget *parent = 0);
    ~TeachingTable();

private slots:
   void submit();
   //void remove();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    //QPushButton *deleteButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;


private:
    Ui::TeachingTable *ui;

};

#endif // TEACHINGTABLE_H