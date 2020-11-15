#include "accesses.hpp"

Accesses::Accesses()
{
    m_restclient = NULL;
    m_path = "accesses";
    m_id = -1;
    m_modified = m_created = QDateTime::currentDateTime();
    m_href = "";
    m_limitType = "";
    m_maxOnce = 0;
    m_periodOfTime = "";
    m_quantity = 0;
}

Accesses::Accesses(RestClient *restclient)
{
    m_restclient = restclient;
    m_path = "accesses";
    m_id = -1;
    m_modified = m_created = QDateTime::currentDateTime();
    m_href = "";
    m_limitType = "";
    m_maxOnce = 0;
    m_periodOfTime = "";
    m_quantity = 0;
}

Accesses::Accesses(const QJsonObject &json, RestClient *restclient)
{
    m_restclient = restclient;
    m_path = "accesses";
    m_id = -1;
    m_modified = m_created = QDateTime::currentDateTime();
    m_href = "";
    m_limitType = "";
    m_maxOnce = 0;
    m_periodOfTime = "";
    m_quantity = 0;
    fromJson(json);
}

bool
Accesses::fromJson(const QJsonObject &json)
{
    m_id = json["id"].toInt();
    m_created = QDateTime::fromString(json["created"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    m_modified = QDateTime::fromString(json["modified"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    m_href = json["href"].toString();
    m_limitType = json["limitType"].toString();
    m_maxOnce = json["maxOnce"].toInt();
    m_periodOfTime = json["periodOfTime"].toString();
    m_quantity = json["quantity"].toInt();
    return true;
}

QJsonObject
Accesses::toJson(void) const
{
    QJsonObject object;
    object["id"] = m_id;
    object["created"] = m_created.toString("yyyy-MM-dd hh:mm:ss");
    object["modified"] = m_modified.toString("yyyy-MM-dd hh:mm:ss");
    object["href"] = m_href;
    object["limitType"] = m_limitType;
    object["maxOnce"] = m_maxOnce;
    object["periodOfTime"] = m_periodOfTime;
    object["quantity"] = m_quantity;
    return object;
}

bool
Accesses::get(int id)
{
    QString realPath = QString("%1/%2").arg(m_path).arg(m_id);
    QJsonObject object;
    if(m_restclient->get(realPath, object) == false)
    {
        qDebug() << "get id" << id << "false (" << m_restclient->lastError() << ")";
        return false;
    } else {
        fromJson(object);
        return true;
    }
    return false;
}

bool
Accesses::create(int id, QString pofType, int quantity, \
            QString ltType, int maxOnce)
{
    m_id = id;
    m_periodOfTime = pofType;
    m_quantity = quantity;
    m_limitType = ltType;
    m_maxOnce = maxOnce;

    QJsonObject object = toJson();

    QString realPath = QString("%1").arg(m_path);
    if(m_restclient->insert(realPath, object) == false)
    {
        qDebug() << m_restclient->lastError();
        return false;
    } else {
        QJsonDocument jsonResponse = m_restclient->getReplyJsonDocument();
        int LastInsertId = jsonResponse.object().value("id").toInt();
        m_id = LastInsertId;
        m_created = QDateTime::fromString(jsonResponse.object().value("created").toString(),\
                                          "yyyy-MM-dd hh:mm:ss");
        m_modified = QDateTime::fromString(jsonResponse.object().value("modified").toString(),
                                           "yyyy-MM-dd hh:mm:ss");
        m_href = jsonResponse.object().value("href").toString();
        qDebug() << "Last insert ID for Accesses:" << m_id << ", created:" << m_created << ", modified:" << m_modified << ", href:" << m_href;
        return true;
    }
}

bool
Accesses::update(U_Accesses what, QVariant variable)
{
    if(getId() == -1)
        return false;

    QJsonObject updateVar;

    switch(what)
    {
        case U_limitType:
            updateVar["limitType"] = variable.toString();
            break;
        case U_maxOnce:
            updateVar["maxOnce"] = variable.toInt();
            break;
        case U_periodOfTime:
            updateVar["periodOfTime"] = variable.toString();
            break;
        case U_quantity:
            updateVar["quantity"] = variable.toInt();
            break;
        default:
            qDebug() << "Variable undefined:" << what;
            return false;
    }

    QString path = QString("%1/%2").arg(m_path).arg(getId());
    if(m_restclient->update(path, updateVar) == false)
    {
        qDebug() << "Error on:" << updateVar << m_restclient->lastError();
        return false;
    }

    switch(what)
    {
        case U_limitType:
            m_limitType = variable.toString();
            break;
        case U_maxOnce:
            m_maxOnce = variable.toInt();
            break;
        case U_periodOfTime:
            m_periodOfTime = variable.toString();
            break;
        case U_quantity:
            m_quantity = variable.toInt();
            break;
        default:
            qDebug() << "Variable undefined:" << what;
            return false;
    }

    variable.clear();
    return true;
}

bool
Accesses::remove()
{
    QString realPath = QString("%1/%2").arg(m_path).arg(getId());
    if(m_restclient->del(realPath) == false)
    {
        qDebug() << m_restclient->lastError();
        return false;
    }
    return true;
}

int
Accesses::getId(void) const
{
    return m_id;
}

const QString&
Accesses::getPeriodOfTime(void) const
{
    return m_periodOfTime;
}

int
Accesses::getQuantity(void) const
{
    return m_quantity;
}

const QString&
Accesses::getLimitType(void) const
{
    return m_limitType;
}

int
Accesses::getMaxOnce(void) const
{
    return m_maxOnce;
}

