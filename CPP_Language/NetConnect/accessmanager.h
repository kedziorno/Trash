#ifndef ACCESSMANAGER_H
#define ACCESSMANAGER_H

#include <QObject>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonDocument>
#include <QEventLoop>

#include "urldata.h"

class AccessManager : public QObject, public urlData
{
    Q_OBJECT
public:
    explicit                    AccessManager(QObject *parent = 0);

    void                        getMethod(void);
    void                        postMethod(QByteArray &postData);

    void                        dumpHeadersFromReply(QNetworkReply *reply);

    void                        setAcceptHeader(const QByteArray &accept);
    void                        setContentTypeHeader(const QByteArray &type);

    QString                     getLocationHeader(QUrl &urlObject);
    QByteArray                  getReplyData();

    QNetworkAccessManager*      getAccessManagerObject(void);
    QNetworkReply*              getReplyObject(void);
    QNetworkRequest*            getRequestObject(void);

signals:

public slots:
    void                        slotFinished(QNetworkReply *reply);

protected:
    QNetworkAccessManager*      m_AccessManagerObject;
    QNetworkReply*              m_AccessManagerReply;
    QNetworkRequest             m_AccessManagerRequest;
};

#endif // ACCESSMANAGER_H
