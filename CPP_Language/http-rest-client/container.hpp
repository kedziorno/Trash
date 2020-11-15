#ifndef CONTAINER_H
#define CONTAINER_H

#include "restclient.hpp"
#include "user.hpp"

template <class T>
class Container
{
public:
    Container();
    Container(RestClient *rc, const QString &path);
    Container(RestClient *rc, const QString &path, int limit, const QString &expand);

    bool get(void);
    bool get(int id);

    bool create(T &object);

    bool update(T &object);

    bool remove(int id);

    QVector<T> getData(void);

private:
    bool get(const QString& path, int limit, const QString &expand);

    bool getAndAppend(int id);
    bool findObjectInContainer(int id, T &object);

    QVector<T> container;
    RestClient *m_rc;
    QString m_path;
    QString m_expand;
    int m_limit;
};

// contructor
template <class T>
Container<T>::Container(RestClient *rc, const QString &path, int limit, const QString &expand)
{
    m_rc = rc;
    m_path = path;
    m_limit = limit;
    m_expand = expand;
}

template <class T>
Container<T>::Container(RestClient *rc, const QString &path)
{
    m_rc = rc;
    m_path = path;
    m_limit = 100;
    m_expand = "";
}

// private methods
template <class T>
bool Container<T>::get(const QString &path, int limit, const QString &expand)
{
    QJsonArray jsonArray;

    QString realPath(path);
    realPath.append("?");

    if(limit)
    {
        QString limitStr = QString("&limit=%1").arg(limit);
        realPath.append(limitStr);
    }

    if(!expand.isEmpty())
    {
        QString expandStr = QString("&expand=%1").arg(expand);
        realPath.append(expandStr);
    }

    if(m_rc->get(realPath, jsonArray) == true)
    {
        if(m_rc->paginator().getPageTotal() == 1)
        {
            foreach(const QJsonValue itemObject, jsonArray)
                container.append(T(itemObject.toObject(), m_rc));
        } else {
            int pageTotal = m_rc->paginator().getPageTotal();
            int pageCurrent = m_rc->paginator().getPageCurrent();
            for( ; pageCurrent <= pageTotal; pageCurrent++)
            {
                QJsonArray pageObjects;
                QString pageStr = QString("%1&page=%2")\
                        .arg(realPath).arg(pageCurrent);
                if(m_rc->get(pageStr, pageObjects) == true)
                {
                    foreach(const QJsonValue itemObject, pageObjects)
                        container.append(T(itemObject.toObject(), m_rc));
                } else {
                    qDebug() << m_rc->lastError();
                    container.clear();
                    return false;
                }
            }
            return true;
        }
    } else {
        qDebug() << m_rc->lastError();
        return false;
    }

    return true;
}

template <class T>
bool Container<T>::findObjectInContainer(int id, T &object)
{
    T item;

    foreach(item, container)
    {
        if(item.getId() == id)
        {
            object = item;
            return true;
        }
    }

    return false;
}

template <class T>
bool Container<T>::getAndAppend(int id)
{
    T item;
    if(findObjectInContainer(id, item) == true)
    {
        qDebug() << "Find id:" << id << "in container.";
        return false;
    }
    QJsonObject jsonObject;
    QString realPath = QString("%1/%2").arg(m_path).arg(id);
    if(m_rc->get(realPath, jsonObject) == true)
    {
        item.create(jsonObject);
        container.append(item);
        qDebug() << "Adding id:" << id << "to container";
        return true;
    } else {
        qDebug()\
            << "Error: can't get id:" << id << "(" << m_rc->lastError() << ")";
        return false;
    }
}

template <class T>
bool Container<T>::get(void)
{
    if(get(m_path, m_limit, m_expand) == true)
        return true;
    return false;
}

template <class T>
bool Container<T>::get(int id)
{
    if(getAndAppend(id))
        return true;
    return false;
}

template <class T>
QVector<T> Container<T>::getData()
{
    return container;
}

template <class T>
bool Container<T>::remove(int id)
{
    T item;
    QString realPath = QString("%1/%2").arg(m_path).arg(id);
    if(m_rc->del(realPath) == true)
    {
        foreach(item, container) {
            if(item.getId() == id)
                qDebug() << "Found id" << id << "and deleted" << container.removeAll(item) << "items";
        }
        qDebug() << "User with id" << id << "deleted";
        return true;
    } else {
        qDebug() << m_rc->lastError();
        qDebug() << "User with id" << id << "not found and not deleted";
        return false;
    }
}

template <class T>
bool Container<T>::create(T &object)
{
    T item;

    foreach(item, container)
    {
//        qDebug() << "item:" << item.getId() << ":" << item.getLogin();
//        qDebug() << "item:" << item.getId() << ":" << item.getNumericLogin();
//        qDebug() << "object:" << object.getId() << ":" << object.getLogin();
//        qDebug() << "object:" << object.getId() << ":" << object.getNumericLogin();
        if(!item.getLogin().compare(object.getLogin())\
                || item.getNumericLogin() == object.getNumericLogin())
        {
            qDebug() << "Object with id:" << item.getId() << "existed.";
            return false;
        }
    }
    T newItem;
    QJsonObject objectToCreate = object.toJsonObject();
    if(m_rc->insert(m_path, objectToCreate) == true)
    {
        QJsonObject replyObject = m_rc->getReplyJsonDocument().object();
        int lastId = replyObject.value("id").toInt();
        newItem.create(replyObject);
        container.append(newItem);
        qDebug() << "Last inserted id:" << lastId << "to path:" << m_path;
        return true;
    } else {
        qDebug() << m_rc->lastError();
        qDebug() << "User existed or other error occurs.";
        return false;
    }

    return false;
}

template <class T>
bool Container<T>::update(T &object)
{
    Q_UNUSED(object);
    return false;
}


#endif // CONTAINER_H
