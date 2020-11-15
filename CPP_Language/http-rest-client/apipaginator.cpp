#include "apipaginator.hpp"

ApiPaginator::ApiPaginator()
{
    m_countTotal = 0;
    m_countCurrent = 0;
    m_pageTotal = 0;
    m_pageCurrent = 0;
}

ApiPaginator::ApiPaginator(int countTotal,
             int countCurrent,
             int pageTotal,
             int pageCurrent)
{
    m_countTotal = countTotal;
    m_countCurrent = countCurrent;
    m_pageTotal = pageTotal;
    m_pageCurrent = pageCurrent;
}

int ApiPaginator::getCountTotal() const
{
    return m_countTotal;
}

int ApiPaginator::getCountCurrent() const
{
    return m_countCurrent;
}

int ApiPaginator::getPageTotal() const
{
    return m_pageTotal;
}

int ApiPaginator::getPageCurrent() const
{
    return m_pageCurrent;
}

#if 0
void ApiPaginator::dump()
{
    qDebug() << "countTotal:" << m_countTotal;
    qDebug() << "countCurrent:" << m_countCurrent;
    qDebug() << "pageTotal:" << m_pageTotal;
    qDebug() << "pageCurrent:" << m_pageCurrent;
}
#endif

bool ApiPaginator::isEmpty(void) const
{
    return (m_countTotal == 0);
}

void ApiPaginator::clear(void)
{
    m_countTotal = 0;
    m_countCurrent = 0;
    m_pageTotal = 0;
    m_pageCurrent = 0;
}

QDebug operator<< (QDebug d, const ApiPaginator &paginator) {
    d << "countTotal:" << paginator.getCountTotal();
    d << "countCurrent:" << paginator.getCountCurrent();
    d << "pageTotal" << paginator.getPageTotal();
    d << "pageCurrent" << paginator.getPageCurrent();

    return d;
}
