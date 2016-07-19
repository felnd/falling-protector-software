#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainpage.h"
#include "test.h"
#include "test2.h"
#include "addnew.h"
#include "setparameters.h"

#define USERNAME "u"
#define PASSWORD "123"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_LoginButton_clicked();
    void inital();
    void on_LogoutButton_clicked();

private:
    Ui::MainWindow *ui;
    MainPage mainpage;
//    AutoTest autotestpage;
    test testpage;
//    PainXY coordinates;
    AddNew addnew;
    test2 test2page;
    SetParameters ParameterPage;
};

#endif // MAINWINDOW_H
