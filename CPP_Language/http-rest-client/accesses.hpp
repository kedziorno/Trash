#ifndef ACCESSES_H
#define ACCESSES_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>

#include "restclient.hpp"

class Accesses
{
    enum U_Accesses {
        U_periodOfTime = 1,
        U_quantity = 2,
        U_limitType = 3,
        U_maxOnce = 4
    };

public:
    Accesses();
    Accesses(RestClient *restclient);
    Accesses(const QJsonObject &json, RestClient *restclient);

    /* JSON */
    bool fromJson(const QJsonObject& json);
    QJsonObject toJson(void) const;

    bool get(int id);
    bool create(int id, QString pofType, int quantity, \
                QString ltType, int maxOnce);
    bool update(U_Accesses what, QVariant variable);
    bool remove();

    /* attributes get */
    int getId(void) const;
    const QString& getPeriodOfTime(void) const;
    int getQuantity(void) const;
    const QString& getLimitType(void) const;
    int getMaxOnce(void) const;

private:
    RestClient *m_restclient;
    QString m_path;

    int m_id;
    QString m_periodOfTime;
    int m_quantity;
    QString m_limitType;
    int m_maxOnce;
    QDateTime m_created;
    QDateTime m_modified;
    QString m_href;
};

#endif // ACCESSES_H
