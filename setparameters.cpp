#include "setparameters.h"
#include "ui_setparameters.h"

SetParameters::SetParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetParameters)
{
    ui->setupUi(this);
    //基础数据文件
    file=new QFile("./basedata.txt");
    this->loaddata();
    ui->File_route->setReadOnly(true);
}

SetParameters::~SetParameters()
{
    delete file;
    delete ui;
}

QStringList* SetParameters::getDatas()
{
    return &(this->datas);
}

//从文件读取数据
bool SetParameters::loaddata()
{
    file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(!file)
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件打开失败!"),QMessageBox::Yes);
        return false;
    }
    QString strLine = file->readAll();
    file->close();
    datas = strLine.split(QRegExp("[|]"));
    if(datas.size()==ITEMS)
    {
        if(!checkDigtal(datas[3]))datas[3]="0000";
        ui->user->setText(datas[0]);
        ui->password->setText(datas[1]);
        ui->number->setText(datas[2]);
        ui->nextvalue->setText(datas[3]);
        ui->model->setText(datas[4]);
        ui->company->setText(datas[5]);
        ui->load->setText(datas[6]);
        ui->speed->setText(datas[7]);
        ui->velocity->setText(datas[8]);
        ui->luoju->setText(datas[9]);
        ui->chishu->setText(datas[10]);
        ui->moshu->setText(datas[11]);
        ui->File_route->setText(datas[12]);
        return true;
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件信息不全!"),QMessageBox::Yes);
        return false;
    }
}

//设置各项数据
bool SetParameters::setDatas()
{
    if(datas.size()==ITEMS)
    {
        ui->user->setText(datas[0]);
        ui->password->setText(datas[1]);
        ui->number->setText(datas[2]);
        ui->nextvalue->setText(datas[3]);
        ui->model->setText(datas[4]);
        ui->company->setText(datas[5]);
        ui->load->setText(datas[6]);
        ui->speed->setText(datas[7]);
        ui->velocity->setText(datas[8]);
        ui->luoju->setText(datas[9]);
        ui->chishu->setText(datas[10]);
        ui->moshu->setText(datas[11]);
        ui->File_route->setText(datas[12]);
        return true;
    }
    return false;
}

//确定按钮
void SetParameters::on_saveData_clicked()
{
     QString temp;
     QChar achar;
     datas[0] = ui->user->toPlainText().trimmed();
     datas[1] = ui->password->toPlainText().trimmed();
     datas[2] = ui->number->toPlainText().trimmed();
     temp = ui->nextvalue->toPlainText().trimmed();
     if(!checkDigtal(temp))
     {
         QMessageBox::warning(this,tr("Warning"),tr("下一个值必须为数字!"),QMessageBox::Yes);
         ui->nextvalue->setFocus();
         return;
     }
     datas[3] = temp;
     datas[4] = ui->model->toPlainText().trimmed();
     datas[5] = ui->company->toPlainText().trimmed();
     datas[6] = ui->load->toPlainText().trimmed();
     datas[7] = ui->speed->toPlainText().trimmed();
     datas[8] = ui->velocity->toPlainText().trimmed();
     datas[9] = ui->luoju->toPlainText().trimmed();
     datas[10] = ui->chishu->toPlainText().trimmed();
     datas[11] = ui->moshu->toPlainText().trimmed();
     datas[12] = ui->File_route->toPlainText().trimmed();
     for(int i=3;i<12;i++)
     {
         achar = datas[i][datas[i].size()-1];
         if(achar==';'||achar==65307)
             datas[i].chop(1);
     }
     this->close();
}

//关闭按钮
void SetParameters::closeEvent(QCloseEvent *e)
{
    emit closed();
}

//取消
void SetParameters::on_cancel_clicked()
{
    this->close();
}

//数字格式检测
bool SetParameters::checkDigtal(QString str)
{
    if(str==NULL)return false;
    int i = 0;
    for(int i=0;i<str.size();i++)
    {
        if(str[i]<'0'||str[i]>'9')
            return false;
    }
    return true;
}

//保存数据
bool SetParameters::saveDatas()
{
    file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file)
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件保存失败!"),QMessageBox::Yes);
        return false;
    }
    QTextStream txtOutput(file);
    for(int i=0;i<ITEMS-1;i++)
    {
        txtOutput<<datas[i]<<"|";
    }
//    txtOutput <<ui->user->toPlainText().trimmed()<<"|"<<
//    ui->password->toPlainText().trimmed()<<"|"<<
//    ui->number->toPlainText().trimmed()<<"|"<<
//    ui->nextvalue->toPlainText().trimmed()<<"|"<<
//    ui->model->toPlainText().trimmed()<<"|"<<
//    ui->company->toPlainText().trimmed()<<"|"<<
//    ui->load->toPlainText().trimmed()<<"|"<<
//    ui->speed->toPlainText().trimmed()<<"|"<<
//    ui->velocity->toPlainText().trimmed()<<"|"<<
//    ui->luoju->toPlainText().trimmed()<<"|"<<
//    ui->chishu->toPlainText().trimmed()<<"|"<<
//    ui->moshu->toPlainText().trimmed()<<"|"<<endl;
    file->close();
    return true;
}

//选择文件路径
void SetParameters::on_open_clicked()
{
//    QString file = QFileDialog::getOpenFileName(this,tr("Open File"),
//                      "./",
//                      tr("All Files (*.*)"));
    QString file = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "./",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    ui->File_route->setText(file);
}

//关闭
void SetParameters::on_pushButton_3_clicked()
{
    this->close();
}

//调速按钮
void SetParameters::on_pushButton_4_clicked()
{
    this->close();
    emit tiaosu();

}

//自动测试按钮
void SetParameters::on_pushButton_5_clicked()
{
    this->close();
    emit autotest();

}

//系统设置
void SetParameters::on_pushButton_6_clicked()
{
    this->close();
    emit system();

}
