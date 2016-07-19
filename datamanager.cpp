#include "datamanager.h"

DataManager::DataManager(QObject *parent) : QObject(parent),
    length(0),DataList(NULL),selectNumber(0)
{

}

DataManager::~DataManager()
{
    if(DataList!=NULL)
    {
        DataNode *p = DataList,*q;
        while(p)
        {
            q = p;
            p = p->next;
            free(q);
        }
        DataList = NULL;
    }
    length = 0;
}

//初始化数据管理类
bool DataManager::InitDataManager()
{
    DataList = (DataNode*)malloc(sizeof(DataNode));
    if(!DataList)return false;
    DataList->action_velocity = -1;
    DataList->error_rate = -1;
    DataList->max_torque = -1;
    DataList->select_state = false;
    DataList->br_length = -1;
    DataList->next = NULL;
    return true;
}

//插入数据
bool DataManager::PustData(DataNode *newDataList)
{
    if(!newDataList)return false;
    DataNode *p=DataList;
    while(p->next)
    {
        p = p->next;
    }
    newDataList->next = p->next;
    p->next = newDataList;
    length++;
    return true;
}

//获取数据从1开始
const DataNode* DataManager::getData(int n)
{
    if(n<1||n>length||!DataList)return NULL;

    const DataNode *p = DataList;
    int i=0;
    while(i<n)
    {
        p = p->next;
        i++;
    }
    return p;

}

//插入第n个数据的碟簧压缩量
void DataManager::setBrlength(double input_length,int n)
{
    if(n<1||n>length||!DataList)return;

    DataNode *p = DataList;
    int i=0;
    while(i<n)
    {
        p = p->next;
        i++;
    }
    p->br_length = input_length;
}

//清除某个数据
bool DataManager::DeleteData(int n)
{
    if(n<=0||n>length||!DataList)return false;
    DataNode *p = DataList,*q;
    int i=0;
    while(i<n-1)
    {
        p = p->next;
        i++;
    }
    q = p->next;
    p->next = q->next;
    free(q);
    length--;
    return true;
}


//设置数据选择状态，true为已选，false为未选
bool DataManager::setState(bool flag,int n)
{
    if(n<=0||n>length||!DataList)return false;
    int i=0;
    DataNode *p = DataList;
    while(i<n)
    {
        i++;
        p = p->next;
    }
    p->select_state = flag;
    return true;
}

//获取数据组数量
int DataManager::getLength()
{
    return this->length;
}

//清除所有数据除表头
bool DataManager::DestroyData()
{
    if(DataList!=NULL)
    {
        DataNode *p = DataList->next,*q;
        while(p)
        {
            q = p;
            p = p->next;
            free(q);
        }
        DataList->next=NULL;
    }
    length = 0;
    return true;
}

//设置已选的数据组数量
bool DataManager::setSelectNumber(int n)
{
    this->selectNumber = n;
    return true;
}

//读取已选的数据组数量
int  DataManager::getSelectNumber()
{
    return this->selectNumber;
}

//读取数据组选择状态，true为已选，false为未选
 bool DataManager::getState(int n)
 {
     if(n<=0||n>length||!DataList)return false;
     int i=0;
     DataNode *p = DataList;
     while(i<n)
     {
         i++;
         p = p->next;
     }
     return p->select_state;
 }

//设置平均误差
void DataManager::setaverror(double inputerror)
{
    averror = inputerror;
}

//设置已选数据组的下标，例如selectors = [1,3,4],表示下标为1,3,4的数据是已选
 void DataManager::setSelector(int *arr,int n)
 {
     selectors.clear();
     for(int i=0;i<n;i++)
     {
         selectors<<arr[i];
     }
 }

 //获取selector下标为n的数据
 int DataManager::getSelector(int n)
 {
     return selectors[n];
 }

 //读取平均误差
  double DataManager::getaverror()
  {
      return averror;
  }

  //设置数据组总数
  void DataManager::setLength(int inLength)
  {
      length = inLength;
  }
