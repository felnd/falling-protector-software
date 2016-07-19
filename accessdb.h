#ifndef ACCESSDB_H
#define ACCESSDB_H
#include <QtSql/QtSql>

class AccessDB
{
public:
    AccessDB();
    bool connectDB(const QString& DatabasePath,
                      const QString& connectName,
                      const QString& Password = "");
       bool queryDB(const QString &query);
       void newQuery();
       //bool insertData(const QString &insert);
       bool openDB(const QString& connectName);
       //void closeDB(const QString& connectName);
        void closeDB();
       QSqlDatabase connection;
       QSqlQuery *m_query;
       QString idName;
};

#endif // ACCESSDB_H
