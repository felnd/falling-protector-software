#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDialog>
#include "addnew.h"
#include "setparameters.h"
#include "test2.h"
#include "test.h"
#include "setenvironment.h"
//#include "accessdb.h"
#include <QtSql/QtSql>
#include <QtSql/QSqlTableModel>
#include <QDebug>
#include <QTableView>
#include <QDir>

namespace Ui {
class MainPage;
}
class MainPage : public QDialog
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();
    void contload();
    QString getBaseData(int n=0);
    void DatasRange();
    void ShowCurdatas();

private slots:

    void on_addnewitem_clicked();

    void on_paramenters_clicked();

    void on_autotest_clicked();

    void on_normal_test_clicked();

    void on_system_clicked();

    void loadaddnew();
    void Slot_addnew();
    void Slot_edit();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_EidtData_clicked();

    void on_delete_2_clicked();

    void on_search_clicked();

    void on_moment_record_clicked();

    void on_upsheet_clicked();

    void on_downsheet_clicked();

private:
    Ui::MainPage *ui;
    AddNew addnew;
    SetParameters paramenters;
    test2 test2page;
    test testpage;
    setEnvironment systempage;
    QStringList *basedatas;
    QStringList *newdatas;
    QSqlDatabase dbconn;
    QTableView *view;
    long DBrows,low,high;
    QStringList strList,strList_2,strList_3;
    int CurIndex;
    Data_captureThread baseThread;
//    AccessDB myDB;


};

#endif // MAINPAGE_H
