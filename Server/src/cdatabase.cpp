#include "src/includes/cdatabase.h"

CDatabase::CDatabase()
{
        m_db = QSqlDatabase::addDatabase("QMYSQL");
        m_db.setHostName("127.0.0.1");
        m_db.setDatabaseName("lockvox");
        m_db.setUserName("lockvox");
        m_db.setPassword("1ock-vox-passwd");
        bool ok = false;
        ok = m_db.open("lockvox", "lock-vox-passwd");
        if(ok == true)

            printf("Connection establish\n");
        else
            printf("Connection couldn't be established...\n");
}

QSqlDatabase * CDatabase::get_db()
{
    return &m_db;
}

void CDatabase::initDatabase()
{
    return;
}

void CDatabase::AskDatabase()   //TODO réaliser un traitement avec le protocole LV
{
    return;
}
