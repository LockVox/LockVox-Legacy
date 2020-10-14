#include "src/includes/cdatabase.h"

CDatabase::CDatabase()
{
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setHostName("127.0.0.1");
        m_db.setDatabaseName("lockvox");
        m_db.setUserName("lockvox");
        m_db.setPassword("1ock-vox-passwd");
        bool ok = false;
        ok = m_db.open();
        if(ok == true)
            printf("Connection establish");
}
