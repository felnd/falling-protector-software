#ifndef SELECTIONS_H
#define SELECTIONS_H

#include <QWidget>
#include <QCheckBox>
#include <vector>
#include <QCheckBox>
#include <QtAlgorithms>
#include <QHBoxLayout>
#include "datamanager.h"
#define MAXSLECTIONS 3

namespace Ui {
class Selections;
}

class Selections : public QWidget
{
    Q_OBJECT

public:
    explicit Selections(QWidget *parent = 0);
    ~Selections();
    bool setTable(int row=3);
    bool addDatas(DataManager *inputdata,QString velocity);
    bool loaddata(DataManager *inputdatas);

private slots:
    void on_pushButton_2_clicked();
    void setvalue(int);
    void testSlot(int row,int colomn);

    void on_OK_clicked();

private:
    Ui::Selections *ui;
    int rows;
    QString Bspeed;
    double average;
    int selects;
    int selectors[100];
    QCheckBox *CheckBox[100];
    DataManager *data;

};

#endif // SELECTIONS_H
