/**
 * @file        cclient.h
 * @brief       Declaration of the class CClient
 * @details     This class represent an user
 * @author
 * @version     0.1
 * @date        2020
 * @copyright   GNU Public License.
 */

#ifndef CSESSIONCOOKIE_H
#define CSESSIONCOOKIE_H

//QT Includes



//Includes
#include "cclient.h"




class CSessionCookie
{
    /**
     * @class       CSessionCookie csessioncookie.h "csessioncookie.h"
     * @brief       Declaration of the class CSessionCookie
     * @details     This class represent a session cookie
     * @author
     * @version     0.1
     * @date        2020
     * @note
     * @bug
     * @warning     Une mauvaise utilisation peut faire planter votre application (c'est votre faute)
     * @remark      Les destructeur non définie ça emepche de compiler !
     * @copyright   GNU GPL V3.
     */
public:
    CSessionCookie();
    CSessionCookie(CClient * c);

    //~CSessionCookie();

    //Getters
    QString getCookie() const;

    //Setters
    void setCookie(const QString &cookie);

    QString HighRandomStringGenerator();

    void clearCookie();

private:
    QString m_cookie;


};

#endif // CSESSIONCOOKIE_H
