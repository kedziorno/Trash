#ifndef APPLICATIONOBJECT_H
#define APPLICATIONOBJECT_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class ApplicationItem
{
public:
    explicit ApplicationItem();
    ApplicationItem(const QJsonObject &jsonAppItemObj);

    void dumpValues(void);

private:
    QString m_accountStoreMappings;
    QString m_accounts;
    QString m_apiKeys;
    QString m_authTokens;
    QString m_createdAt;
    QString m_customData;
    QString m_defaultAccountStoreMapping;
    QString m_defaultGroupStoreMapping;
    QString m_description;
    QString m_groups;
    QString m_href;
    QString m_loginAttempts;
    QString m_modifiedAt;
    QString m_name;
    QString m_oAuthPolicy;
    QString m_passwordResetTokens;
    QString m_status;
    QString m_tenant;
    QString m_verificationEmails;
};

class ApplicationObject
{
public:
    explicit ApplicationObject();
    ApplicationObject(const QJsonObject &jsonApplicationObj);

    void                dumpValues();

    QString             getHref();
    ApplicationItem     getApplicationItem(quint16 offset);
    quint16             getLimit();
    quint16             getOffset();
    quint16             getSize();

private:
    QString m_href;
    QList<ApplicationItem> m_appItems;
    quint16 m_limit;
    quint16 m_offset;
    quint16 m_size;
};

#endif // APPLICATIONOBJECT_H
