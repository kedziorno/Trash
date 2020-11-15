#include "database.h"

sqliteDB::sqliteDB(QObject *parent) :
    QObject(parent)
{
    DB = QSqlDatabase::addDatabase("QSQLITE");
}

void sqliteDB::setName(QString name)
{
    dbName = name;
}

QString sqliteDB::getName()
{
    return dbName;
}

void sqliteDB::openConnection()
{
    if(!dbName.isEmpty()) {
        DB.setDatabaseName(dbName);
        DB.open();
        if(DB.isOpen())
        {
            status = "Połączono z bazą";
        } else {
            status = "Brak połączenia z bazą";
        }
    } else {
        status = "Brak nazwy bazy danych";
    }
}

bool sqliteDB::isOpen()
{
    if(DB.isOpen())
        return true;
    else
    {
        status = "Brak połączenia z bazą";
        return false;
    }
}

void sqliteDB::closeConnection()
{
    if(DB.isOpen())
    {
        DB.close();
        status = "Zakończono połączenie z bazą";
    }
}

QSqlDatabase sqliteDB::getConnectionObject()
{
    return DB;
}

QString sqliteDB::getStatus()
{
    return status;
}

