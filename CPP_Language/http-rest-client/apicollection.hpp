#ifndef APICOLLECTION_H
#define APICOLLECTION_H

#include "apipaginator.hpp"
#include <QJsonObject>
#include <QJsonArray>

class ApiCollection
{
public:
    ApiCollection(QJsonObject json);

    const QJsonArray &getData(void) const;
    const ApiPaginator &getPaginator(void) const;
    bool isEmpty(void) const;

private:
    QJsonArray m_data;
    ApiPaginator m_paginator;
};

#endif // APICOLLECTION_H
