#include "test2.h"
#include "ui_test2.h"

test2::test2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test2),
    data_length(8)
{
    ui->setupUi(this);
}

test2::~test2()
{
    delete ui;
}

//关闭按钮
void test2::closeEvent(QCloseEvent *e)
{
    disconnect(newThread,SIGNAL(sendSpeed(double)),this,SLOT(show_speed(double)));
    emit closed();
}

//数据链接
void test2::datasLink(QStringList *inputDatas,Data_captureThread *inputThread)
{
    basedata = inputDatas;
    newThread = inputThread;
    newThread->setDataStore(NULL,basedata,NULL,2);
    connect(newThread,SIGNAL(sendSpeed(double)),this,SLOT(show_speed(double)));
}

//电机启动
void test2::on_start_clicked()
{
    //newThread->setnewDatas(basedata);
    QRegExp regexp_digital(tr("[1-9]*[0-9]{1}"));
    QString str = ui->initSpeed->toPlainText();
    if(!regexp_digital.exactMatch(str))
    {
        ui->initSpeed->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【初始转速】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    uint initSpeed = str.toUInt();
    newThread->setSpeed(initSpeed);         //初始转速
    newThread->startMechine();
    newThread->start();
    ui->start->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
}

//停止按钮
void test2::on_stop_clicked()
{
    //newThread->setnewDatas(basedata);
    newThread->setStopFlag();
    newThread->stopMechine();
    ui->start->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);

}

//关闭按钮
void test2::on_pushButton_clicked()
{
    this->close();
}

//自动测试
void test2::on_pushButton_3_clicked()
{
    //电机停机检测
    this->close();
    emit autotest();

}

//系统设置
void test2::on_pushButton_4_clicked()
{
    this->close();
    emit system();

}

//参数设置
void test2::on_pushButton_5_clicked()
{
    this->close();
    emit parament();
}

//速度显示
void test2::show_speed(double speed)
{
    qDebug()<<"here2";
    ui->current_speed->setText(QString("%1r/min").arg(speed));
}

//加速按钮
void test2::on_pushButton_9_clicked()
{
    QRegExp regexp_digital(tr("[1-9]*[0-9]{1}"));
    QString str = ui->initSpeed->toPlainText();
    if(!regexp_digital.exactMatch(str))
    {
        ui->initSpeed->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【初始转速】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    uint initSpeed = str.toUInt()+1;
    newThread->setSpeed(initSpeed);
    ui->initSpeed->setPlainText(QString("%1").arg(initSpeed));
}

//减速按钮
void test2::on_pushButton_10_clicked()
{
    QRegExp regexp_digital(tr("[1-9]*[0-9]{1}"));
    QString str = ui->initSpeed->toPlainText();
    if(!regexp_digital.exactMatch(str))
    {
        ui->initSpeed->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【初始转速】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    uint initSpeed = str.toUInt();
    newThread->setSpeed(initSpeed);
    ui->initSpeed->setPlainText(QString("%1").arg(initSpeed));
}

//滚轮运动
void test2::wheelEvent(QWheelEvent *event)
 {
     int numDegrees = event->delta() / 8; //滚动的角度，*8就是鼠标滚动的距离
     int numSteps = numDegrees/15; //滚动的步数，*15就是鼠标滚动的角度
     int curent = ui->initSpeed->toPlainText().toInt()+numSteps;
     if(curent<0)curent=0;
     ui->initSpeed->setPlainText(QString("%1").arg(curent));

     newThread->setSpeed((uint)curent);
     event->accept();      //接收该事件

 }

//正转反转
void test2::on_pushButton_6_clicked()
{
    newThread->counterRotation();
    on_start_clicked();
}
