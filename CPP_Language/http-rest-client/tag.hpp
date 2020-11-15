#ifndef TAGS_H
#define TAGS_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>

#include "restclient.hpp"

class Tags
{
    enum WhatUpdate {
        U_uniqueKey = 1,
        U_validFrom = 2,
        U_validTo = 3
    };

public:
    Tags();
    Tags(const QJsonObject &json, RestClient *restclient);
    Tags(RestClient *restclient);
    Tags(int tagId);

    bool create(const QJsonObject &json);
    bool create(int userId,
                const QString& uniqueKey,
                const QString& validFrom,
                const QString& validTo);
    bool update(WhatUpdate what, const QString& value);
    bool remove(int tagId);

    QJsonObject toJsonObject(void) const;

    int getId() const;
    int getUserId() const;
    const QString& getUniqueKey(void) const;
    const QString getValidFrom(void) const;
    const QString getValidTo(void) const;
    const QString getHref(void) const;

    bool setId(int id);
    bool setHref(QString href);

private:
    int id_m;
    int userId_m;
    QString uniqueKey_m;
    QDateTime validFrom_m;
    QDateTime validTo_m;
    QString href_m;

    RestClient *m_restclient;
};

QDebug operator<< (QDebug d, const Tags& tag);

#endif // TAGS_H
