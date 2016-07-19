#include "writeword.h"

WriteWord::WriteWord(QObject *parent) : QObject(parent),
    GraphData(NULL),
    baseData(NULL),newData(NULL)
{
    word = new QAxWidget("Word.Application",0,Qt::MSWindowsOwnDC);
}

WriteWord::~WriteWord()
{
    word->dynamicCall("Quit()");
    delete word;
}

//数据链接
void WriteWord::loaddata(DataManager *inputGraphData,QStringList *inputBaseData,QStringList *inputNewDatas)
{
    GraphData = inputGraphData;
    baseData = inputBaseData;
    newData = inputNewDatas;
}

//word写入
bool WriteWord::wordWriting()
{
    if(QFile::exists((*baseData)[12]))
    {
        //QMessageBox::warning(this,tr("Warning"),tr("文件路径不存在!"),QMessageBox::Yes);
        return false;
    }
    word->setProperty("Visible", false);
    this->GenerData();
    QAxObject * documents = word->querySubObject("Documents");
    documents->dynamicCall("Open(QString)",QString(QCoreApplication::applicationDirPath()+"/model_1.dot"));
    QAxObject *document=word->querySubObject("ActiveDocument");
    qDebug()<<QCoreApplication::applicationDirPath();
    if(document)
         qDebug()<<"exist";
    else
        qDebug()<<"not exist";
    /*
    QAxObject *bookmark_code = document->querySubObject("Bookmarks(QVariant)","L1");
    if(!bookmark_code->isNull())
    {
        bookmark_code->dynamicCall("Select(void)");
        bookmark_code->querySubObject("Range")->setProperty("Text","textg");
    }
*/
    QAxObject *bookmark_code;
    for(int i=0;i<writeData.size();i++)
    {      
        bookmark_code = document->querySubObject("Bookmarks(QVariant)",QString("L%1").arg(i+1));
        if(!bookmark_code->isNull())
        {
            bookmark_code->dynamicCall("Select(void)");
            bookmark_code->querySubObject("Range")->setProperty("Text",writeData[i]);
        }
    }
    //document->dynamicCall("SaveAs(const QString)", QString(QCoreApplication::applicationDirPath()+"/mode2.doc"));
    document->dynamicCall("SaveAs(const QString)", QString((*baseData)[12]+"/mode2.doc"));
    qDebug()<<"Time++";
    document->dynamicCall("Close()");


    //文档2

    documents->dynamicCall("Open(QString)",QString(QCoreApplication::applicationDirPath()+"/model_2.dot"));
    document=word->querySubObject("ActiveDocument");
    if(document)
         qDebug()<<"exist";
    else
        qDebug()<<"not exist";

    for(int i=0;i<writeData2.size();i++)
    {
        bookmark_code = document->querySubObject("Bookmarks(QVariant)",QString("L%1").arg(i+1));
        if(!bookmark_code->isNull())
        {
            bookmark_code->dynamicCall("Select(void)");
            bookmark_code->querySubObject("Range")->setProperty("Text",writeData2[i]);
        }
    }
    QCustomPlot aplot;
    aplot.addGraph();
    aplot.addGraph(aplot.xAxis,aplot.yAxis2);
    aplot.xAxis->setRange(0,120);
    aplot.yAxis->setRange(0,800);
    aplot.yAxis2->setRange(0,120);
    aplot.graph(1)->setPen(QPen(Qt::red));
    aplot.xAxis2->setVisible(true);
    aplot.xAxis2->setTickLabels(false);
    aplot.yAxis2->setVisible(true);
    aplot.yAxis2->setTickLabels(true);
    aplot.graph(1)->rescaleAxes();
    aplot.xAxis->setLabel("时间(s)");
    aplot.yAxis2->setLabel("扭矩(N·m)");
    aplot.yAxis->setLabel("转速(r/min)");

    const DataNode *p;
    QVector<double> x;
    for(int ii=0;ii<3;ii++)
    {
        p = GraphData->getData(GraphData->getSelector(ii)+1);
        if(!p)return false;
        x.clear();
        for(int i=0;i<p->T_datas.size();i++)
        {
            x<<i*(*baseData)[15].toDouble();
        }
        aplot.graph(0)->setData(x,p->V_datas);
        aplot.graph(1)->setData(x,p->T_datas);
        aplot.graph(1)->rescaleAxes();
        double scaley = p->action_velocity*1.17;
        double scaley1 = p->max_torque*1.1;
        aplot.yAxis->setRange(0,scaley);
        aplot.yAxis2->setRange(0,scaley1);
        aplot.replot();
        aplot.saveJpg(QCoreApplication::applicationDirPath()+"/"+QString("%1").arg(ii)+".jpg",1000,500,1);

        bookmark_code = document->querySubObject("Bookmarks(QVariant)",QString("L%1").arg(ii+54));
        if(!bookmark_code->isNull())
        {
            bookmark_code->dynamicCall("Select(void)");
            QAxObject *range;
            range = bookmark_code->querySubObject("Range");
            QVariant tmp = range->asVariant();
            QList<QVariant>qList;
            qList<<QVariant(QCoreApplication::applicationDirPath()+"/"+QString("%1").arg(ii)+".jpg");
            qList<<QVariant(false);
            qList<<QVariant(true);
            qList<<tmp;
            QAxObject *Inlineshapes = document->querySubObject("InlineShapes");
            Inlineshapes->dynamicCall("AddPicture(const QString&, QVariant, QVariant ,QVariant)",qList);
        }
        QFile::remove(QCoreApplication::applicationDirPath()+"/"+QString("%1").arg(ii)+".jpg");
    }

    //document->dynamicCall("SaveAs(const QString)", QString(QCoreApplication::applicationDirPath()+"/mode2.doc"));
    document->dynamicCall("SaveAs(const QString)", QString((*baseData)[12]+"/mode2.doc"));

    document->dynamicCall("Close()");

    //铭牌
    QFile *file1= new QFile("./Nameplate.txt");
    QFile *file2= new QFile(QString((*baseData)[12]+"/Nameplate2.Tx6"));
    file1->open(QIODevice::ReadOnly|QIODevice::Text);
    file2->open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file1||!file2)
    {
        qDebug()<<"文件不存在";
    }
    else
    {
        QString strLine = file1->readAll();
        file1->close();
        strLine.replace(QString("【LL1】"),(*newData)[6]);
        strLine.replace(QString("【LL2】"),(*newData)[12]);
        strLine.replace(QString("【LL3】"),(*newData)[8]);
        strLine.replace(QString("【LL4】"),writeData[22].mid(0,4));
        strLine.replace(QString("【LL5】"),writeData[22].mid(5,2));
        strLine.replace(QString("【LL6】"),writeData[22].mid(8,2));
        strLine.replace(QString("【LL7】"),writeData[6]);
        QTextStream txtOutput(file2);
        txtOutput<<strLine;
        file2->close();
    }
    delete file1;
    delete file2;
    return true;
}

//word数据
void WriteWord::GenerData()
{
    writeData.clear();
    int Ritem = 0;

    writeData<<"FZ-"+(*newData)[0].left(4)+"-"+(*newData)[0].right((*newData)[0].size()-4);
    writeData<<(*newData)[2];
    writeData<<(*newData)[6];
    writeData<<(*newData)[7];
    writeData<<(QDateTime::fromString((*newData)[1], "yyyy-MM-dd")).toString("yyyy年MM月dd日");
    writeData<<writeData[0];
    writeData<<(*newData)[0];
    writeData<<(*newData)[2];
    writeData<<(*newData)[7];
    writeData<<(*newData)[6];

    qDebug()<<(*newData)[9];
    QDateTime madeTime = (QDateTime::fromString((*newData)[9], "yyyy-MM-dd"));
    QString str = madeTime.toString("yyyy年MM月dd日");
    if(NULL==str)
    {
        madeTime = QDateTime::fromString((*newData)[9], "yyyy-MM");
        str = madeTime.toString("yyyy年MM月");
    }
    writeData<<str;

    writeData<<(*newData)[8];
    writeData<<(*newData)[10];
    writeData<<(*newData)[13];
    writeData<<(*newData)[11];
    writeData<<(*newData)[14];
    writeData<<(*newData)[12];
    writeData<<(*newData)[15];
    writeData<<(*newData)[16];
    writeData<<(*newData)[18];

    writeData<<(*newData)[17];
    writeData<<writeData[4];
	
	madeTime = madeTime.addYears(5).addDays(-1);
	QDateTime temp_time = (QDateTime::fromString((*newData)[1], "yyyy-MM-dd")).addMonths(12).addDays(-1);
    if(madeTime<=temp_time)
    {
        str = madeTime.toString("yyyy年MM月dd日");
    }
    else
    {
        str = temp_time.toString("yyyy年MM月dd日");
    }
	 writeData<<str;    
	 
    qDebug()<<"here_2  "<<GraphData->getSelector(0);
    double error = GraphData->getData(GraphData->getSelector(0)+1)->error_rate;
    qDebug()<<error;
    writeData<<QString("%1").arg(error,0,'g',2);
    if(error<7)
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";

    double averror = GraphData->getaverror();
    qDebug()<<averror;
    writeData<<QString("%1").arg(averror,0,'g',2);

    qDebug()<<"here_1";

    if(error<5)
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";

    double niuju = GraphData->getData(GraphData->getSelector(0)+1)->max_torque;
    writeData<<QString("%1").arg(niuju,0,'g',1);

    double speed = (*newData)[11].toDouble();   //Vdm
    double phy = 1+speed*speed/2.94;            //1+Vdm^2/(2gL1)
    //phy*mn*z*F/2
    double Bniuju = phy*(*newData)[15].toDouble()*(*newData)[14].toDouble()*(*newData)[10].toDouble()*0.5;



    if(niuju>=Bniuju)
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";
    //pi*mn*z*l/(1000*p)
    double brlength = 3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()*GraphData->getData(GraphData->getSelector(0)+1)->br_length/(1000*(*newData)[13].toDouble());
    writeData<<QString("%1").arg(brlength,0,'g',1);
    if(brlength>=0.15&&brlength<=1.6)
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";

    qDebug()<<"here_2"<<GraphData->getSelector(1);

    double niuju2 = GraphData->getData(GraphData->getSelector(1)+1)->max_torque;
    writeData<<QString("%1").arg(niuju2,0,'g',1);

    qDebug()<<"here_3";

    if(niuju2>=Bniuju*1.12)
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";


    //1.12*pi*mn*z*l/(1000*p)
    double brlength2 =1.12*3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()*GraphData->getData(GraphData->getSelector(1)+1)->br_length/(1000*(*newData)[13].toDouble());
    writeData<<QString("%1").arg(brlength2,0,'g',1);

     qDebug()<<"here_4";

    if(brlength>=0.15&&brlength<=1.6)       //1.12标准
    {
        writeData<<"合格";
        Ritem++;
    }
    else
        writeData<<"不合格";

    if(Ritem==6)
        writeData<<"合格";
    else
        writeData<<"不合格";

    writeData<<writeData[4];

    writeData2.clear();
    //qDebug()<<"here_end"<<writeData[0]<<writeData[1];
    writeData2<<(*newData)[6];
    writeData2<<(*newData)[0];
    writeData2<<writeData[10];
    writeData2<<writeData[11];
    writeData2<<writeData[12];
    writeData2<<writeData[14];
    writeData2<<writeData[16];
    writeData2<<writeData[15];
    writeData2<<writeData[13];
    writeData2<<writeData[17];

    writeData2<<writeData[7];
    writeData2<<writeData[8];
    writeData2<<writeData[21];
    writeData2<<writeData[18];
    writeData2<<writeData[20];
    writeData2<<writeData[19];

    double action_v_12 = GraphData->getData(GraphData->getSelector(2)+1)->action_velocity;
    //writeData2<<QString("%1").arg(action_v_12,0,'g',2);
    //n*(pi*mn*z)/60000 = vd  r/min->m/s
    double action_v2_12 = action_v_12*3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()/60000;
    writeData2<<QString("%1").arg(action_v2_12,0,'g',2);
    //double action_error_12 = GraphData->getData(GraphData->getSelector(2)+1)->error_rate;
    //writeData2<<QString("%1").arg(action_error_12,0,'g',2);
    if(action_error <7)
        writeData2<<"合格";
    else
        writeData2<<"不合格";

    writeData2<<writeData[29];
    writeData2<<writeData[30];

    writeData2<<writeData[33];
    writeData2<<writeData[34];
    writeData2<<writeData[27];
    writeData2<<writeData[28];
    writeData2<<writeData[31];
    writeData2<<writeData[32];
    writeData2<<writeData[35];
    double action_v = GraphData->getData(GraphData->getSelector(0)+1)->action_velocity;
    writeData2<<QString("%1").arg(action_v,0,'g',2);
    //n*(pi*mn*z)/60000 = vd  r/min->m/s
    double action_v2 = action_v*3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()/60000;
    writeData2<<QString("%1").arg(action_v2,0,'g',2);
    double action_error = GraphData->getData(GraphData->getSelector(0)+1)->error_rate;
    writeData2<<QString("%1").arg(action_error,0,'g',2);
    if(action_error <7)
        writeData2<<"合格";
    else
        writeData2<<"不合格";

    double action_v_1 = GraphData->getData(GraphData->getSelector(1)+1)->action_velocity;
    writeData2<<QString("%1").arg(action_v_1,0,'g',2);
    //n*(pi*mn*z)/60000 = vd  r/min->m/s
    double action_v2_1 = action_v*3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()/60000;
    writeData2<<QString("%1").arg(action_v2_1,0,'g',2);
    double action_error_1 = GraphData->getData(GraphData->getSelector(1)+1)->error_rate;
    writeData2<<QString("%1").arg(action_error_1,0,'g',2);
    if(action_error <7)
        writeData2<<"合格";
    else
        writeData2<<"不合格";

    double action_v_2 = GraphData->getData(GraphData->getSelector(2)+1)->action_velocity;
    writeData2<<QString("%1").arg(action_v_2,0,'g',2);
    //n*(pi*mn*z)/60000 = vd  r/min->m/s
    double action_v2_2 = action_v*3.14*(*newData)[15].toDouble()*(*newData)[14].toDouble()/60000;
    writeData2<<QString("%1").arg(action_v2_2,0,'g',2);
    double action_error_2 = GraphData->getData(GraphData->getSelector(2)+1)->error_rate;
    writeData2<<QString("%1").arg(action_error_2,0,'g',2);
    if(action_error <7)
        writeData2<<"合格";
    else
        writeData2<<"不合格";

    double action_v_3 = (action_v+action_v_1+action_v_2)/3;
    writeData2<<QString("%1").arg(action_v_3,0,'g',2);
    double  action_v2_3 = (action_v2+action_v2_1+action_v2_2)/3;
    writeData2<<QString("%1").arg(action_v2_3,0,'g',2);

    double  action_error_3 = qAbs(action_v2_3/writeData[16].toDouble()-1)*100;
    writeData2<<QString("%1").arg(action_error_3,0,'g',2);
    if(action_error_3 <5)
        writeData2<<"合格";
    else
        writeData2<<"不合格";

    //double speed = (*newData)[11].toDouble();   //Vdm
    //double phy = 1+speed*speed/2.94;            //1+Vdm^2/(2gL1)
    //phy*mn*z*F/2
    //double Bniuju = phy*(*newData)[15].toDouble()*(*newData)[14].toDouble()*(*newData)[10].toDouble()*0.5;

    writeData2<<QString("%1").arg(Bniuju,0,'g',1);
    writeData2<<writeData[27];
    double br_length = GraphData->getData(GraphData->getSelector(0)+1)->br_length;
    writeData2<<QString("%1").arg(br_length,0,'g',1);
    writeData2<<QString("%1").arg(br_length,0,'g',1);
    writeData2<<writeData[29];

    writeData2<<QString("%1").arg(Bniuju*1.12,0,'g',1);
    writeData2<<writeData[31];
    br_length = GraphData->getData(GraphData->getSelector(1)+1)->br_length;
    writeData2<<QString("%1").arg(br_length,0,'g',1);
    writeData2<<QString("%1").arg(br_length,0,'g',1);
    writeData2<<writeData[33];

}
