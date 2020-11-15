#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QEventLoop>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "apierror.hpp"
#include "apicollection.hpp"
#include "apipaginator.hpp"

class RestClient : public QObject
{
    Q_OBJECT

public:
    RestClient();
    RestClient(const QUrl &url, QObject *parent = 0);

    bool get(const QString &path, QJsonObject &output);
    bool get(const QString &path, QJsonArray &output);
    bool insert(const QString &path, const QJsonObject &input);
    bool update(const QString &path, QJsonObject &input);
    bool del(const QString &path);
    ApiError &lastError(void);
    const ApiPaginator &paginator(void) const;
    const QJsonDocument getReplyJsonDocument(void) const;

private slots:
    void slotFinished(QNetworkReply *);

signals:

private:
    enum HttpMethod {
        POST = 1,
        PUT = 2,
        GET = 3,
        DELETE = 4
    };
    bool send(const QString &path, const QJsonObject &input, HttpMethod method);
    bool updateUrlPath(QString url);
    void waitForServerResponse(QNetworkReply *reply);
    int httpStatusCode(QNetworkReply *reply);

    QNetworkAccessManager *m_AccMgr;
    QNetworkRequest m_request;
    QUrl m_url;
    QString m_basicPath;
    ApiError m_error;
    ApiPaginator m_paginator;
    QByteArray m_data;
};

#endif // RESTCLIENT_H
