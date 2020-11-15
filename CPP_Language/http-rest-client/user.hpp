#ifndef USER_HPP
#define USER_HPP

#include <QString>
#include <QSet>
#include <QVector>
#include <QDateTime>

#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

#include <QCryptographicHash>
#include <QIntegerForSize>

#include "restclient.hpp"
#include "tag.hpp"

/*
    User:
    {
        "id": 10981,
        "externalKey": "deadbeaf",
        "firstName": "Jan",
        "secondName": "Wiesław",
        "lastName": "Kowalski",
        "login": "janko",
        "numericLogin": 99999,
        "password": "passs",
        "pin": 1234,
        "validFrom": "2015-01-31",
        "validTo": "2015-02-01",
        "created": "2015-01-01",
        "modified": "2015-01-01",
        "privileges": 3,
        "language": "PL",
        "accessId": [ 1 ],
        "costCenterId": [ 3 ],
        "tagId": [ 1023, 99932 ],
        "supplierId": [ 1, 2, 3 ],
    }
*/

typedef enum WhatUpdate {
    U_externalKey = 1,
    U_firstName,
    U_secondName,
    U_lastName,
    U_login,
    U_numericLogin,
    U_privileges,
    U_pin,
    U_validDateFromAndTo,
    U_language
} WhatUpdate;

class User
{
public:
    enum PrivilageFlag{
        NORMAL_GET = 1 << 0,
        RETURN     = 1 << 1,
        SERVICE    = 1 << 2
    };

    User();
    User(RestClient *restclient);
    User(const QJsonObject &json, RestClient *restclient);

    /* JSON */
    bool fromJson(const QJsonObject& json);
    QJsonObject toJson(void) const;

    bool get(int id);
    int create(const QString& first_name,
               const QString& last_name,
               int privileges,
               const QString& language,
               const QString& login,
               int numeric_login,
               const QString& password,
               int pin,
               const QString& valid_from,
               const QString& external_key);
    bool update(WhatUpdate what, QVariant variable);
    bool remove(void);

    /* attributes get */
    int getId(void) const;
    const QString& getExternalKey(void) const;
    const QString& getFirstName(void) const;
    const QString& getSecondName(void) const;
    const QString& getLastName(void) const;
    const QString getLogin(void) const;
    int getNumericLogin(void) const;
    const QString getPin(void) const;
    const QString getValidFrom(void) const;
    const QString getValidTo(void) const;
    const QString getCreatedTime(void) const;
    const QString getModifiedTime(void) const;
    int getPrivileges(void) const;
    const QString& getLanguage(void) const;
    const QSet<int>& getAccessId(void) const;
    const QSet<int>& getCostCentersId(void) const;
    const QSet<int>& getTagsId(void) const;
    const QSet<int>& getSuppliersId(void) const;
    const QString& getObjectHref(void) const;
    const QString& getTagsHref(void) const;

    /* attributes set */
    bool setExternalKey(QString key);
    bool setFirstName(QString fname);
    bool getSecondName(QString sname);
    bool setLastName(QString lname);
    bool setLogin(QString login);
    bool setNumericLogin(int numericLogin);
    bool setPin(int pin);
    bool setValidFrom(QString dateTime);
    bool setValidTo(QString dateTime);
    bool setPrivileges(int flags);
    bool setLanguage(QString language);
//    const QSet<int>& setAccessId(void);
//    const QSet<int>& setCostCentersId(void);
//    const QSet<int>& setTagsId(void);
//    const QSet<int>& setSuppliersId(void);

    /* Tags */
    QList<Tags> getTags(void) const;
    bool insertTag(Tags &tagObject);
    bool insertTag(QJsonObject &json);
    bool updateTags(void);

    /* Validation */
    bool isValid(void) const;

    /* operators */
    bool operator==(const User& other) const;
    bool operator!=(const User& other) const;
    bool operator<(const User& other) const;
    bool operator>(const User& other) const;

private:
    bool updateVariable(QJsonObject variable);

    RestClient *m_restclient;
    QString m_path;

    int m_id;

    QString m_externalkey;

    QString m_firstname;
    QString m_secondname;
    QString m_lastname;

    QString m_login;
    int m_numericLogin;
    QString m_password;
    QVariant m_pin;

    QDateTime m_validfrom;
    QDateTime m_validto;
    QDateTime m_created;
    QDateTime m_modified;

    int m_privileges;
    QString m_language;

    QSet<int> m_accessIds;
    QSet<int> m_costCentersIds;
    QSet<int> m_tagsIds;
    QSet<int> m_suppliersIds;

    QString m_objHref;
    QString m_tagHref;

    QList<Tags> m_tags;

};

QDebug operator<< (QDebug d, const User& user);

#endif // USER_HPP
