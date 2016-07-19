#ifndef TEST2_H
#define TEST2_H

#include <QWidget>
#include "spcomm.h"
#include "setenvironment.h"
#include "data_capturethread.h"
#include <QMessageBox>
#include <QWheelEvent>


namespace Ui {
class test2;
}

class test2 : public QWidget
{
    Q_OBJECT

public:
    explicit test2(QWidget *parent = 0);
    ~test2();
    void datasLink(QStringList *inputDatas,Data_captureThread *inputThread);
    void wheelEvent(QWheelEvent *event);
signals:
    void closed();
    void autotest();
    void system();
    void parament();

private slots:
    void on_start_clicked();

    void on_stop_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_6_clicked();

    void show_speed(double);

private:
    Ui::test2 *ui;
    QStringList *basedata;
    unsigned char data_value[8];
    unsigned char data_length;
    Data_captureThread *newThread;
private:
    void closeEvent(QCloseEvent *e);
};

#endif // TEST2_H
