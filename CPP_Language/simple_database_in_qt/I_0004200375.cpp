#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H

#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>

namespace App {
    class SqlWrapper
    {
        public:
            enum option {
                NORMAL = -1,
                FROM_ZERO = 0,
                ALL_DB = 1,
                ONE_DB = 2,
                NR_ROWS = 3,
                NR_COLS = 4
            };

            SqlWrapper(); // in memory

            void            addDb(std::string &dbName);
            void            setContext(std::string &dbName);
            void            createExampleDb(std::string &query, enum option what);
            void            getDbStatus(int flag);
            void            showError();
            void            connectionInfo();
            void            closeAllConnections();
            void            getAllTable();
            void            dumpAllTable();
            void            getColumnNames();
            void            insertData(std::vector<std::string> &data);
            int             getNr(enum option what);
            std::string     getTupleAtIndex(int row, int col, enum option what);
            std::string     getColumnAt(int index);
            sqlite3*        findContextByDatabaseName(std::string &dbName);
            sqlite3*        getContext();

            void            dumpColumnNames() {
                std::cout << "columnnames";
                for(std::string s : this->_columns)
                    std::cout << " (" << s << ")";
                std::cout << "\n";
            }

            static int      callbackNrRows(void *nu, int argc, char **argv, char**azColName);
            static int      callback1(void *nu, int argc, char **argv, char **azColName);

        private:
            std::vector<sqlite3*>                   _vectorSqlite3Ctx;
            sqlite3*                                _curDbCtx;
            sqlite3*                                _memDbCtx;
            std::vector<std::vector<std::string>>   _table;
            std::vector<std::string>                _columns;
    };
};

#endif // SQLWRAPPER_H

