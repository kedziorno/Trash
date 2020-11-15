#include "netcode.h"

StormPath::StormPath(const QString &user, const QString &password)
{
        setUser(user);
        setPassword(password);
        setScheme("https");
        setHost("api.stormpath.com");
        setPath("/v1/tenants/current");

        getMethod();
}

void StormPath::dumpHeadersFromReply(QNetworkReply *reply)
{
    qDebug() << "----";
    QList<QByteArray> headerList = reply->rawHeaderList();
    qint8 headerListSize = headerList.size(), index;
    for(index = 0; index < headerListSize; index++)
    {
        QByteArray item = headerList.at(index);
        qDebug() << item << "-" << reply->rawHeader(item);
    }
    qDebug() << "----";
}

void
StormPath::dumpHeadersFromRequest(QNetworkRequest *request)
{
    qDebug() << "----";
    QList<QByteArray> rawHeaderList = request->rawHeaderList();
    quint16 rawHeaderListSize = rawHeaderList.size();
    int index;
    for(index = 0; index < rawHeaderListSize; index++)
    {
        QByteArray item = rawHeaderList.at(index);
        qDebug() << item << "-" << request->rawHeader(item);
    }
    qDebug() << "----";
}

void StormPath::getApplications(void)
{
    QString type = "/applications";

    QUrl url;
    getLocationHeader(url);

    qDebug() << url;

    setPath(url.path() + type);

    setContentTypeHeader("application/json");
    getMethod();

    QJsonDocument jsonDoc = \
            QJsonDocument::fromJson(getReplyData());

    ApplicationObject application(jsonDoc.object());

    m_application = application;
}

void StormPath::addUser(const QByteArray &postData)
{
    //QByteArray b64auth =  QByteArray("6JZDX57RQRQ8GNF1YTTU7GW8I:ZvthSziaYClwIBeXvfLaEDmzrjxLWG51km0rBBeGpAs").toBase64();
    //m_AccessManagerRequest.setRawHeader("Authorization", "Basic " + b64auth);
    m_AccessManagerRequest.setRawHeader("Content-Type", "application/json");
    //m_AccessManagerRequest.setRawHeader("Accept", "");

    dumpHeadersFromRequest(&m_AccessManagerRequest);

    setPath("/v1/applications/xxx/accounts");

    //QUrl temp = ();
    //temp = temp.adjusted(QUrl::RemoveUserInfo);
    //qDebug() << temp;

    m_AccessManagerRequest.setUrl(getUrlObject());

    //qDebug() << m_AccessManagerRequest.url();

    dumpHeadersFromRequest(&m_AccessManagerRequest);

    //qDebug() << m_AccessManagerReply->rawHeaderPairs();

    QByteArray data;
    data.append("{\"givenName\": \"Joe\",\"surname\": \"Stormtrooper\",\"username\": \"tk666\",\"email\": \"tk666@stormpath.com\",\"password\": \"Changeme1\",\"customData\": {\"favoriteColor\": \"white\"}}");
    postMethod(data);

    //dumpHeadersFromReply(m_AccessManagerReply);

    qDebug() << getReplyData();
}
