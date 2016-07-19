#ifndef SETPARAMETERS_H
#define SETPARAMETERS_H

#include <QWidget>
#include <QFile>
#include <qstring.h>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#define ITEMS 26

namespace Ui {
class SetParameters;
}

class SetParameters : public QWidget
{
    Q_OBJECT

public:
    explicit SetParameters(QWidget *parent = 0);
    ~SetParameters();
    QStringList* getDatas();
    bool loaddata();
    bool setDatas();
    bool saveDatas();

private slots:

    void on_saveData_clicked();
    void on_cancel_clicked();

    void on_open_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

signals:
    void closed();
    void tiaosu();
    void autotest();
    void system();

private:
    Ui::SetParameters *ui;
    QFile *file;
    QStringList datas;
private:
    void closeEvent(QCloseEvent *e);
    bool checkDigtal(QString str);
};

#endif // SETPARAMETERS_H
