#include "tag.hpp"

Tags::Tags()
{
    id_m = -1;
}

Tags::Tags(const QJsonObject &json, RestClient *restclient)
{
    m_restclient = restclient;
    create(json);
}

Tags::Tags(RestClient *restclient)
{
    qDebug() << "konstruktor Tags ze wsk. do REST i id = -1";

    id_m = -1;
    m_restclient = restclient;
}

Tags::Tags(int tagId)
{
    id_m = tagId;
}

bool Tags::create(const QJsonObject &json)
{
    id_m = json["id"].toInt();
    userId_m = json["userId"].toInt();
    uniqueKey_m = json["uniqueKey"].toString();
    validFrom_m = QDateTime::fromString(json["validFrom"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    validTo_m = QDateTime::fromString(json["validTo"].toString(),\
            "yyyy-MM-dd hh:mm:ss");
    href_m = json["href"].toString();
    return true;
}

bool Tags::create(int userId,
                  const QString& uniqueKey,
                  const QString& validFrom,
                  const QString& validTo)
{
    if(userId == -1)
        return false;
    userId_m = userId;
    uniqueKey_m = uniqueKey;
    validFrom_m = QDateTime::fromString(validFrom, "yyyy-MM-dd hh:mm:ss");
    validTo_m = QDateTime::fromString(validTo, "yyyy-MM-dd hh:mm:ss");
    return true;
}

bool Tags::update(WhatUpdate what, const QString& value)
{
    switch(what)
    {
        case U_uniqueKey:
            uniqueKey_m = value;
        break;
        case U_validFrom:
            validFrom_m = QDateTime::fromString(value, "yyyy-MM-dd");
        break;
        case U_validTo:
            validTo_m = QDateTime::fromString(value, "yyyy-MM-dd");
        break;
    }
    return true;
}

bool Tags::remove(int tagId)
{
    Q_UNUSED(tagId);
    return false;
}

QJsonObject Tags::toJsonObject() const
{
    QJsonObject item;
    item["userId"] = userId_m;
    item["uniqueKey"] = uniqueKey_m;
    item["validFrom"] = validFrom_m.toString("yyyy-MM-dd");
    item["validTo"] = validTo_m.toString("yyyy-MM-dd");
    qDebug() << "New Tag item" << item;
    return item;
}

int Tags::getId() const
{
    return id_m;
}

int Tags::getUserId() const
{
    return userId_m;
}

const QString& Tags::getUniqueKey(void) const
{
    return uniqueKey_m;
}

const QString Tags::getValidFrom(void) const
{
    return validFrom_m.toString("yyyy-MM-dd");
}

const QString Tags::getValidTo(void) const
{
    return validTo_m.toString("yyyy-MM-dd");
}

const QString Tags::getHref() const
{
    return href_m;
}

bool Tags::setId(int id)
{
    id_m = id;
    return true;
}

bool Tags::setHref(QString href)
{
    href_m = href;
    return true;
}

QDebug operator<< (QDebug d, const Tags& tag) {
    d << "---------------------------"                        << '\n';
    d << "id:                    " << tag.getId()             << '\n';
    d << "uniqueKey:             " << tag.getUniqueKey()      << '\n';
    d << "userId:                " << tag.getUserId()         << '\n';
    d << "validFrom:             " << tag.getValidFrom()      << '\n';
    d << "validTo:               " << tag.getValidTo()        << '\n';
    d << "href:                  " << tag.getHref()           << '\n';
    return d;
}
