#include "restclient.hpp"

RestClient::RestClient(const QUrl &url, QObject *parent):
    m_url(url)
{
    m_AccMgr = new QNetworkAccessManager(parent);

    connect(m_AccMgr,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(slotFinished(QNetworkReply*)));
    m_basicPath = url.path();
    m_request.setRawHeader("Accept", QString("application/json").toUtf8());
    m_request.setRawHeader("Content-Type", QString("application/json").toUtf8());
}

RestClient::RestClient()
{

}

void
RestClient::slotFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "";
        qDebug() << reply->errorString();
        return;
    }
}

bool
RestClient::updateUrlPath(QString path)
{
    /* update url path */
    m_url.setPath(QString(m_basicPath + path));
    m_request.setUrl(QUrl::fromPercentEncoding(m_url.toEncoded()));
    return true;
}

void
RestClient::waitForServerResponse(QNetworkReply *reply)
{
    /* wait for server response */
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
}

bool
RestClient::send(const QString &path, const QJsonObject &input, HttpMethod method = POST)
{
    int acceptedHttpStatusCode = 0;
    QNetworkReply *reply;
    if(input.isEmpty())
        return false;

    /* json object to json text */
    QJsonDocument jsonData;
    jsonData.setObject(input);
    QByteArray postData = jsonData.toJson();

    updateUrlPath(path);

    /* send POST request */
    if(method == POST) {
        reply = m_AccMgr->post(m_request, postData);
        acceptedHttpStatusCode = 201;
    } else {/* PUT */
        reply = m_AccMgr->put(m_request, postData);
        acceptedHttpStatusCode = 200;
    }

    if(reply == nullptr) return false;
    waitForServerResponse(reply);    

    /* set replay module pointer */
    m_data = reply->readAll();
    //qDebug() << m_data;

    /* check for erros */
    if(httpStatusCode(reply) != acceptedHttpStatusCode)
    {
        /* some error occured */
        m_error.setError(httpStatusCode(reply), \
                         QJsonDocument::fromJson(m_data).object(), \
        (method == POST ? "POST" : (method == PUT ? "PUT" : "Undefined Method")));
        reply->deleteLater();
        return false;
    }
    reply->deleteLater();
    return true;
}

bool
RestClient::get(const QString &path, QJsonObject &output)
{
    QJsonArray array;
    if(get(path, array) == false)
        return false;
    if(m_paginator.isEmpty() == false) {
        qDebug() << "Received collection when single object requested.";
        return false;
    }
    output = array.first().toObject();
    return true;
}

bool
RestClient::get(const QString &path, QJsonArray &output)
{
    updateUrlPath(path);
    qDebug() << m_request.url();
    /* send GET request */
    QNetworkReply *reply = m_AccMgr->get(m_request);

    if(reply == nullptr) return false;
    waitForServerResponse(reply);
    m_data = reply->readAll();

    QJsonObject serverOutput(QJsonDocument::fromJson(m_data).object());

    /* check for erros */
    if(httpStatusCode(reply) != 200)
    {
        /* some error occured */
        m_error.setError(httpStatusCode(reply), serverOutput, "GET");
        reply->deleteLater();
        return false;
    }

    ApiCollection collection(serverOutput);

    if(collection.isEmpty()) {
        QJsonArray newSingleObject;
        newSingleObject.append(serverOutput);
        output = newSingleObject;
        m_paginator.clear();
    } else {
        output = collection.getData();
        m_paginator = collection.getPaginator();
    }

    reply->deleteLater();
    return true;
}

bool
RestClient::insert(const QString &path, const QJsonObject &input)
{
    return send(path, input, POST);
}

bool
RestClient::update(const QString &path, QJsonObject &input)
{
    return send(path, input, PUT);
}

bool
RestClient::del(const QString &path)
{
    updateUrlPath(path);

    /* send DELETE request */
    QNetworkReply *reply = m_AccMgr->deleteResource(m_request);

    if(reply == nullptr) return false;
    waitForServerResponse(reply);

    /* check for erros */
    if(httpStatusCode(reply) != 204)
    {
        /* some error occured */
        QJsonObject json;
        json["message"] = QJsonValue(reply->readAll().data());
        m_error.setError(httpStatusCode(reply), \
                         json, \
                         "DELETE");
        reply->deleteLater();
        return false;
    }
    reply->deleteLater();
    return true;
}

int RestClient::httpStatusCode(QNetworkReply *reply)
{
    return reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}

ApiError &RestClient::lastError(void)
{
    return m_error;
}

const ApiPaginator &RestClient::paginator(void) const
{
    return m_paginator;
}

const QJsonDocument RestClient::getReplyJsonDocument(void) const
{
    return QJsonDocument::fromJson(m_data);
}
