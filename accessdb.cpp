#include "accessdb.h"
#include <QMessageBox>

AccessDB::AccessDB()
{

}

bool AccessDB::connectDB(const QString& DatabasePath,
                         const QString& connectName,
                         const QString& Password)
{
    const QString connID(connectName);
    //QString connID = PREFIX +"-"+DatabasePath;
    connection = QSqlDatabase::database(connID,false);

    if(connection.isValid())
    {
        if(connection.isOpen())
            return true;
    }
    else
    {
        connection = QSqlDatabase::addDatabase("QODBC",connID);

        QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={Meteorology Access};DBQ=%1;UID=;PWD=%2")
                .arg(DatabasePath).arg(Password);
        connection.setDatabaseName(dsn);
        //connection.setUserName(UserName);
        //connection.setPassword(Password);
        if(!connection.open())
        {
            QMessageBox::critical(0,QObject::tr("Database Error"),connection.lastError().text());
            return false;
        }
        else
        {
            newQuery();
            return true;
        }
    }

}

void AccessDB::newQuery()
{
    m_query = new QSqlQuery(connection);
}

bool AccessDB::queryDB(const QString &query)
{
    if(!m_query->prepare(query))//如果加载失败，刚断开连接，然后再连接
    {
        connection.close();
        if(!openDB(idName))
        {
            return false;
        }
        qDebug()<<"new.....Connect";
    }

    return(m_query->exec());
}

bool AccessDB::openDB(const QString& connectName)
{
    idName = connectName;
    if(!connectDB(dbName,idName,"","",password))
    {
        QMessageBox msgBox;
        msgBox.setText("数据库连接失败");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }
    return true;
}

void AccessDB::closeDB()
{
    const QString connID(idName);
    //connection = QSqlDatabase::database(connID,false);
    connection.close();
    connection.removeDatabase(connID);
}
