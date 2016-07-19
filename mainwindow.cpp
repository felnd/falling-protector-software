#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainpage.h"
#include <string>
#include <QMessageBox>
#include <qdir.h>
#include <QFile>
#include <qdebug.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->inital();
}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::on_LoginButton_clicked()
{
    //获取用户名
    QString user = this->ui->UserName->toPlainText();
    //获取密码
    QString pwd = this->ui->Password->toPlainText();
//    if(user==USERNAME&&pwd==PASSWORD)
    //账户验证
    if(user==mainpage.getBaseData(0)&&pwd==mainpage.getBaseData(1))
    {
         mainpage.show();
         this->close();
//        autotestpage.show();
//        addnew.show();
//        testpage.show();
//        ParameterPage.show();
//        test2page.show();
    }
    else if(user!=mainpage.getBaseData(1))
    {
        QMessageBox::warning(this,tr("Warning"),tr("用户名错误!"),QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(this,tr("Warning"),tr("密码错误!"),QMessageBox::Yes);
    }
}

//退出
void MainWindow::on_LogoutButton_clicked()
{

    this->close();
}

//快捷建
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
 if(obj == ui->UserName || obj == ui->Password )
 {
  if(event->type() == QEvent::KeyPress)
  {
   //将事件转化为键盘事件
   QKeyEvent *key_event = static_cast<QKeyEvent *>(event);

   //按下Tab键执行焦点切换事件
   if(key_event->key() == Qt::Key_Tab)
   {
    bool realm_edit_focus = ui->UserName->hasFocus();
    bool user_edit_focus = ui->Password->hasFocus();


    if(realm_edit_focus)
    {
     focusNextChild();
     ui->Password->setFocus();
    }
    else if(user_edit_focus)
    {
     focusNextChild();
     ui->UserName->setFocus();
    }
    return true;
   }
  }
 }

 return QMainWindow::eventFilter(obj, event);

}

//建立事件过滤
void MainWindow::inital()
{
    ui->UserName->installEventFilter(this);
    ui->Password->installEventFilter(this);
}
