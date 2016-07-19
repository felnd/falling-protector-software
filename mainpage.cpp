#include "mainpage.h"
#include "ui_mainpage.h"
#include "QStringListModel"

MainPage::MainPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainPage),
    CurIndex(0)
{
    ui->setupUi(this);
    this->contload();
    //链接基本数据
    this->basedatas = paramenters.getDatas();
    //链接新建样本数据或者选择样本数据
    this->newdatas = addnew.getData();



    connect(&paramenters,SIGNAL(closed()),this,SLOT(loadaddnew()));
    connect(&paramenters,SIGNAL(tiaosu()),this,SLOT(on_normal_test_clicked()));
    connect(&paramenters,SIGNAL(autotest()),this,SLOT(on_autotest_clicked()));
    connect(&paramenters,SIGNAL(system()),this,SLOT(on_system_clicked()));

    connect(&test2page,SIGNAL(closed()),this,SLOT(show()));
    connect(&test2page,SIGNAL(autotest()),this,SLOT(on_autotest_clicked()));
    connect(&test2page,SIGNAL(system()),this,SLOT(on_system_clicked()));
    connect(&test2page,SIGNAL(parament()),this,SLOT(on_paramenters_clicked()));

    connect(&testpage,SIGNAL(closed()),this,SLOT(show()));
    connect(&testpage,SIGNAL(tiaosu()),this,SLOT(on_normal_test_clicked()));
    connect(&testpage,SIGNAL(system()),this,SLOT(on_system_clicked()));
    connect(&testpage,SIGNAL(parament()),this,SLOT(on_paramenters_clicked()));

    connect(&systempage,SIGNAL(closed()),this,SLOT(show()));
    connect(&systempage,SIGNAL(tiaosu()),this,SLOT(on_normal_test_clicked()));
    connect(&systempage,SIGNAL(autotest()),this,SLOT(on_autotest_clicked()));
    connect(&systempage,SIGNAL(parament()),this,SLOT(on_paramenters_clicked()));

    //connect(&baseThread,SIGNAL(sendSpeed(double)),&test2page,SLOT(show_speed(double)));

    connect(&addnew,SIGNAL(addsignal()),this,SLOT(Slot_addnew()));
    connect(&addnew,SIGNAL(editsignal()),this,SLOT(Slot_edit()));
    connect(&addnew,SIGNAL(closed()),this,SLOT(show()));

    //testpage.setLoadData(basedatas,newdatas,&baseThread);

    //数据库读取
    strList<<"Serial_Number"<<"Application_Data"<<"Company"<<"Contact"
             <<"telephone"<<"equip_name"<<"model"<<"made_company"<<"manufacturing_No"
            <<"made_date"<<"load"<<"speed"<<"velocity"<<"luoju"<<"chishu"<<"moshu"
           <<"wendu"<<"shidu"<<"dianya";
    strList_2<<"流水号"<<"申请日期"<<"申请单位名称"<<"联系人"<<"手机"<<"设备名称"<<"型号规格"
            <<"制造单位名称"<<"出厂编号"<<"出厂日期"<<"额定制动载荷"<<"额定动作速度"<<"标定动作速度"
               <<"螺距"<<"齿数"<<"模数"<<"温度"<<"湿度"<<"电压";
//    QString path
//    QDir dir;
//    path=dir.currentPath();
//    myDB.connectDB(path,"myDB","");
//    myDB.openDB("myDB");
    dbconn = QSqlDatabase::addDatabase("QODBC", "test");
    QDir dir;
    QFile file("./myDB.mdb");
        if (!file.exists())QMessageBox::warning(this,tr("Warning"),tr("数据库不存在！")+dir.currentPath(),QMessageBox::Yes);
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=myDB.mdb;DefaultDir="+dir.currentPath());
    dbconn.setDatabaseName(dsn);
    dbconn = QSqlDatabase::database("test", false);
    if(!dbconn.open())
    {
        QMessageBox::warning(this,tr("Warning"),tr("数据库连接失败！")+dir.currentPath(),QMessageBox::Yes);
    }
    this->ShowCurdatas();
}


MainPage::~MainPage()
{
    paramenters.saveDatas();
//    myDB.closeDB();
    if(dbconn.isOpen())
    dbconn.close();
//    free(view);
   //addnew.close();
   //paramenters.close();
   // test2page.close();
   //systempage.close();
    baseThread.exit();
    delete ui;
}

//从数据库中读取数据
void MainPage::ShowCurdatas()
{
    QSqlQueryModel *q = new QSqlQueryModel();
    q->setQuery(tr("SELECT MAX(ID) FROM Sample_Records"),dbconn);
    DBrows = q->index(0,0).data().toLongLong();
    high = DBrows;
    low = DBrows-10;
    if(low<0)low = 0;
    QString str=NULL;
    qDebug()<<high<<"\t"<<low;
    for(int i=0;i<strList.size();i++)
    {
        str +=strList[i]+',';
    }
    str.chop(1);
    str = "SELECT "+str+" From Sample_Records Where ID >"+QString("%1").arg(low)+" AND ID <= "+QString("%1").arg(high);
    qDebug()<<str;
    q->setQuery(str,dbconn);
//    q->setQuery("SELECT * From Sample_Records where ID >0 AND ID <3",dbconn);
//    qDebug()<<q->lastError();
    for(int i=0;i<strList.size();i++)
        q->setHeaderData(i,Qt::Horizontal,strList_2[i]);
    ui->tableView->setModel(q);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

void MainPage::contload()
{

}

//新增按钮按下
void MainPage::on_addnewitem_clicked()
{ 
    addnew.loaddata(basedatas,&dbconn);
    addnew.show();
    this->hide();
}

//参数设置按钮
void MainPage::on_paramenters_clicked()
{
    this->hide();
    paramenters.setDatas();
    paramenters.show();
}

//自动测试按钮
void MainPage::on_autotest_clicked()
{
    this->hide();
    if(testpage.setLoadData(basedatas,newdatas,&baseThread))
    {
        testpage.show();
    }
    else
        this->show();
}

//调速按钮
void MainPage::on_normal_test_clicked()
{
    this->hide();
    test2page.datasLink(basedatas,&baseThread);
    test2page.show();
}

//系统设置按钮
void MainPage::on_system_clicked()
{
    this->hide();
    systempage.linkdatas(basedatas);
    systempage.show();
}

//给addnew重新加载基础数据
void MainPage::loadaddnew()
{
    this->show();
    addnew.loaddata(basedatas,&dbconn);
}

//获取基础数据
QString MainPage::getBaseData(int n)
{
    if(n<0||n>ITEMS)return NULL;
    return (*basedatas)[n];
}

//新增数据之后调整主页显示数据
void MainPage::Slot_addnew()
{
//    dbconn.open();
    DBrows++;
    high = DBrows;
    low = DBrows-10;
    if(low<0)low = 0;
    this->DatasRange();

}

//主页显示数据库数据的10条数据
void MainPage::DatasRange()
{
    QString str=NULL;
    QSqlQueryModel *q = new QSqlQueryModel();
    for(int i=0;i<strList.size();i++)
    {
        str +=strList[i]+',';
    }
    str.chop(1);
    str = "SELECT "+str+" From Sample_Records Where ID >"+QString("%1").arg(low)+" AND ID <= "+QString("%1").arg(high);
    qDebug()<<str;
    q->setQuery(str,dbconn);
    for(int i=0;i<strList.size();i++)
        q->setHeaderData(i,Qt::Horizontal,strList_2[i]);
    ui->tableView->setModel(q);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

//表格双击进入编辑模式
void MainPage::on_tableView_doubleClicked(const QModelIndex &index)
{
    strList_3.clear();
    CurIndex = index.row();
    for(int i=0;i<strList.size();i++)
        strList_3<<ui->tableView->model()->index(CurIndex,i).data().toString();
    addnew.eidtItems(strList_3);
    addnew.loaddata(basedatas,&dbconn);
    this->hide();
    addnew.show();
}

//表格单击选中项
void MainPage::on_tableView_clicked(const QModelIndex &index)
{
    CurIndex = index.row();
    (*newdatas).clear();
    for(int i=0;i<strList.size();i++)
       (*newdatas)<<ui->tableView->model()->index(CurIndex,i).data().toString();
}

//编辑按钮
void MainPage::on_EidtData_clicked()
{
    strList_3.clear();
    for(int i=0;i<strList.size();i++)
        strList_3<<ui->tableView->model()->index(CurIndex,i).data().toString();
    addnew.eidtItems(strList_3);
    addnew.loaddata(basedatas,&dbconn);
    this->hide();
    addnew.show();
}

//删除按钮
void MainPage::on_delete_2_clicked()
{
    QSqlQueryModel q;
    QString str = "DELETE FROM Sample_Records Where Serial_Number = \'"+ui->tableView->model()->index(CurIndex,0).data().toString()+"\'";
    q.setQuery(str,dbconn);
    DBrows = DBrows-2;
    this->Slot_addnew();
}

//搜索按钮
void MainPage::on_search_clicked()
{
    QSqlQueryModel *q = new QSqlQueryModel();
    QString str;
    for(int i=0;i<strList.size();i++)
    {
        str +=strList[i]+',';
    }
    str.chop(1);
    str = "SELECT "+str+" From Sample_Records Where  Serial_Number = \'"+ui->NumberSearch->toPlainText()+"\'";
    qDebug()<<str;
    q->setQuery(str,dbconn);
    for(int i=0;i<strList.size();i++)
        q->setHeaderData(i,Qt::Horizontal,strList_2[i]);
    ui->tableView->setModel(q);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

//显示最近的10个
void MainPage::on_moment_record_clicked()
{

    this->ShowCurdatas();
}

//向上翻页
void MainPage::on_upsheet_clicked()
{
    high += 10;
    if(high>DBrows)high = DBrows;
    low = high - 10;
    if(low<0)low = 0;
    this->DatasRange();
}

//向下翻页
void MainPage::on_downsheet_clicked()
{
    low -= 10;
    if(low<0)low = 0;
    high = low + 10;
    if(high>DBrows)high = DBrows;
    this->DatasRange();
}

//显示数据
void MainPage::Slot_edit()
{
    this->DatasRange();
}
