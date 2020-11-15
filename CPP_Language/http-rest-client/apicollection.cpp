#include "apicollection.hpp"

ApiCollection::ApiCollection(QJsonObject json)
{
    m_paginator = ApiPaginator(
                json.value("countTotal").toInt(),
                json.value("countCurrent").toInt(),
                json.value("pageTotal").toInt(),
                json.value("pageCurrent").toInt());
    if(json.value("items").isArray()) {
        m_data = json.value("items").toArray();
    }
}

const QJsonArray &ApiCollection::getData(void) const
{
    return m_data;
}

const ApiPaginator &ApiCollection::getPaginator(void) const
{
    return m_paginator;
}

bool ApiCollection::isEmpty(void) const
{
    return m_paginator.isEmpty() || m_data.isEmpty();
}
