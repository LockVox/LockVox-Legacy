#include "csessioncookie.h"

CSessionCookie::CSessionCookie()
{

}

QString CSessionCookie::getCookie() const
{
    return m_cookie;
}

void CSessionCookie::setCookie(const QString &cookie)
{
    m_cookie = cookie;
}

