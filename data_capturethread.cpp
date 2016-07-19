#include "data_capturethread.h"


Data_captureThread::Data_captureThread():bStoppedFlag(true),
    MaxVelocity(0),MaxTorque(0)
{
    //myCome = new SPComm();
}

Data_captureThread::~Data_captureThread()
{
    //delete myCome;
    myCome.close();
}
//-------------速度读取--------------
void Data_captureThread::getSpeed()
{
   char buffer[36];
   ulong mLength=36;
   ulong IOlength = 49;
   //== INVALID_HANDLE_VALUE;
   uchar writebuff[49]={0};
   writebuff[48] = 0xAA;
   CH375WriteData(0,writebuff,&IOlength);

   int n=CH375ReadData( 0, buffer, &mLength );
   //qDebug()<<"n=:"<<n;
   //CH375CloseDevice( 0 );
   curV = ((uchar)buffer[28]*256*256*256+(uchar)buffer[29]*256*256+(uchar)buffer[30]*256+(uchar)buffer[31])*20;
   //curV = (ulong)((uchar)buffer[2]*256+(uchar)buffer[3])*500/4095;
   //qDebug()<<buffer[2]<<"\t"<<buffer[3];//<<"\t"<<buffer[29]<<"\t"<<buffer[30];
   qDebug()<<"curV"<<curV;
   curT = ((uchar)buffer[32]*256*256*256+(uchar)buffer[33]*256*256+(uchar)buffer[34]*256+(uchar)buffer[35])*20;

   qDebug()<<"curT"<<curT;
   curVV =(ulong)((uchar)buffer[0]*256+(uchar)buffer[1])*10000/4095;
   qDebug()<<"curVV"<<curVV;
}

//速度转矩保存
void Data_captureThread::run()
{
    int i = 0;
    double p;
    bStoppedFlag = false;

    velocitys.clear();
    torques.clear();

    MaxVelocity = 0;
    MaxTorque = 0;
    double Bspeed=1;
    if(mode_flag==1)Bspeed = (*newdata)[12].toDouble()*60000/(3.14*(*newdata)[15].toDouble()*(*newdata)[14].toDouble());
    this->msleep(1500);
    int k=0;
    while(k!=50)
    {
        getSpeed();

        //p = (curT-10000.0)*2;//-2000.0;
        p = curVV;
        if(p<0)p=0;
        torques<<p;
        if(MaxTorque<p)MaxTorque = p;

        p = (double)curV/2700*60;
        //p = (double)curV;
        velocitys<<p;
        if(MaxVelocity<p)MaxVelocity = p;

        i++;

        if(i%10==0)
        {
            if(mode_flag==1)
                emit sendMessage();
            else
            {
                emit sendSpeed(p);
            }
        }
        qDebug()<<cur_setSpeed;
        //if(p >= cur_setSpeed-5)          //大于当前设定速度-5r/min
        //if(i%20==0)
           //speedUp();
        this->msleep(100);
        if(bStoppedFlag)
        {
            k++;
            qDebug()<<k;
        }

    }
    //stopMechine();
    qDebug()<<"stop Device";
    CH375CloseDevice( 0 );

    if(mode_flag==1)
    {
        DataNode *currentDatas = new DataNode();
        currentDatas->V_datas = velocitys;
        currentDatas->T_datas = torques;
        currentDatas->max_torque = MaxTorque;
        currentDatas->action_velocity = MaxVelocity;

        currentDatas->error_rate = qAbs(MaxVelocity/Bspeed-1)*100;

        currentDatas->select_state = false;
        currentDatas->next = NULL;
        datas->PustData(currentDatas);

        emit sendFinish();
    }

}

//------------数据链接--------------
void Data_captureThread::setDataStore(DataManager *inputdata,QStringList *inputdatas_2,QStringList *inputNew,int flag)
{
    datas = inputdata;
    newdata = inputNew;
    basedata = inputdatas_2;
    side_flag = ((*basedata)[CURRENTNO+9].toInt()>0)?false:true;       //false反转true正转
    this->inital();
    mode_flag = flag;
}

//------------串口初始化--------------
void Data_captureThread::inital()
{
    QString str;
    switch((*basedata)[CURRENTNO+1].toInt())
    {
        case '0': str = QString("COM1");break;
        case '1': str = QString("COM2");break;
        case '2': str = QString("COM3");break;
        case '3': str = QString("COM4");break;
        case '4': str = QString("COM5");break;
        case '5': str = QString("COM6");break;
        case '6': str = QString("COM7");break;
        case '7': str = QString("COM8");break;
        case '8': str = QString("COM9");break;
        default: str = QString("COM1");break;
    }
    qDebug()<<str;
    myCome.setPortName(str);
    myCome.setBaudRate(19200);

    //char buffer[49];
    //ulong mTotal = 49;
    //== INVALID_HANDLE_VALUE;
    //CH375WriteData( 0, buffer, &mTotal );
    //CH375CloseDevice( 0 );
}
//-----------设置初始转速------------
void Data_captureThread::setSpeed(uint setSpeed)
{
    speed = (uint)setSpeed*19.1923;
    myCome.open();

    char buffer[8];
    uchar TxData[8] = {0x01,0x06,0x20,0x00,0x00,0x00};

    TxData[5]=(uchar) (0x000000ff & speed);
    TxData[4]=(uchar) ((0x0000ff00 & speed)>>8);

    unsigned int CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);
    qDebug()<<"CRC:  "<<CRCcode;

    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);

    this->msleep(10);
    myCome.writeData(TxData,8);
    myCome.readData(buffer,8);
    this->msleep(10);
    myCome.close();
}

//-----------转速增加------------
void Data_captureThread::speedUp()
{
    if(cur_setSpeed == speed)return;
    qDebug()<<"speed:  "<<speed;
    if(cur_setSpeed<speed) cur_setSpeed += (uint)10*19.1923;             //速度增量步长
    if(cur_setSpeed>speed)
    {
        cur_setSpeed = speed;
    }
    myCome.open();

    char buffer[8];
    uchar TxData[8] = {0x01,0x06,0x20,0x00,0x00,0x00};

    TxData[5]=(uchar) (0x000000ff & cur_setSpeed);
    TxData[4]=(uchar) ((0x0000ff00 & cur_setSpeed)>>8);

    unsigned int CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);
    qDebug()<<"CRC:  "<<CRCcode;

    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);

    this->msleep(10);
    myCome.writeData(TxData,8);
    myCome.readData(buffer,8);
    this->msleep(10);
    myCome.close();
}

//-----------当前速度数据链接------------
QVector<double> * Data_captureThread::getCurrentVDatas()
{
    return &this->velocitys;
}

//-----------当前转矩数据链接------------
QVector<double> * Data_captureThread::getCurrentTDatas()
{
    return &this->torques;
}

//-----------停止动作-------------
void Data_captureThread::setStopFlag()
{
    bStoppedFlag = true;
    qDebug()<<bStoppedFlag;
    //CH375CloseDevice( 0 );
}

//-----------动作速度-------------
double Data_captureThread::getMAXV()
{
    return MaxVelocity;
}

//-----------最大制动转矩----------
double Data_captureThread::getMAXT()
{
    return MaxTorque;
}

//-----------读取停止标志----------
bool Data_captureThread::getStopFlag()
{
    return bStoppedFlag;
}

//-----------电机启动-----------
void Data_captureThread::startMechine()
{
    /*
    QString str;
    switch((*basedata)[CURRENTNO+1].toInt())
    {
        case '0': str = QString("COM1");break;
        case '1': str = QString("COM2");break;
        case '2': str = QString("COM3");break;
        case '3': str = QString("COM4");break;
        default: str = QString("COM1");break;
    }
    qDebug()<<str;
    myCome.setPortName(str);
    myCome.setBaudRate(19200);
*/
    myCome.open();
    char buffer[8];

    qDebug()<<"start";
    uchar TxData[8] = {0x01,0x06,0x20,0x00,0x00,0x00};

    //cur_setSpeed = 70*19.1923;                                 //初始速度
    cur_setSpeed = speed;
    TxData[5]=(uchar) (0x000000ff & cur_setSpeed);
    TxData[4]=(uchar) ((0x0000ff00 & cur_setSpeed)>>8);

    unsigned int CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);
    qDebug()<<"CRC:  "<<CRCcode;
    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);

    this->msleep(10);

    myCome.writeData(TxData,8);

    myCome.readData(buffer,8);
    qDebug()<<"buffer"<<buffer;
    this->msleep(10);
    qDebug()<<"side_flag"<<side_flag;
    if(side_flag)
    //if(1)
    {
        TxData[2]=0x10;
        TxData[3]=0x00;
        TxData[4]=0x00;
        TxData[5]=0x01;
    }
    else
    {
        TxData[2]=0x10;
        TxData[3]=0x00;
        TxData[4]=0x00;
        TxData[5]=0x02;
    }

    CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);
    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);
    qDebug()<<"CRCode:"<<CRCcode;
    this->msleep(10);
    myCome.writeData(TxData,8);
    myCome.readData(buffer,8);
    this->msleep(10);

    CH375OpenDevice( 0 ) ;
    uchar writebuff[49]={0};
    ulong IOleng=49;
    writebuff[46] = 2;
    writebuff[47] = 2;
    writebuff[48] = 85;
    CH375WriteData(0,writebuff,&IOleng);

    myCome.close();
//    qDebug()<<str;
}

//---------------电机停止------------------
void Data_captureThread::stopMechine()
{
    qDebug()<<"stop--";
    myCome.open();
    char buffer[8];
    uchar TxData[8] = {0x01,0x06,0x10,0x00,0x00,0x06};
    unsigned int CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);

    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);

    this->msleep(10);
    myCome.writeData(TxData,8);
    myCome.readData(buffer,8);
    this->msleep(10);
    myCome.close();
    //CH375CloseDevice( 0 );

}
//--------------CRC校验----------------
unsigned int Data_captureThread::CRCcorrect(uchar *data_value,unsigned char data_length)
{
    int i;
    unsigned int crc_value = 0xffff;
    while(data_length--)
    {
        crc_value ^=*data_value++;
        for(i=0;i<8;i++)
        {
            if(crc_value&0x0001)
                crc_value = (crc_value>>1)^0xa001;
            else
                crc_value = crc_value>>1;
        }
    }
    return(crc_value);
}

//--------------反转设置---------------
void Data_captureThread::counterRotation()
{
    side_flag = !side_flag;
}

//--------------复位---------------
void Data_captureThread::resetMachine()
{
    side_flag = !side_flag;
    speed = (*basedata)[CURRENTNO+11].toUInt()*19.1923;
    startMechine();
    this->msleep(1000);
    stopMechine();
    side_flag = !side_flag;
}
