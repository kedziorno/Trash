#include "applicationobject.h"

ApplicationItem::ApplicationItem()
{

}

ApplicationItem::ApplicationItem(const QJsonObject &jsonAppItemObj)
{
    m_accountStoreMappings = jsonAppItemObj.value("accountStoreMappings")\
            .toObject().value("href").toString();
    m_accounts = jsonAppItemObj.value("accounts")\
            .toObject().value("href").toString();
    m_apiKeys = jsonAppItemObj.value("apiKeys")\
            .toObject().value("href").toString();
    m_authTokens = jsonAppItemObj.value("authTokens")\
            .toObject().value("href").toString();
    m_createdAt = jsonAppItemObj.value("createdAt").toString();
    m_customData = jsonAppItemObj.value("customData")\
            .toObject().value("href").toString();
    m_defaultAccountStoreMapping = jsonAppItemObj.value("defaultGroupStoreMapping")\
            .toObject().value("href").toString();
    m_defaultGroupStoreMapping = jsonAppItemObj.value("defaultGroupStoreMapping")\
            .toObject().value("href").toString();
    m_description = jsonAppItemObj.value("description").toString();
    m_groups = jsonAppItemObj.value("groups")\
            .toObject().value("href").toString();
    m_href = jsonAppItemObj.value("href").toString();
    m_loginAttempts = jsonAppItemObj.value("loginAttempts")\
            .toObject().value("href").toString();
    m_modifiedAt = jsonAppItemObj.value("modifiedAt").toString();
    m_name = jsonAppItemObj.value("name").toString();
    m_oAuthPolicy = jsonAppItemObj.value("oAuthPolicy")\
            .toObject().value("href").toString();
    m_passwordResetTokens = jsonAppItemObj.value("passwordResetTokens")\
            .toObject().value("href").toString();
    m_status = jsonAppItemObj.value("status").toString();
    m_tenant = jsonAppItemObj.value("tenant")\
            .toObject().value("href").toString();
    m_verificationEmails = jsonAppItemObj.value("verificationEmails")\
            .toObject().value("href").toString();
}

void
ApplicationItem::dumpValues()
{
        qDebug() << m_accountStoreMappings;
        qDebug() << m_accounts;
        qDebug() << m_apiKeys;
        qDebug() << m_authTokens;
        qDebug() << m_createdAt;
        qDebug() << m_customData;
        qDebug() << m_defaultAccountStoreMapping;
        qDebug() << m_defaultGroupStoreMapping;
        qDebug() << m_description;
        qDebug() << m_groups;
        qDebug() << m_href;
        qDebug() << m_loginAttempts;
        qDebug() << m_modifiedAt;
        qDebug() << m_name;
        qDebug() << m_oAuthPolicy;
        qDebug() << m_passwordResetTokens;
        qDebug() << m_status;
        qDebug() << m_tenant;
        qDebug() << m_verificationEmails;
}

ApplicationObject::ApplicationObject()
{

}

ApplicationObject::ApplicationObject(const QJsonObject &jsonApplicationObj)
{
//    qDebug() << jsonApplicationObj;
    m_href = jsonApplicationObj.value("href").toString();
    m_limit = jsonApplicationObj.value("limit").toInt();
    m_offset = jsonApplicationObj.value("offset").toInt();
    m_size = jsonApplicationObj.value("size").toInt();

    QList<ApplicationItem> appItemsList;

    QJsonArray appItems = jsonApplicationObj.value("items").toArray();

    quint16 index;
    for(index = 0; index < m_size; index++)
    {
        QJsonObject appItemObj = appItems.at(index).toObject();

        ApplicationItem item(appItemObj);
        appItemsList.append(item);
    }

    m_appItems = appItemsList;

    dumpValues();
}

void
ApplicationObject::dumpValues()
{
    qDebug() << m_href;
    qDebug() << m_limit;
    qDebug() << m_offset;
    qDebug() << m_size;

    quint16 index;
    for(index = 0; index < m_size; index++)
    {
        m_appItems[index].dumpValues();
    }
}

