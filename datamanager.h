#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QVector>


typedef struct DataNode
{
    QVector<double> V_datas;
    QVector<double> T_datas;
    double max_torque;
    double action_velocity;
    double error_rate;
    bool  select_state;
    double br_length;
    DataNode *next;
}DataNode;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = 0);
    ~DataManager();
    bool InitDataManager();
    bool PustData(DataNode *newDataList=NULL);
    const DataNode* getData(int n);
    bool DeleteData(int n);
    bool setState(bool flag, int n);
    bool getState(int n);
    int  getLength();
    void setLength(int inLength);
    bool DestroyData();
    bool setSelectNumber(int n);
    int  getSelectNumber();
    void setaverror(double);
    double getaverror();
    void setBrlength(double, int n);
    void setSelector(int *,int);
    int getSelector(int);
signals:

public slots:

private:
    DataNode *DataList;
    int length;
    int selectNumber;
    double averror;
    QVector<int> selectors;

};

#endif // DATAMANAGER_H
