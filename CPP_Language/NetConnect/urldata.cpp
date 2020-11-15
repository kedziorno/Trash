#include "urldata.h"

urlData::urlData()
{

}

void
urlData::setPath(const QString &path)
{
    if(path.isEmpty() || path.isNull())
        m_path = "/";
    else
        m_path = path;
}

void
urlData::setScheme(const QString &scheme)
{
    if(scheme.isEmpty() || scheme.isNull())
        m_scheme = "http";
    else
        m_scheme = scheme;
}

void
urlData::setHost(const QString &host)
{
    m_host = host;
}

void
urlData::setUser(const QString &user)
{
    m_user = user;
}

void
urlData::setPassword(const QString &password)
{
    m_password = password;
}

QString
urlData::getPath()
{
    return m_path;
}

QString
urlData::getScheme()
{
    return m_scheme;
}

QString
urlData::getHost()
{
    return m_host;
}

QString
urlData::getUser()
{
    return m_user;
}

QString
urlData::getPassword()
{
    return m_password;
}

QUrl
urlData::getUrlObject()
{
    QUrl url;

    url.setScheme(getScheme());
    url.setUserName(getUser());
    url.setPassword(getPassword());
    url.setHost(getHost());
    url.setPath(getPath());

    return url;
}

QUrl
urlData::getUrlObjectWithoutUserInfo()
{
    QUrl url = getUrlObject();
    return url.adjusted(QUrl::RemoveUserInfo);
}

QString
urlData::getUrlPath()
{
    return QString("%1://%2%3")\
            .arg(getScheme())\
            .arg(getHost())\
            .arg(getPath());
}

QString
urlData::getUrlPathWithAccess()
{
    return QString("%1://%2:%3@%4%5")\
            .arg(getScheme())\
            .arg(getUser())\
            .arg(getPassword())\
            .arg(getHost())\
            .arg(getPath());
}

QString
urlData::getBasicAuthorizationHeader()
{
    QByteArray basicAuth;
    basicAuth.append(getUser());
    basicAuth.append(":");
    basicAuth.append(getPassword());
    return basicAuth.toBase64();
}
