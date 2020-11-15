#ifndef APIERROR_H
#define APIERROR_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

class ApiError
{
public:
    ApiError(void);

    void setError(int httpStatusCode, QString method);
    void setError(int httpStatusCode, QJsonObject json, QString method);
    void appendMessage(QVariant WTF);
    QString getMessage(void) const;
    void clearMessage(void);
    int getCode(void) const;
    int getHttpStatusCode(void) const;

    bool isEmpty(void) const;

private:
    QString m_message;
    int m_code;
    int m_httpStatusCode;
};

QDebug operator<< (QDebug d, const ApiError &error);

#endif // APIERROR_H
