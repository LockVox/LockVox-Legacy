#include "cdatabase.h"

CDatabase::CDatabase()
{
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setHostName("192.168.1.13");
        m_db.setDatabaseName("lockvox");
        m_db.setUserName("lockvox");
        m_db.setPassword("1ock-vox-passwd");
        bool ok = m_db.open();
        if(ok == true)
            printf("Connection establish");
}
