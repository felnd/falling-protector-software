#ifndef DATA_CAPTURETHREAD_H
#define DATA_CAPTURETHREAD_H

#include "datamanager.h"

#include <QObject>
#include <QThread>
#include "spcomm.h"
#include "setenvironment.h"
#include  <QtAlgorithms>
#include "WinDef.h"
#include "CH375DLL.H"


class Data_captureThread:public QThread
{
    Q_OBJECT
public:
    explicit Data_captureThread();
    ~Data_captureThread();

    void setCommend(int);
    void setDataStore(DataManager *inputdata,QStringList *inputdatas_2,QStringList *,int flag);
    void setStopFlag();
    void startMechine();
    void stopMechine();
    QVector<double> * getCurrentVDatas();
    QVector<double> * getCurrentTDatas();
    double getMAXV();
    double getMAXT();
    bool getStopFlag();
    void speedUp();
    void setSpeed(uint);

    void counterRotation();

    void resetMachine();

protected:
    virtual void run();

signals:
    void sendMessage();
    void sendFinish();
    void sendSpeed(double p);

private:
    volatile bool bStoppedFlag;
    DataManager *datas;
    QVector<double>  velocitys,torques;
    QStringList *basedata,*newdata;
    double MaxVelocity,MaxTorque;
    SPComm myCome;
    unsigned int speed,mode_flag,cur_setSpeed;
    ulong curV,curT,curVV;
    bool side_flag;

private:
    unsigned int CRCcorrect(uchar *data_value,unsigned char data_length);
    void inital();
    void getSpeed();
};

#endif // DATA_CAPTURETHREAD_H
