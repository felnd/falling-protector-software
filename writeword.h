#ifndef WRITEWORD_H
#define WRITEWORD_H

#include <QObject>
#include "datamanager.h"
#include <QAxWidget>
#include <QAxObject>
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include "qcustomplot.h"

class WriteWord : public QObject
{
    Q_OBJECT
public:
    explicit WriteWord(QObject *parent = 0);
    ~WriteWord();
    void loaddata(DataManager *inputGraphData, QStringList *inputBaseData, QStringList *inputNewDatas);
    bool wordWriting();
    void GenerData();

private:
    DataManager *GraphData;
    QStringList *baseData;
    QStringList *newData;
    QStringList writeData,writeData2;
    QAxWidget *word;

signals:

public slots:
};

#endif // WRITEWORD_H
