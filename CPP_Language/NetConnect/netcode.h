#ifndef NETCODE_H
#define NETCODE_H

#include <QObject>
#include <QDebug>
#include <QEventLoop>

#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "urldata.h"
#include "accessmanager.h"
#include "applicationobject.h"

class StormPath : public AccessManager
{
public:
    explicit    StormPath(const QString &user, const QString &password);

    void        dumpHeadersFromReply(QNetworkReply *reply);
    void        dumpHeadersFromRequest(QNetworkRequest *request);

    void        getApplications(void);

    void        addUser(const QByteArray &data);

private:
    ApplicationObject   m_application;
};

#endif // NETCODE_H
