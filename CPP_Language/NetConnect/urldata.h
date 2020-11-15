#ifndef URLDATA_H
#define URLDATA_H

#include <QObject>
#include <QDebug>

#include <QUrl>

class urlData
{
public:
    explicit urlData();

    void        setScheme(const QString &scheme);
    void        setUser(const QString &user);
    void        setPassword(const QString &password);
    void        setHost(const QString &host);
    void        setPath(const QString &path);

    QString     getScheme();
    QString     getUser();
    QString     getPassword();
    QString     getHost();
    QString     getPath();

    QUrl        getUrlObject(void);
    QUrl        getUrlObjectWithoutUserInfo(void);
    QString     getUrlPath(void);
    QString     getUrlPathWithAccess(void);
    QString     getBasicAuthorizationHeader(void);

private:
    QString m_user;
    QString m_password;
    QString m_scheme;
    QString m_host;
    QString m_path;
};

#endif // URLDATA_H
