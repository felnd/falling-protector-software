#include "selections.h"
#include "ui_selections.h"
#include <QDebug>

Selections::Selections(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Selections),
    selects(0)
{
    ui->setupUi(this);
    this->ui->tableWidget->setColumnCount(5);
    QStringList headerLabels;
    headerLabels<<"最大制动力矩测量值(T·m)"<<"动作转速测量值(r/min)"<<"误差(%)"<<"次别选择"<<"制动距离";
    this->ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(testSlot(int,int)));
}
Selections::~Selections()
{
    delete ui;
}

//设置表格的行数
bool Selections::setTable(int row)
{
    rows = row;
    this->ui->tableWidget->setRowCount(row);
    return true;
}

//数据链接表格插入数据
bool Selections::addDatas(DataManager *inputdata, QString velocity)
{
    data = inputdata;
    Bspeed = velocity;
    selects = 0;
    ui->tableWidget->clear();
    QTableWidgetItem *item0,*item1,*item2,*item3;
    QHBoxLayout *hLayout;
    QWidget *widget;
    QString txt;
    for (int i = 0; i < rows; i++)
    {

        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;
        item3 = new QTableWidgetItem;

        CheckBox[i] = new QCheckBox;
//        if(data->getState(i+1))
//            CheckBox[i]->setChecked(true);

        txt = QString("%1").arg(inputdata->getData(i+1)->max_torque);
        item0->setText(txt);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, item0);

        txt = QString("%1").arg(inputdata->getData(i+1)->action_velocity);
        item1->setText(txt);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 1, item1);

        txt = QString("%1%").arg(inputdata->getData(i+1)->error_rate);
        item2->setText(txt);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, item2);

        txt = QString("%1%").arg(inputdata->getData(i+1)->br_length);
        item3->setText(txt);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, item3);


//        CheckBox[i]->setText(tr("scan"));
        hLayout = new QHBoxLayout();
        CheckBox[i]->setEnabled(true);
        hLayout->addWidget(CheckBox[i]);
        hLayout->setAlignment(CheckBox[i], Qt::AlignCenter);
        widget = new QWidget(ui->tableWidget);
        widget->setLayout(hLayout);
        ui->tableWidget->setCellWidget(i,4,widget);
        connect(this->CheckBox[i], SIGNAL(stateChanged(int)), this, SLOT(setvalue(int)));
    }

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
//    free(item0);
//    free(item1);
//    free(item2);
}

//关闭
void Selections::on_pushButton_2_clicked()
{
    this->close();
}

//设置选择数据
void Selections::setvalue(int arg1)
{
    int i,j,flag;
    if(arg1==2)
    {
        if(selects<MAXSLECTIONS)
        {

            for(i=0;i<rows;i++)
            {
                if(CheckBox[i]->checkState()==2)
                {
                    flag=0;
                    for(j=0;j<selects;j++)
                    {
                        if(i==selectors[j])flag=1;
                    }
                    if(flag!=1)
                    {
                        selectors[selects]=i;
                        selects++;
                    }
                }
            }
        }
        else
        {
            for(i=0;i<rows;i++)
            {
                if(CheckBox[i]->checkState()==2)
                {
                    flag = 0;
                    for(j=0;j<selects;j++)
                    {
                        if(i==selectors[j])flag=1;
                    }
                    if(flag==0)CheckBox[i]->setCheckState(Qt::Unchecked);
                }
            }
        }
    }
    else
    {
        for(i=0;i<selects;i++)
        {
            if(CheckBox[selectors[i]]->checkState()==0)
            {
                while(i<selects-1)
                {
                    selectors[i]=selectors[i+1];
                    i++;
                }
                selects--;
            }
        }
    }
    average = 0;
    for(i=0;i<selects;i++)
    {
        average += qAbs(this->ui->tableWidget->item(selectors[i],1)->text().toDouble());
    }

        if(selects!=0)average = qAbs(average/selects/Bspeed.toDouble()-1);
        else average = 0;

    ui->label_3->setText(QString("%1%").arg(average,4,'g'));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::green);
    if(average<5)pe.setColor(QPalette::WindowText,Qt::green);
    else pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_3->setPalette(pe);
}

//表格选择
void Selections::testSlot(int row,int colomn)
{
    if(colomn==3)
    {
            if(CheckBox[row]->checkState()==0)
            {
                if(selects<MAXSLECTIONS)
                {
                    selectors[selects]=row;
                    selects++;
                    CheckBox[row]->setCheckState(Qt::Checked);
                }
            }
            else if(CheckBox[row]->checkState()==2)
            {
                if(selects>0)
                {
                    for(int i =0;i<selects;i++)
                    {
                        if(selectors[i]==row)
                        {
                            while(i<selects-1)
                            {
                                selectors[i] = selectors[i+1];
                                i++;
                            }
                        }
                    }
                    selects--;
                    CheckBox[row]->setCheckState(Qt::Unchecked);
                }
            }
     }

}

bool Selections::loaddata(DataManager *inputdatas)
{
    if(!inputdatas)return false;
    data = inputdatas;

}

//确认按钮
void Selections::on_OK_clicked()
{
    data->setSelectNumber(selects);
    for(int i=1;i<=data->getLength();i++)
        data->setState(false,i);
    for(int i=0;i<selects;i++)
        data->setState(true,selectors[i]+1);
    data->setaverror(average);
    data->setSelector(selectors,selects);
    this->close();
}
