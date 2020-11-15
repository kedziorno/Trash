#include "accessmanager.h"

AccessManager::AccessManager(QObject *parent) :
    QObject(parent)
{
    m_AccessManagerObject = new QNetworkAccessManager(this);

    connect(m_AccessManagerObject,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(slotFinished(QNetworkReply*)));
}

void
AccessManager::slotFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << ">>>>> " << reply->errorString() << " <<<<<";
        dumpHeadersFromReply(reply);
    }

    return;
}

void
AccessManager::getMethod()
{
    QNetworkReply *reply;
    m_AccessManagerRequest.setUrl(getUrlObject());

    reply = m_AccessManagerObject->get(m_AccessManagerRequest);

    QEventLoop loop;
    connect(reply,
            SIGNAL(finished()),
            &loop,
            SLOT(quit()));
    loop.exec();

    m_AccessManagerReply = reply;
}

void
AccessManager::postMethod(QByteArray &postData)
{
    QNetworkReply *reply;
    //m_AccessManagerRequest.setUrl(getUrlObject());

    reply = m_AccessManagerObject->post(m_AccessManagerRequest, postData);

    QEventLoop loop;
    connect(reply,
            SIGNAL(finished()),
            &loop,
            SLOT(quit()));
    loop.exec();

    m_AccessManagerReply = reply;
}

void
AccessManager::dumpHeadersFromReply(QNetworkReply *reply)
{
    qDebug() << "\n\n----- Headers for Reply -----";

    QList<QByteArray> headerList = reply->rawHeaderList();
    qint8 headerListSize = headerList.size(), index;

    for(index = 0; index < headerListSize; index++)
    {
        QByteArray item = headerList.at(index);
        qDebug() << item << "-" << reply->rawHeader(item);
    }

    qDebug() << "\n-----------------------------\n";
}

void
AccessManager::setAcceptHeader(const QByteArray &accept)
{
    m_AccessManagerRequest.setRawHeader("Accept", accept);
}

void
AccessManager::setContentTypeHeader(const QByteArray &type)
{
    m_AccessManagerRequest.setRawHeader("Content-Type", type);
}

QString
AccessManager::getLocationHeader(QUrl &urlObject)
{
    QVariant locationHeader;
    locationHeader.setValue(m_AccessManagerReply->header(QNetworkRequest::LocationHeader));

    QUrl url = locationHeader.toUrl();
    urlObject = url;

    QString urlStr;
    urlStr.append(getScheme());
    urlStr.append("://");
    urlStr.append(getHost());
    urlStr.append(getPath());

    return urlStr;
}

QByteArray
AccessManager::getReplyData()
{
    return m_AccessManagerReply->readAll();
}

QNetworkAccessManager*
AccessManager::getAccessManagerObject()
{
    return m_AccessManagerObject;
}

QNetworkReply*
AccessManager::getReplyObject()
{
    return m_AccessManagerReply;
}

QNetworkRequest*
AccessManager::getRequestObject()
{
    return &m_AccessManagerRequest;
}
