#include "user.hpp"
#include "restclient.hpp"

User::User()
{

}

User::User(RestClient *restclient)
{
    qDebug() << "konstruktor User ze wsk. do REST i id = -1";

    m_id = -1;
    m_restclient = restclient;
    m_path = "users";
}

User::User(const QJsonObject &json, RestClient *restclient)
{
    m_restclient = restclient;
    m_path = "users";
    fromJson(json);
}

bool User::fromJson(const QJsonObject &json)
{
#warning "zmienić typy liczbowe"
    m_id           = json["id"].toInt();

    m_externalkey  = json["externalKey"].toString();

    m_firstname    = json["firstName"].toString();
    m_secondname   = json["secondName"].toString();
    m_lastname     = json["lastName"].toString();

    m_login        = json["login"].toString();
    m_numericLogin = json["numericLogin"].toInt();
    m_pin          = json["pin"].toString();

    m_validfrom    = QDateTime::fromString(json["validFrom"].toString(),\
                        "yyyy-MM-dd hh:mm:ss");
    m_validto      = QDateTime::fromString(json["validTo"].toString(),\
                        "yyyy-MM-dd hh:mm:ss");
    m_created      = QDateTime::fromString(json["created"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    m_modified     = QDateTime::fromString(json["modified"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    m_privileges   = json["privileges"].toInt();
    m_language     = json["language"].toString();

    m_objHref      = json["href"].toString();
    if(json["tags"].toArray().isEmpty())
        m_tagHref = json["tags"].toObject().value("href").toString();

    QJsonArray accessIds = json["accessId"].toArray();
    for(int i = 0; i < accessIds.size(); i++)
        m_accessIds.insert(accessIds.at(i).toInt());

    QJsonArray costCenterIds = json["costCenterId"].toArray();
    for(int i = 0; i < costCenterIds.size(); i++)
        m_costCentersIds.insert(costCenterIds.at(i).toInt());

    if(json["tags"].isArray())
    {
        QJsonArray tags = json["tags"].toArray();
        int tagsSize = tags.size(), i;
        for(i = 0; i < tagsSize; i++)
        {
            int idTag = tags.at(i).toObject().value("id").toInt();
            m_tagsIds.insert(idTag);

            int userId = tags.at(i).toObject().value("userId").toInt();

            QString validFrom = \
                    tags.at(i).toObject().value("validFrom").toString();
            QString validTo = \
                    tags.at(i).toObject().value("validTo").toString();
            QString uniqueKey = \
                    tags.at(i).toObject().value("uniqueKey").toString();

            Tags tagItem(idTag);
            tagItem.create(userId, uniqueKey, validFrom, validTo);
            m_tags.append(tagItem);
        }
    } else {

    }

    QJsonArray supplierIds = json["supplierId"].toArray();
    for(int i = 0; i < supplierIds.size(); i++)
        m_suppliersIds.insert(supplierIds.at(i).toInt());

    return true;
}

int User::create(const QString& firstName,
                 const QString& lastName,
                 int privileges,
                 const QString& language,
                 const QString& login,
                 int numericLogin,
                 const QString& password,
                 int pin,
                 const QString& validFrom,
                 const QString& external_key)
{
    m_id = -1;
    m_firstname = firstName;
    m_lastname = lastName;
    m_privileges = privileges;
    m_language = language;
    m_login = login;
    m_numericLogin = numericLogin;
    m_password = password;
    m_pin = pin;
    m_validfrom = QDateTime::fromString(validFrom, "yyyy-MM-dd");
    m_externalkey = external_key;

    if(m_restclient->insert(m_path, toJson()) == false)
    {
        qDebug() << m_restclient->lastError();
    } else {
        QJsonDocument jsonResponse = m_restclient->getReplyJsonDocument();
        int LastInsertId = jsonResponse.object().value("id").toInt();
        m_id = LastInsertId;
        m_created = QDateTime::fromString(jsonResponse.object().value("created").toString(),\
                                          "yyyy-MM-dd hh:mm:ss");
        m_modified = QDateTime::fromString(jsonResponse.object().value("modified").toString(),
                                          "yyyy-MM-dd hh:mm:ss");
        m_objHref = jsonResponse.object().value("href").toString();
        m_tagHref = m_objHref + "/tags";
        qDebug() << "Last insert ID for User:" << m_id << ", created:" << m_created << ", modified:" << m_modified << ", href:" << m_objHref << ", tags:" << m_tagHref;
    }
    return true;
}

QJsonObject User::toJson(void) const
{
    QJsonObject json;

    json["firstName"] = m_firstname;
    json["lastName"] = m_lastname;
    json["privileges"] = m_privileges;
    json["language"] = m_language;
    json["login"] = m_login;
    json["numericLogin"] = m_numericLogin;
    json["password"] = m_password;
    json["pin"] = m_pin.toString();
#warning "valid_from - snake case to camel case"
    json["valid_from"] = m_validfrom.toString("yyyy-MM-dd");
    json["externalKey"] = m_externalkey;

    return json;
}

int User::getId() const
{
    // 0 - deleted object, -1 - empty object
    return m_id;
}

const QString& User::getExternalKey(void) const
{
    return m_externalkey;
}

const QString& User::getSecondName(void) const
{
    return m_secondname;
}

const QSet<int>& User::getAccessId() const
{
    return m_accessIds;
}

const QSet<int>& User::getCostCentersId() const
{
    return m_costCentersIds;
}

const QString& User::getFirstName() const
{
    return m_firstname;
}

const QString& User::getLanguage() const
{
    return m_language;
}

int User::getNumericLogin(void) const
{
    return m_numericLogin;
}

const QString User::getLogin(void) const
{
    return m_login;
}

const QString User::getPin(void) const
{
    return m_pin.toString();
}

const QString& User::getLastName() const
{
    return m_lastname;
}

int User::getPrivileges() const
{
    return m_privileges;
}

const QSet<int>& User::getSuppliersId() const
{
    return m_suppliersIds;
}

const QSet<int>& User::getTagsId(void) const
{
    return m_tagsIds;
}

const QString& User::getObjectHref(void) const
{
    return m_objHref;
}

const QString& User::getTagsHref(void) const
{
    return m_tagHref;
}

bool User::isValid(void) const
{
    return m_id >= 0;
}

const QString User::getValidFrom(void) const
{
    return m_validfrom.toString("yyyy-MM-dd");
}

const QString User::getValidTo(void) const
{
    return m_validto.toString("yyyy-MM-dd");
}

const QString User::getCreatedTime(void) const
{
    return m_created.toString("yyyy-MM-dd hh:mm:ss");
}

const QString User::getModifiedTime(void) const
{
    return m_modified.toString("yyyy-MM-dd hh:mm:ss");
}

QList<Tags> User::getTags(void) const
{
    return m_tags;
}

bool User::update(WhatUpdate what, QVariant variable)
{
    if(getId() <= 0)
    {
        qDebug() << "User Id:" << getId() << "Update niemożliwy.";
        return false;
    }

    QJsonObject updateVar;

    QStringList dateFromAndTo;

    switch(what)
    {
        case U_externalKey:
            updateVar["externalKey"] = variable.toString();
            break;
        case U_firstName:
            updateVar["firstName"] = variable.toString();
            break;
        case U_secondName:
            updateVar["secondName"] = variable.toString();
            break;
        case U_lastName:
            updateVar["lastName"] = variable.toString();
            break;
        case U_login:
            updateVar["login"] = variable.toString();
            break;
        case U_numericLogin:
            updateVar["numericLogin"] = variable.toInt();
            break;
        case U_privileges:
            updateVar["privileges"] = variable.toInt();
            break;
        case U_pin:
            updateVar["pin"] = variable.toInt();
            break;
        case U_validDateFromAndTo:
        {
            dateFromAndTo = variable.toString().split("#");
            updateVar["validFrom"] = dateFromAndTo.at(0).trimmed();
            updateVar["validTo"] = dateFromAndTo.at(1).trimmed();
            break;
        }
        case U_language:
            updateVar["language"] = variable.toString();
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
    } else {
        QJsonDocument updateResponse = m_restclient->getReplyJsonDocument();
        //qDebug() << updateResponse;
        m_modified = QDateTime::fromString( updateResponse.object().value("modified").toString(), "yyyy-MM-dd hh:mm:ss");
    }

    switch(what)
    {
        case U_externalKey:
            m_externalkey = variable.toString();
        break;
        case U_firstName:
            m_firstname = variable.toString();
        break;
        case U_secondName:
            m_secondname = variable.toString();
        break;
        case U_lastName:
            m_lastname = variable.toString();
        break;
        case U_login:
            m_login = variable.toString();
        break;
        case U_numericLogin:
            m_numericLogin = variable.toInt();
        break;
        case U_privileges:
            m_privileges = variable.toInt();
        break;
        case U_pin:
            m_pin = variable.toInt();
        break;
        case U_validDateFromAndTo:
        {
            m_validfrom = QDateTime::fromString(dateFromAndTo.at(0).trimmed(),\
                                                "yyyy-MM-dd hh:mm:ss");
            m_validto = QDateTime::fromString(dateFromAndTo.at(1).trimmed(),\
                                              "yyyy-MM-dd hh:mm:ss");
        }
        break;
        case U_language:
            m_language = variable.toString();
        break;
    }

    variable.clear();
    return true;
}

bool User::insertTag(Tags &tagObject)
{
    if(getId() == -1)
        return false;
    QString uniqueKey = tagObject.getUniqueKey();
    int tagsSize = m_tags.size(), i;
    for(i = 0; i < tagsSize; i++)
        if(!m_tags.at(i).getUniqueKey().compare(uniqueKey))
        {
            qDebug() << "UniqueKey" << uniqueKey << "is in tags list!";
            return false;
        }
    m_tags.append(tagObject);
    return true;
}

bool User::insertTag(QJsonObject &json)
{
    int tagId = json["id"].toInt();
    int userId = json["userId"].toInt();
    QString uniqueKey = json["uniqueKey"].toString();
    QString validFrom = json["validFrom"].toString();
    QString validTo = json["validTo"].toString();

    Tags newTag(tagId);
    newTag.create(userId, uniqueKey, validFrom, validTo);
    return insertTag(newTag);
}

bool User::updateTags(void)
{
    if(getId() == -1)
        return false;
    int tagsSize = m_tags.size(), i, error = 0;
    qDebug() << "Tags size for user id " << m_id << "is" << tagsSize;
    for(i = 0; i < tagsSize; i++)
    {
        qDebug() << "--------------";
        QJsonObject tagItem;
        tagItem = m_tags.at(i).toJsonObject();
        qDebug() << "Index" << i << ": " << tagItem;
        if(m_restclient->insert("/tags", tagItem) == false)
        {
            qDebug() << m_restclient->lastError();
            error = 1;
        } else {
            QJsonDocument responseJson = m_restclient->getReplyJsonDocument();
            int lastInsertIdTag = responseJson.object().value("id").toInt();
            QString hrefTag = responseJson.object().value("href").toString();
            Tags item = m_tags.at(i);
            item.setId(lastInsertIdTag);
            item.setHref(hrefTag);
            m_tags.replace(i, item);
        }
    }
    return error;
}

bool User::get(int id)
{
    QString realPath = QString("%1/%2").arg(m_path).arg(id);
    QJsonObject item;
    if(m_restclient->get(realPath, item) == true)
    {
        fromJson(item);
    } else {
        qDebug() << "get id" << id << "false (" << m_restclient->lastError() << ")";
    }
    return false;
}

bool User::remove(void)
{
    int id = getId();
    if(id > 0)
    {
        QString realPath = QString("%1/%2").arg(m_path).arg(getId());
        if(m_restclient->del(realPath) == false)
        {
            qDebug() << "delete id" << id << "false (" << m_restclient->lastError() << ")";
            return false;
        }
        m_id = 0;
        return true;
    } else {
        qDebug() << "Remove dont need (Id:" << getId() << ")";
        return false;
    }
}

bool User::operator==(const User& other) const
{
    return m_id == other.m_id;
}

bool User::operator!=(const User& other) const
{
    return m_id != other.m_id;
}

bool User::operator<(const User& other) const
{
    return m_id < other.m_id;
}

bool User::operator>(const User& other) const
{
    return m_id > other.m_id;
}

QDebug operator<< (QDebug d, const User &user) {
    if(user.getId() > 0)
    {
        d << "QDebug ---------------------------"                        << '\n';
        d << "id:                    " << user.getId()            << '\n';
        d << "externalKey:           " << user.getExternalKey()   << '\n';
        d << "firstName:             " << user.getFirstName()     << '\n';
        d << "secondName:            " << user.getSecondName()    << '\n';
        d << "lastName:              " << user.getLastName()      << '\n';
        d << "login:                 " << user.getLogin()         << '\n';
        d << "numericLogin:          " << user.getNumericLogin()  << '\n';
        d << "password:              " << "********"              << '\n';
        d << "pin:                   " << user.getPin()           << '\n';
        d << "validFrom:             " << user.getValidFrom()     << '\n';
        d << "validTo:               " << user.getValidTo()       << '\n';
        d << "created:               " << user.getCreatedTime()   << '\n';
        d << "modified:              " << user.getModifiedTime()  << '\n';
        d << "privileges:            " << user.getPrivileges()    << '\n';
        d << "language:              " << user.getLanguage()      << '\n';
        d << "accessId:              " << user.getAccessId()      << '\n';
        d << "costcentersId:         " << user.getCostCentersId() << '\n';
        d << "tagsId:                " << user.getTagsId()        << '\n';
        d << "suppliersId:           " << user.getSuppliersId()   << '\n';
        d << "hrefObject:            " << user.getObjectHref()    << '\n';
        d << "hrefTags:              " << user.getTagsHref()      << '\n';
        return d;
    } else {
        d << "QDebug : User Id:" << user.getId() << ". Użyj get(id)";
        return d;
    }
}
