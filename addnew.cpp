#include "addnew.h"
#include "ui_addnew.h"
#include <QDateTime>
#include <qdebug.h>

AddNew::AddNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNew),
    datas(NULL),stateFlag(0)
{
    ui->setupUi(this);
    //时间选择
    connect(&dateTime,SIGNAL(sendTime(QDateTime)),this,SLOT(getTime(QDateTime)));
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::closeEvent(QCloseEvent *e)
{
    //stateFlag=0 为新增，stateFlag = 1为编辑
    stateFlag = 0;
    emit closed();
}

QStringList* AddNew::getData()
{
    return &datas;
}
//链接基本数据和数据库，为ui添加基本选项
void AddNew::loaddata(QStringList *inputDatas,QSqlDatabase *inputDB)
{
    baseData = inputDatas;
    DBpinter = inputDB;
    if(stateFlag!=1)
    {
        //设置时间
        ui->serial_number->setText((*baseData)[2]+(*baseData)[3]);
        QDateTime curDatetime = QDateTime::currentDateTime();
        QString current_date = curDatetime.toString("yyyy-MM-dd");
        ui->date->setText(current_date);
    }
    QStringList strList = (*baseData)[4].split(QRegExp("[;；]"));
    ui->ts->clear();
    ui->ts->insertItems(0,strList);
    strList = (*baseData)[5].split(QRegExp("[;；]"));
    ui->manufacturer->clear();
    ui->manufacturer->insertItems(0,strList);
    strList = (*baseData)[6].split(QRegExp("[;；]"));
    ui->load->clear();
    ui->load->insertItems(0,strList);
    strList = (*baseData)[7].split(QRegExp("[;；]"));
    ui->speed->clear();
    ui->speed->insertItems(0,strList);
    strList = (*baseData)[8].split(QRegExp("[;；]"));
    ui->velocity->clear();
    ui->velocity->insertItems(0,strList);
    strList = (*baseData)[9].split(QRegExp("[;；]"));
    ui->luoju->clear();
    ui->luoju->insertItems(0,strList);
    strList = (*baseData)[10].split(QRegExp("[;；]"));
    ui->chishu->clear();
    ui->chishu->insertItems(0,strList);
    strList = (*baseData)[11].split(QRegExp("[;；]"));
    ui->moshu->clear();
    ui->moshu->insertItems(0,strList);
//    ui->make_day->setText("2014-1");
    ui->equip_name->setPlainText("SC型施工升降机齿轮锥鼓形防坠安全器");
    ui->wendu->setText("25");
    ui->shidu->setText("70");
    ui->dianya->setText("380");
}

//确定按钮按下
void AddNew::on_correct_clicked()
{
    datas.clear();
    QString temp;
    double demp;
    //时间格式检测
    QRegExp regexp_date(tr("[0-9]{4}([-]{1}[0-9]{2}){1,2}"));
    //数据格式检测
    QRegExp regexp_digital(tr("[0-9]+([\.]{1}[0-9]+){0,1}"));
    //手机号码格式检测
    QRegExp regexp_tel(tr("([0-9]{3}[-]{0,1}[0-9]{8}){0,1}"));

    temp = ui->serial_number->toPlainText().trimmed();
    if(temp==NULL)
    {
        ui->serial_number->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("流水号不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;
    temp = ui->date->text().trimmed();
    datas<<temp;

    temp = ui->company->toPlainText().trimmed();
    if(temp==NULL)
    {
        ui->company->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("申请单位名称不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->contact->toPlainText().trimmed();
    if(temp==NULL)
    {
        ui->contact->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("联系人不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->tel->toPlainText().trimmed();
    if(!regexp_tel.exactMatch(temp))
    {
        ui->tel->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【手机】格式错误!\n正确格式为：020-44445555或13577778888"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->equip_name->toPlainText().trimmed();
    if(temp==NULL)
    {
        ui->equip_name->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("设备名称不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->ts->currentText().trimmed();
    if(temp==NULL)
    {
        ui->ts->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("型号规格不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->manufacturer->currentText().trimmed();
    if(temp==NULL)
    {
        ui->manufacturer->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("制造单位名称不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->No->toPlainText().trimmed();
    if(temp==NULL)
    {
        ui->No->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("出厂编号不能为空！"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->make_day->toPlainText().trimmed();
    if(!regexp_date.exactMatch(temp))
    {
        ui->make_day->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【日期】格式错误!\n正确格式为：2016-01或2016-01-01"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->load->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->load->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【额定制动载荷】错误!\n请保证输入全数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->load->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【额定制动载荷】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }

    datas<<temp;

    temp = ui->speed->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->speed->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【额定动作速度】错误!\n请保证输入全数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->speed->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【额定动作速度】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->velocity->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->velocity->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【标定动作速度】错误!\n请保证输入全数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->velocity->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【标定动作速度】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->luoju->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->luoju->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【螺距】错误!\n请保证输入为数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->luoju->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【螺距】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->chishu->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->chishu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【齿数】错误!\n请保证输入为数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->chishu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【齿数】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->moshu->currentText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->moshu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【模数】错误!\n请保证输入为数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->moshu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【模数】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->wendu->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->wendu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【温度】错误!\n请保证输入为数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    datas<<temp;

    temp = ui->shidu->toPlainText().trimmed();
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->shidu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【湿度】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;

    temp = ui->dianya->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->dianya->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【电压】错误!\n请保证输入为数字"),QMessageBox::Yes);
        return;
    }
    demp = temp.toDouble();
    if(demp<=0)
    {
        ui->dianya->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【电压】错误!\n请保证输入值大于零"),QMessageBox::Yes);
        return;
    }
    datas<<temp;
    (*baseData)[3]=QString("%1").arg((*baseData)[3].toInt()+1,4,10,QLatin1Char('0'));

    //写入数据库
    QStringList strList;
    strList<<"Serial_Number"<<"Application_Data"<<"Company"<<"Contact"
             <<"telephone"<<"equip_name"<<"model"<<"made_company"<<"manufacturing_No"
            <<"made_date"<<"load"<<"speed"<<"velocity"<<"luoju"<<"chishu"<<"moshu"
           <<"wendu"<<"shidu"<<"dianya";
    QSqlQueryModel q;
    QString str=NULL;
    //新增
    if(stateFlag!=1)
    {
        for(int i=0;i<strList.size();i++)
        {
            str += strList[i]+',';
        }
        str.chop(1);
        str = "INSERT INTO Sample_Records ("+str+") VALUES (";
        for(int i = 0;i<datas.size();i++)
            str += "\'"+datas[i]+"\',";
        str.chop(1);
        str +=")";
        qDebug()<<str;
        q.setQuery(str,*DBpinter);
        emit addsignal();
    }
    //修改
    else
    {
        for(int i=0;i<strList.size();i++)
        {
            str += strList[i]+" = "+ "\'"+datas[i]+"\',";
        }
        str.chop(1);
        str = "UPDATE Sample_Records set "+str+" where Serial_Number = "+"\'"+datas[0]+"\'";
        stateFlag = 0;
        ui->serial_number->setEnabled(true);
        qDebug()<<str;
        q.setQuery(str,*DBpinter);
        emit editsignal();
    }
    qDebug()<<q.lastError();
    this->close();
}

//日期选择按钮
void AddNew::on_datechoose_clicked()
{
    dateTime.setWindowModality(Qt::ApplicationModal);
    dateTime.show();
}

//获取选择时间
void AddNew::getTime(QDateTime nowdays)
{
    QString current_date = nowdays.toString("yyyy-MM-dd");
    ui->date->setText(current_date);
}

//取消按钮
void AddNew::on_cancel_clicked()
{
    this->close();
}

//编辑模式
void AddNew::eidtItems(QStringList &eidtdata)
{
    stateFlag = 1;                                  //设置状态模式为1，编辑模式
    //将数据导入各项
    ui->serial_number->setText(eidtdata[0]);
    ui->serial_number->setEnabled(false);
    ui->date->setText(eidtdata[1]);
    ui->company->setText(eidtdata[2]);
    ui->contact->setText(eidtdata[3]);
    ui->tel->setText(eidtdata[4]);
    ui->equip_name->setText(eidtdata[5]);
    ui->ts->setCurrentText(eidtdata[6]);
    ui->manufacturer->setCurrentText(eidtdata[7]);
    ui->No->setText(eidtdata[8]);
    ui->make_day->setText(eidtdata[9]);
    ui->load->setCurrentText(eidtdata[10]);
    ui->speed->setCurrentText(eidtdata[11]);
    ui->velocity->setCurrentText(eidtdata[12]);
    ui->luoju->setCurrentText(eidtdata[13]);
    ui->chishu->setCurrentText(eidtdata[14]);
    ui->moshu->setCurrentText(eidtdata[15]);
    ui->wendu->setText(eidtdata[16]);
    ui->shidu->setText(eidtdata[17]);
    ui->dianya->setText(eidtdata[18]);
}
