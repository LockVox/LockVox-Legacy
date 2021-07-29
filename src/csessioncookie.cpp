#include "Server/includes/csessioncookie.h"

CSessionCookie::CSessionCookie()
{
    m_cookie="";
}

QString CSessionCookie::getCookie() const
{
    return m_cookie;
}

void CSessionCookie::setCookie(QString &cookie)
{
    m_cookie = cookie;
}


void CSessionCookie::clearCookie()
{
    m_cookie="";
}

QString HighRandomStringGenerator(int len)
{
    QString str;
    str.resize(len);
    for (int s = 0; s < len ; ++s)
         str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));
    return str;
}

void CSessionCookie::generateCookie()
{
    m_cookie = HighRandomStringGenerator(128);
}

