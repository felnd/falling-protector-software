#ifndef ADDNEW_H
#define ADDNEW_H

#include <QDialog>
#include <QMessageBox>
#include "datetime.h"
#include "QStringListModel"
#include <QtSql/QtSql>
#include <QtSql/QSqlTableModel>

namespace Ui {
class AddNew;
}

class AddNew : public QDialog
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = 0);
    ~AddNew();
    QStringList* getData();
    void loaddata(QStringList* inputDatas, QSqlDatabase *inputDB);
    void eidtItems(QStringList &eidtdata);
signals:
    void closed();
    void addsignal();
    void editsignal();
private slots:
    void on_correct_clicked();
    void on_datechoose_clicked();
    void getTime(QDateTime nowdays);
    void on_cancel_clicked();

private:
    Ui::AddNew *ui;
    QStringList datas;
    QStringList *baseData;
    DateTime dateTime;
    QSqlDatabase *DBpinter;
    int stateFlag;

private:
    void closeEvent(QCloseEvent *e);


};

#endif // ADDNEW_H
