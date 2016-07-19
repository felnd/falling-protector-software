#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "selections.h"
#include "datamanager.h"
#include "data_capturethread.h"
#include <QTime>
#include <QFile>
#include "setenvironment.h"         //#define CURRENTNO 13 引用，频率输入
#include <QAxWidget>
#include <QAxObject>
#include "writeword.h"
#include <QMessageBox>
#include "spcomm.h"

//#include "wordmakethread.h"

namespace Ui {
class test;
}

class test : public QDialog
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = 0);
    ~test();
    bool setLoadData(QStringList *inputData, QStringList *inputNewData, Data_captureThread *upThread);


private slots:

    void on_pushButton_clicked();

    void addItems();
    void on_run_clicked();

    void on_stop_clicked();
    void drawLines();

    void on_Times_currentIndexChanged(int index);

    void on_save_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_decrement_textChanged();

    void on_savaThisDatas_clicked();

    void on_open_clicked();

    void on_recovery_clicked();

signals:
    void closed();
    void tiaosu();
    void system();
    void parament();
private:
    Ui::test *ui;
    Selections table;
    DataManager datas;
    Data_captureThread *dataCapture;
    QVector<double> *currentVDatas,*currentTDatas;
    int Times_2;
    QStringList *baseData;
    QStringList *newData;
    bool saveFlag;
    QFile *file;
    WriteWord wordMaker;
    SPComm myCome;
//    WordMakeThread wordThread;
private:
    void closeEvent(QCloseEvent *e);
    unsigned CRCcorrect(uchar *data_value,unsigned char data_length);


};

#endif // TEST_H
