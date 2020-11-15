#ifndef APIPAGINATOR_H
#define APIPAGINATOR_H

#include <QDebug>

class ApiPaginator
{
public:
    ApiPaginator();
    ApiPaginator(int countTotal,
                 int countCurrent,
                 int pageTotal,
                 int pageCurrent);

    int getCountTotal(void) const;
    int getCountCurrent(void) const;
    int getPageTotal(void) const;
    int getPageCurrent(void) const;
    bool isEmpty(void) const;
    void clear(void);

private:
    int m_countTotal;
    int m_countCurrent;
    int m_pageTotal;
    int m_pageCurrent;
};

QDebug operator<< (QDebug d, const ApiPaginator &paginator);

#endif // APIPAGINATOR_H
