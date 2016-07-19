#include "test.h"
#include "ui_test.h"
//#include "painxy.h"
#include "qpainter.h"
#include "qcustomplot.h"
#include "selections.h"

test::test(QWidget *parent):
    QDialog(parent),
    ui(new Ui::test),
    currentVDatas(NULL),
    currentTDatas(NULL),
    Times_2(0),saveFlag(true)
{
    ui->setupUi(this);

    ui->widget->addGraph();
    ui->widget->addGraph(ui->widget->xAxis, ui->widget->yAxis2);
    ui->widget->xAxis->setRange(0,12);
//    ui->widget->xAxis2->setRange(0,120);
    ui->widget->yAxis->setRange(0,300);
    ui->widget->yAxis2->setRange(0,5000);
    ui->widget->graph(1)->setPen(QPen(Qt::red));
    ui->widget->xAxis2->setVisible(true);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->yAxis2->setTickLabels(true);
    ui->widget->graph(1)->rescaleAxes();
//    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
//    ui->widget->graph(0)->rescaleAxes();
//    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->widget->xAxis->setLabel("时间(s)");
    ui->widget->yAxis2->setLabel("扭矩(N·m)");
    ui->widget->yAxis->setLabel("转速(r/min)");
//    ui->widget->xAxis->setRange(-11,11);
//    ui->widget->yAxis->setRange(-1100,1100);
//    ui->widget->xAxis->setRange(30,120);
//    on_xFrom_textChanged();
    if(!datas.InitDataManager())this->close();
    ui->Times->addItem(QString(""));
//    ui->Times->addItem(QString("第%1次").arg(datas.getLength()));
    QStringList strList;
//    strList<<"100%载荷"<<"112%载荷";
//    ui->load->addItems(strList);
    ui->velocity->setText("0");
    ui->Times->setEnabled(true);
//    ui->load->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->run->setEnabled(true);
    ui->save->setEnabled(true);
    ui->open->setEnabled(true);
    ui->recovery->setEnabled(true);

    file = new QFile("./basedata.txt");

}

test::~test()
{
//    dataCapture->exit();
    delete ui;
}

//次别选择按钮
void test::on_pushButton_clicked()
{
    table.setTable(datas.getLength());
//    this->hide();
    double Bspeed = (*newData)[12].toDouble()*60000/(3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble());
    table.addDatas(&datas,QString("%1").arg(Bspeed));
    table.setWindowModality(Qt::ApplicationModal);
    table.show();
}

//关闭
void test::closeEvent(QCloseEvent *e)
{
    //电机停止检测
    if(!dataCapture->getStopFlag())
    {
        QMessageBox::warning(this,tr("Warning"),tr("电机还未停止，请先关闭电机！"),QMessageBox::Yes);
        e->ignore();
    }
    else
    {
        if(!saveFlag)
        {
            /*
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, tr("Warning"),tr("数据还未保存，是否保存？"),
                                                QMessageBox::Yes | QMessageBox::No );
            if (reply == QMessageBox::Yes)
            {
                saveFlag = true;
                //增加数据保存函数;
                e->ignore();
            }

            else
            {
                datas.DestroyData();
                ui->widget->graph(0)->clearData();
                ui->widget->graph(1)->clearData();
                ui->widget->replot();

                for(int i=0,n=ui->Times->count();i<n;i++)
                {
                    ui->Times->removeItem(1);
                }
                disconnect(dataCapture,SIGNAL(sendMessage()),this,SLOT(drawLines()));
                disconnect(dataCapture,SIGNAL(sendFinish()),this,SLOT(addItems()));
                emit closed();
            }
            */

            on_savaThisDatas_clicked();
            datas.DestroyData();
            ui->widget->graph(0)->clearData();
            ui->widget->graph(1)->clearData();
            ui->widget->replot();

            for(int i=0,n=ui->Times->count();i<n;i++)
            {
                ui->Times->removeItem(1);
            }
            disconnect(dataCapture,SIGNAL(sendMessage()),this,SLOT(drawLines()));
            disconnect(dataCapture,SIGNAL(sendFinish()),this,SLOT(addItems()));
            emit closed();
        }
        else
        {
            datas.DestroyData();
            ui->widget->graph(0)->clearData();
            ui->widget->graph(1)->clearData();
            ui->widget->replot();

            for(int i=0,n=ui->Times->count();i<n;i++)
            {
                ui->Times->removeItem(1);
            }
            disconnect(dataCapture,SIGNAL(sendMessage()),this,SLOT(drawLines()));
            disconnect(dataCapture,SIGNAL(sendFinish()),this,SLOT(addItems()));
            emit closed();
        }
    }
}

//启动按钮
void test::on_run_clicked()
{
    Times_2=0;
//    ui->Times->setCurrentText(QString("第%1次").arg(datas.getLength()+1));
    ui->Times->setEnabled(false);
//    ui->load->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->run->setEnabled(false);
    ui->save->setEnabled(false);
    ui->open->setEnabled(false);
    ui->recovery->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    saveFlag = false;
    dataCapture->setSpeed(170);                      //设置最高转速
    dataCapture->startMechine();                       //电机启动
    dataCapture->start();                              //读数据线程启动

    //ui->widget->clear;                      //清除图像数据
    ui->widget->graph(0)->clearData();
    ui->widget->graph(1)->clearData();
}

//绘图
void test::drawLines()
{
    QVector<double> x(10),y(10),y1(10);
    for(int i=0;i<10;i++)
    {
        x[i]=(i+10*Times_2)*(*baseData)[15].toDouble();
        y[i] =(*currentVDatas)[i+10*Times_2];
        y1[i] =(*currentTDatas)[i+10*Times_2];
    }
    ui->widget->graph(0)->addData(x,y);
    ui->widget->graph(1)->addData(x,y1);
//    ui->widget->update();

//    QVector<double> xx(50),y(50),y1(50);
//    for(int i=0;i<50;i++)
//    {
//        xx[i] = i+50*Times;
//        y[i] = i*3.3;
//        y[i] = i*2.2;
//    }
//    ui->widget->graph(0)->addData(xx,y);
//    ui->widget->graph(1)->addData(xx,y1);
    ui->widget->graph(1)->rescaleKeyAxis();
    //double scaley = dataCapture->getMAXV()*1.17;
    //double scaley1 = dataCapture->getMAXT()*1.1;
    //ui->widget->yAxis->setRange(0,scaley);
    //ui->widget->yAxis2->setRange(0,scaley1);
    ui->widget->replot();
    Times_2++;
}

//数据链接
bool test::setLoadData(QStringList *inputData,QStringList *inputNewData,Data_captureThread *upThread)
{
    baseData = inputData;
    newData = inputNewData;
    dataCapture = upThread;
    if((*newData).size()<19)
    {
        QMessageBox::warning(this,tr("Warning"),tr("样本未选择\n请选择样本？"),QMessageBox::Yes);
        return false;
    }
    connect(dataCapture,SIGNAL(sendMessage()),this,SLOT(drawLines()));
    connect(dataCapture,SIGNAL(sendFinish()),this,SLOT(addItems()));

    dataCapture->setStopFlag();

    dataCapture->setDataStore(&datas,baseData,newData,1);

    currentVDatas = dataCapture->getCurrentVDatas();
    currentTDatas = dataCapture->getCurrentTDatas();
    return true;
}

//停止按钮
void test::on_stop_clicked()
{
    ui->Times->setEnabled(true);
//    ui->load->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->run->setEnabled(true);
    ui->save->setEnabled(true);
    ui->open->setEnabled(true);
    ui->recovery->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    qDebug()<<"stop_1";
    dataCapture->setStopFlag();
    qDebug()<<"stop_2";
    dataCapture->stopMechine();

    QString str;
    switch((*baseData)[CURRENTNO+1].toInt())
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
    myCome.setPortName(str);
    myCome.setBaudRate(19200);
    myCome.open();
    char buffer[8];
    uchar TxData[8] = {0x01,0x06,0x10,0x00,0x00,0x06};
    unsigned int CRCcode = CRCcorrect((uchar*)TxData,(unsigned char)6);

    TxData[6]=(uchar) (0x000000ff & CRCcode);
    TxData[7]=(uchar) ((0x0000ff00 & CRCcode)>>8);

    //this->msleep(10);
    myCome.writeData(TxData,8);
    myCome.readData(buffer,8);
    myCome.close();
    //this->msleep(10);
    qDebug()<<"stop_3";

}

//增加测量次别
void test::addItems()
{
    ui->Times->addItem(QString("第%1次").arg(datas.getLength()));
    ui->Times->setCurrentIndex(datas.getLength());
    double torque = datas.getData(datas.getLength())->max_torque;
    double speed = (*newData)[11].toDouble();   //Vdm
    double phy = 1+speed*speed/2.94;            //1+Vdm^2/(2gL1)
    //phy*mn*z*F/2
    double Bniuju = phy*(*newData)[15].toDouble()*(*newData)[14].toDouble()*(*newData)[10].toDouble()*0.5;
    qDebug()<<"Bniuju"<<Bniuju;
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::green);
    if(torque >= Bniuju)pe.setColor(QPalette::WindowText,Qt::green);
    else pe.setColor(QPalette::WindowText,Qt::red);
    ui->torque->setText(QString("%1").arg(torque,0,'g',4));
    ui->torque->setPalette(pe);
    double velocity = datas.getData(datas.getLength())->action_velocity;
    double error_v = datas.getData(datas.getLength())->error_rate;
    qDebug()<<"error_v"<<error_v;
    QPalette pe2;
    pe2.setColor(QPalette::WindowText,Qt::green);
    if(error_v <= 7)pe2.setColor(QPalette::WindowText,Qt::green);
    else pe2.setColor(QPalette::WindowText,Qt::red);
    ui->velocity->setText(QString("%1").arg(velocity,0,'g',4));
    ui->velocity->setPalette(pe2);
}

//测量次别变动
void test::on_Times_currentIndexChanged(int index)
{
    const DataNode *p;
    p = datas.getData(index);
    if(!p)return;
    QVector<double> x;
    for(int i=0;i<p->T_datas.size();i++)
    {
        x<<i*(*baseData)[15].toDouble();
    }
    ui->widget->graph(0)->setData(x,p->V_datas);
    ui->widget->graph(1)->setData(x,p->T_datas);
    ui->widget->graph(1)->rescaleAxes();
    double scaley = p->action_velocity*1.17;
    double scaley1 = p->max_torque*1.1;
    ui->widget->yAxis->setRange(0,scaley);
    ui->widget->yAxis2->setRange(0,scaley1);
    ui->widget->replot();
}

//文档生成
void test::on_save_clicked()
{

    if(datas.getSelectNumber()!=3)
    {
        QMessageBox::warning(this,tr("Warning"),tr("记录数据还未选择！\n请点击【次别选择按钮】"),QMessageBox::Yes);
        return;
    }
    if((*newData).size()<19)
    {
        QMessageBox::warning(this,tr("Warning"),tr("样本未选择\n请点重新选择？"),QMessageBox::Yes);
        return;
    }
    wordMaker.loaddata(&datas,baseData,newData);
   if(!wordMaker.wordWriting())
   {
       QMessageBox::warning(this,tr("Warning"),tr("文件路径不存在！"),QMessageBox::Yes);
       return;
   }
}

//关闭按钮
void test::on_pushButton_3_clicked()
{
    this->close();
}

//调速按钮
void test::on_pushButton_4_clicked()
{
    if(!saveFlag)
    {
        on_savaThisDatas_clicked();
        /*
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),tr("数据还未保存，是否保存？"),
                                            QMessageBox::Yes | QMessageBox::No );
        if (reply == QMessageBox::Yes)
        {
            saveFlag = true;
            //增加数据保存函数;
        }
        */
    }
    this->close();
    emit tiaosu();

}

//系统设置按钮
void test::on_pushButton_6_clicked()
{
    if(!saveFlag)
    {
        on_savaThisDatas_clicked();
        /*
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),tr("数据还未保存，是否保存？"),
                                            QMessageBox::Yes | QMessageBox::No );
        if (reply == QMessageBox::Yes)
        {
            saveFlag = true;
            //增加数据保存函数;
        }
        */
    }
    this->close();
    emit system();

}

//参数设置按钮
void test::on_pushButton_7_clicked()
{
    if(!saveFlag)
    {
        on_savaThisDatas_clicked();
        /*
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),tr("数据还未保存，是否保存？"),
                                            QMessageBox::Yes | QMessageBox::No );
        if (reply == QMessageBox::Yes)
        {
            saveFlag = true;
            //增加数据保存函数;
        }
        */
    }
    this->close();
    emit parament();

}

//碟簧压缩量修改
void test::on_decrement_textChanged()
{
     QString temp;
     temp = ui->decrement->toPlainText();
     QRegExp regexp_digital(tr("[0-9]+([\.]{1}[0-9]*){0,1}"));
     if(!regexp_digital.exactMatch(temp))
     {
         ui->decrement->setFocus();
         QMessageBox::warning(this,tr("Warning"),tr("【碟簧压缩量测量值】格式错误!\n请正确输入数值"),QMessageBox::Yes);
         return;
     }
     datas.setBrlength(temp.toDouble(),ui->Times->currentIndex());
}

//保存数据
void test::on_savaThisDatas_clicked()
{
    qDebug()<<(*baseData)[12];
    if(!QFile::exists((*baseData)[12]))
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件路径不存在!"),QMessageBox::Yes);
        return;
    }
    QFile *file= new QFile((*baseData)[12]+"/"+(*baseData)[2]+(*baseData)[3]+".txt");

    file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file)
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件保存失败!"),QMessageBox::Yes);
        return;
    }
    QTextStream txtOutput(file);
    QString temp;
    const DataNode *p;
    for(int i=0;i<datas.getLength();i++)
    {
        p = datas.getData(i+1);

        for(int j=0;j<p->V_datas.size();j++)
            temp +=QString("%1").arg(p->V_datas[j])+'|';
        temp.chop(1);
        temp +='$';
        for(int j=0;j<p->T_datas.size();j++)
            temp +=QString("%1").arg(p->T_datas[j])+'|';
        temp.chop(1);
        temp +='$';
        temp +=QString("%1").arg(p->max_torque)+'$';
        temp +=QString("%1").arg(p->action_velocity)+'$';
        temp +=QString("%1").arg(p->error_rate)+'$';
        temp +=QString("%1").arg(p->br_length)+';';
    }
    temp +=QString("%1").arg(datas.getLength());
    txtOutput<<temp;
    file->close();
    delete file;

}

//打开数据
void test::on_open_clicked()
{
    if(!QFile::exists((*baseData)[12]))
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件路径不存在!"),QMessageBox::Yes);
        return;
    }
    QString filename = QFileDialog::getOpenFileName (this,tr("选择文件"),(*baseData)[12],"file(*.txt)",0);
    if(filename==NULL)return;
    qDebug()<<filename;
    for(int i=0,n=ui->Times->count();i<n;i++)
    {
        ui->Times->removeItem(1);
    }
    QFile *file = new QFile(filename);
    file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(!file)
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件打开失败!"),QMessageBox::Yes);
        return;
    }
    QString strLine = file->readAll();
    file->close();
    delete file;
    QStringList loaddatas = strLine.split(QRegExp("[;]"));
    if(loaddatas.size()<1)
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件错误!"),QMessageBox::Yes);
        return;
    }
    QStringList temp,datas2;
    DataNode *currentDatas;
    QVector<double>  velocitys,torques;

    ui->Times->clear();
    ui->Times->addItem(QString(""));
    for(int i=0,n=ui->Times->count();i<n;i++)
    {
        ui->Times->removeItem(1);
    }
    for(int i=0;i<loaddatas.size()-1;i++)
    {
         temp.clear();
         temp = loaddatas[i].split(QRegExp("[$]"));
         if(temp.size()!=6)
         {
             QMessageBox::warning(this,tr("Warning"),tr("文件错误!"),QMessageBox::Yes);
             return;
         }
         datas2.clear();
         datas2 = temp[0].split(QRegExp("[|]"));

         velocitys.clear();
         for(int j=0;j<datas2.size();j++)
         {
            velocitys<<datas2[j].toDouble();
         }
         datas2.clear();
         datas2 = temp[1].split(QRegExp("[|]"));

         torques.clear();
         for(int j=0;j<datas2.size();j++)
         {
            torques<<datas2[j].toDouble();
         }

         currentDatas = new DataNode();
         currentDatas->V_datas = velocitys;
         currentDatas->T_datas = torques;
         currentDatas->max_torque = temp[2].toDouble();
         currentDatas->action_velocity = temp[3].toDouble();
         currentDatas->error_rate = temp[4].toDouble();
         currentDatas->br_length = temp[5].toDouble();
         currentDatas->select_state = false;
         currentDatas->next = NULL;
         datas.PustData(currentDatas);
         addItems();
    }
    datas.setLength(loaddatas[loaddatas.size()-1].toInt());
    saveFlag = true;
}

//电机复位
void test::on_recovery_clicked()
{
    dataCapture->resetMachine();                              //读数据线程启动
}

//crc校验
unsigned int test::CRCcorrect(uchar *data_value,unsigned char data_length)
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
