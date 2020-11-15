#include "apierror.hpp"

ApiError::ApiError()
{
    m_code = 0;
    m_message = "";
}


QString
ApiError::getMessage() const
{
    return m_message;
}

int
ApiError::getCode() const
{
    return m_code;
}

int
ApiError::getHttpStatusCode() const
{
    return m_httpStatusCode;
}

bool
ApiError::isEmpty() const
{
    return (m_message.isEmpty() && (m_code == 0));
}

void ApiError::setError(int httpStatusCode, QString method)
{
    m_httpStatusCode = httpStatusCode;
    m_code = 0;
    m_message.clear();
    m_message.append("Method: " + method);
}

void ApiError::setError(int httpStatusCode, QJsonObject json, QString method)
{
    m_message.clear();
    m_httpStatusCode = httpStatusCode;
    if(m_httpStatusCode == 422)
    {
        m_message.append("Method: " + method);
        m_code = json.value("code").toInt(0);
        QJsonObject errorObject = json.value("message").toObject();
        QStringList keys = errorObject.keys();
        int keysSize = keys.length(), index;
        for(index = 0; index < keysSize; index++)
        {
            QString key = keys.at(index);
            QString value = errorObject.value(key).toArray().at(0).toString();
            m_message.append(", ");
            m_message.append(key + ": [ " + value + " ]");
        }
    } else
    if(m_httpStatusCode == 404)
    {
        m_message.append("Method: " + method + " ");
        m_message.append(json.value("message").toString());
    } else
    {
        m_code = json.value("code").toInt(0);
        if(json.value("message").isNull())
            m_message.append(json.value("message").toString("No message received"));
        else
        {
            m_message.append("Method: "+ method + " ");
            m_message.append(json.value("message").toString());
        }
    }
}

void ApiError::appendMessage(QVariant WTF)
{
    QString wtf;
    if(WTF.type() == QVariant::String)
        wtf = " [Message :" + WTF.toString() + "] ";
    if(WTF.type() == QVariant::Int)
        wtf = " [Message :" + QString(WTF.toInt()) + "] ";
    m_message.append(" [Message :" + wtf + "] ");
}

void ApiError::clearMessage(void)
{
    m_message.clear();
}

QDebug operator<< (QDebug d, const ApiError &error) {
    d << "HTTP status code:" << error.getHttpStatusCode();
    d << "code:" << error.getCode();
    d << "message:" << error.getMessage();
    return d;
}
