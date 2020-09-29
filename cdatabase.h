#ifndef CDATABASE_H
#define CDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtWidgets>

class CDatabase
{
public:
    CDatabase();



    QSqlDatabase * get_db();
    void set_db(QSqlDatabase * db);

    void initDatabase();
    void AskDatabase();


private slots:


private:
    QSqlDatabase* m_db;

};

#endif // CDATABASE_H
