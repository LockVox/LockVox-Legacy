/**
 * @file        cclient.h
 * @brief       Declaration of the class CookiesManager
 * @details     This class is used to manage users session cookie
 * @author      Hugues Mattei (LockVox Team)
 * @version     0.1.1
 * @date        2021
 * @copyright   GNU GPL-3.0
 */


#ifndef COOKIESMANAGER_H
#define COOKIESMANAGER_H


//QT Includes
#include <QMap>



//Includes
#include "cclient.h"
#include "csessioncookie.h"


class CookiesManager
{
public:
    //Penser a faire les definition des constructeur et destrcteur quand on créé une classes sinon ça compile pas pour les autres...
    CookiesManager(QList<CClient*> clients);
    //~CookiesManager();

    void assignCookie(QUuid id);
    void deleteCookie(QUuid id);

    CSessionCookie getSessionCookie(QUuid id);



private:
    QMap<QString,CSessionCookie> m_sessionCookiesMap;

};

#endif // COOKIESMANAGER_H
