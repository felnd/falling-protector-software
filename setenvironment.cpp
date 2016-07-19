#include "setenvironment.h"
#include "ui_setenvironment.h"

#include <QSettings>

setEnvironment::setEnvironment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setEnvironment)
{
    ui->setupUi(this);
    QStringList temp;
    temp<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<"COM9";
    ui->IO_2->addItems(temp);
    ui->IO_1->addItems(temp);
    temp.clear();
    temp<<"正向运行"<<"反向运行";
    ui->side->addItems(temp);
}

setEnvironment::~setEnvironment()
{
    delete ui;
}

void setEnvironment::closeEvent(QCloseEvent *e)
{
    emit closed();
}

//确定按钮
void setEnvironment::on_OK_clicked()
{
    QString temp;
    QRegExp regexp_digital(tr("[0-9]+([\.]{1}[0-9]+){0,1}"));
    temp = QString("%1").arg(ui->IO_1->currentIndex());
    (*control_datas)[CURRENTNO] = temp;
    temp = QString("%1").arg(ui->IO_2->currentIndex());
    (*control_datas)[CURRENTNO+1] = temp;

    temp = ui->sf->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->sf->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【转矩转速测量采样频率】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+2] = temp;

    temp = ui->niuju->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->niuju->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【扭矩修正系数】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+3] = temp;

    temp = ui->zhuansu->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->zhuansu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【转速修正系数】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+4] = temp;

    temp = ui->zero_niuju->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->zero_niuju->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【扭矩零点】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+5] = temp;

    temp = ui->zero_sudu->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->zero_sudu->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【转速零点】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+6] = temp;

    temp = ui->zero_f->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->zero_f->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【零点频率】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+7] = temp;

    temp = ui->rate_vf->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->rate_vf->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【压频比】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+8] = temp;

    temp = QString("%1").arg(ui->side->currentIndex());
    (*control_datas)[CURRENTNO+9] = temp;

    temp = ui->target_v->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->target_v->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【防坠器目标动作转速】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+10] = temp;

    temp = ui->recover_v->toPlainText().trimmed();
    if(!regexp_digital.exactMatch(temp))
    {
        ui->recover_v->setFocus();
        QMessageBox::warning(this,tr("Warning"),tr("【飞轮复位转速】格式错误!\n请正确输入数值"),QMessageBox::Yes);
        return;
    }
    (*control_datas)[CURRENTNO+11] = temp;
    this->close();
}

//取消按钮
void setEnvironment::on_cancel_clicked()
{
    this->close();
}

//各项设置数据
bool setEnvironment::linkdatas(QStringList *inputDatalink)
{
    control_datas = inputDatalink;
    ui->IO_1->setCurrentIndex((*control_datas)[CURRENTNO].toInt());
    ui->IO_2->setCurrentIndex((*control_datas)[CURRENTNO+1].toInt());
    ui->sf->setText((*control_datas)[CURRENTNO+2]);
    ui->niuju->setText((*control_datas)[CURRENTNO+3]);
    ui->zhuansu->setText((*control_datas)[CURRENTNO+4]);
    ui->zero_niuju->setText((*control_datas)[CURRENTNO+5]);
    ui->zero_sudu->setText((*control_datas)[CURRENTNO+6]);
    ui->zero_f->setText((*control_datas)[CURRENTNO+7]);
    ui->rate_vf->setText((*control_datas)[CURRENTNO+8]);
    ui->side->setCurrentIndex((*control_datas)[CURRENTNO+9].toInt());
    ui->target_v->setText((*control_datas)[CURRENTNO+10]);
    ui->recover_v->setText((*control_datas)[CURRENTNO+11]);
    return true;
}

//恢复出厂设置按钮
void setEnvironment::on_recovery_clicked()
{
    QRegExp regexp_digital(tr("[0-9]+([\.]{1}[0-9]+){0,1}"));
    QRegExp regexp_data(tr("[0-9]+"));
    QSettings settings("defaultsetting.ini", QSettings::IniFormat);
    QString temp;
    temp = settings.value("IO/IO_1").toString();
    if(!regexp_data.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【IO_1】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->IO_1->setCurrentIndex(temp.toInt());
    (*control_datas)[CURRENTNO]=temp;

    temp = settings.value("IO/IO_2").toString();
    if(!regexp_data.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【IO_2】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->IO_2->setCurrentIndex(temp.toInt());
    (*control_datas)[CURRENTNO+1]=temp;

    temp = settings.value("Factory_reset/x1").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x1】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->sf->setText(temp);
    (*control_datas)[CURRENTNO+2]=temp;

    temp = settings.value("Factory_reset/x2").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x2】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->niuju->setText(temp);
    (*control_datas)[CURRENTNO+3]=temp;

    temp = settings.value("Factory_reset/x3").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x3】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->zhuansu->setText(temp);
    (*control_datas)[CURRENTNO+4]=temp;

    temp = settings.value("Factory_reset/x4").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x4】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->zero_niuju->setText(temp);
    (*control_datas)[CURRENTNO+5]=temp;

    temp = settings.value("Factory_reset/x5").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x5】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->zero_sudu->setText(temp);
    (*control_datas)[CURRENTNO+6]=temp;

    temp = settings.value("Factory_reset/x6").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x6】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->zero_f->setText(temp);
    (*control_datas)[CURRENTNO+7]=temp;

    temp = settings.value("Factory_reset/x7").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x7】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->rate_vf->setText(temp);
    (*control_datas)[CURRENTNO+8]=temp;

    temp = settings.value("Direction/direction").toString();
    if(!regexp_data.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【direction】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->side->setCurrentIndex(temp.toInt());
    (*control_datas)[CURRENTNO+9]=temp;

    temp = settings.value("Factory_reset/x8").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x8】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->target_v->setText(temp);
    (*control_datas)[CURRENTNO+10]=temp;

    temp = settings.value("Factory_reset/x9").toString();
    if(!regexp_digital.exactMatch(temp))
    {
        QMessageBox::warning(this,tr("Warning"),tr("【x9】格式错误!\n请修改defaultsetting.ini文件"),QMessageBox::Yes);
        return;
    }
    ui->recover_v->setText(temp);
    (*control_datas)[CURRENTNO+11]=temp;
}

//关闭按钮
void setEnvironment::on_pushButton_3_clicked()
{
    this->close();
}

//调速按钮
void setEnvironment::on_pushButton_4_clicked()
{
    this->close();
    emit tiaosu();

}

//自动测试按钮
void setEnvironment::on_pushButton_5_clicked()
{
    this->close();
    emit autotest();

}

//参数设置按钮
void setEnvironment::on_pushButton_7_clicked()
{
    this->close();
    emit parament();
}
