#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include "sqlwrapper.h"

namespace App {
    SqlWrapper::SqlWrapper() {
        std::string dbName;
        sqlite3 *memDbCtx;
        int ret = -1;

        dbName = ":memory:";
        ret = sqlite3_open(dbName.c_str(), &memDbCtx);
        this->_curDbCtx = this->_memDbCtx = memDbCtx;

        if(ret == SQLITE_OK) {
            fprintf(stderr, "[%s] Open connection in constructor at %p\n", dbName.c_str(), this->_curDbCtx); // XXX:s3
            this->_vectorSqlite3Ctx.push_back(this->_curDbCtx);
        } else {
            fprintf(stderr, "[%s] Error in constructor: %s\n", dbName.c_str(), sqlite3_errmsg(this->_curDbCtx)); // XXX:s3
        }
    }

    void SqlWrapper::addDb(std::string &dbName) {
        if(!dbName.empty()) {
            sqlite3 *ctx;
            int ret = -1;

            ret = sqlite3_open(dbName.c_str(), &ctx); // XXX:s3
            this->_curDbCtx = ctx;
            if(ret == SQLITE_OK) {
                fprintf(stderr, "[%s] Open connection at %p\n", dbName.c_str(), this->_curDbCtx); // XXX:s3
                this->_vectorSqlite3Ctx.push_back(this->_curDbCtx);
            } else {
                showError();
            }
        } else {
            fprintf(stderr, "[%s] db name empty, switch to memory\n", __FUNCTION__);
            this->_curDbCtx = this->_memDbCtx;
        }
    }

    void SqlWrapper::createExampleDb(std::string &query, enum option what) {
        if(this->_curDbCtx) {
            int ret = -1;

            if(what == option::ALL_DB) {
                sqlite3 *tmp = this->_curDbCtx;
                fprintf(stderr, "Create examples in all databases\n");
                for(sqlite3 *i : this->_vectorSqlite3Ctx) {
                    this->_curDbCtx = i;
                    ret = sqlite3_exec(i, query.c_str(), NULL, NULL, NULL); // XXX:s3,toedit
                    if(ret == SQLITE_OK) {
                        if(i == this->_memDbCtx)
                            fprintf(stderr, "[%s] \"%s\"\n", ":memory:", query.c_str()); // XXX:s3
                        else
                            fprintf(stderr, "[%s] \"%s\"\n", sqlite3_db_filename(this->_curDbCtx, NULL), query.c_str()); // XXX:s3
                    } else {
                        this->showError();
                    }
                }
                this->_curDbCtx = tmp;
                this->getColumnNames();
                this->dumpColumnNames();
            } else
            if(what == option::ONE_DB){
                fprintf(stderr, "Create examples in one database\n");
                ret = sqlite3_exec(this->_curDbCtx, query.c_str(), NULL, NULL, NULL); // XXX:s3,toedit
                if(ret == SQLITE_OK) {
                    fprintf(stderr, "[%s] \"%s\"\n", sqlite3_db_filename(this->_curDbCtx, NULL), query.c_str()); // XXX:s3
                } else {
                    this->showError();
                }
                this->getColumnNames();
                this->dumpColumnNames();
            } else {
                fprintf(stderr, "[%s] Unknown option %d\n", __FUNCTION__, what);
            }
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
        }
    }


    int SqlWrapper::getNr(enum option what) {
        if(this->_curDbCtx) {
            sqlite3_stmt *stmt;
            int ret, count;
            std::string query;

            switch(what) {
                case option::NR_ROWS:
                    query = "SELECT COUNT(*) FROM demo1";
                    ret = sqlite3_prepare_v2(this->_curDbCtx, query.c_str(), query.length(), &stmt, 0);
                    if(ret == SQLITE_OK) {
                        int index = 0;
                        while(sqlite3_step(stmt) == SQLITE_ROW && index++ == 0) { // XXX:wtf-OnlyOneTime
                            count = sqlite3_column_int(stmt, 0);
                        }
                        sqlite3_finalize(stmt);
                        return count;
                    } else {
                        this->showError();
                        return 0;
                    }
                break;
                case option::NR_COLS:
                    query = "SELECT * FROM demo1 LIMIT 1"; // XXX:onerow
                    ret = sqlite3_prepare_v2(this->_curDbCtx, query.c_str(), query.length(), &stmt, 0);
                    if(ret == SQLITE_OK) {
                        count = sqlite3_column_count(stmt);
                        sqlite3_finalize(stmt);
                        return count;
                    } else {
                        this->showError();
                        return 0;
                    }
                break;
                default:
                    fprintf(stderr, "[%s] Unknown option %d\n", __FUNCTION__, what);
                    return 0;
            }
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
        }
        return 0;
    }

    void SqlWrapper::getAllTable() {
        if(this->_curDbCtx) {
            int nrrow, nrcol, icol, ret, columntype;
            std::string query, tuple;
            sqlite3_stmt *stmt;
            char buffer[32];

            query = "SELECT * FROM demo1";
            nrrow = this->getNr(this->option::NR_ROWS);
            nrcol = this->getNr(this->option::NR_COLS);
            ret = sqlite3_prepare_v2(this->_curDbCtx, query.c_str(), query.length(), &stmt, 0);
            if(ret == SQLITE_OK) {
                this->_table.clear();
                while(sqlite3_step(stmt) == SQLITE_ROW) {
                    std::vector<std::string> vColums; // Columns
                    for(icol = 0; icol < nrcol; icol++) {
                        columntype = sqlite3_column_type(stmt, icol);
                        if(columntype == SQLITE_INTEGER) {
                            sprintf(buffer, "%d", sqlite3_column_int(stmt, icol));
                        }
                        if(columntype == SQLITE_TEXT) {
                            sprintf(buffer, "%s", sqlite3_column_text(stmt, icol));
                        }
                        tuple = buffer;
                        vColums.push_back(tuple);
                    }
                    this->_table.push_back(vColums); // Rows
                }
                nrrow++;
            }
            sqlite3_finalize(stmt);
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
        }
    }

    void SqlWrapper::dumpAllTable() {
        if(this->_curDbCtx) {
            int nrrow = this->getNr(this->option::NR_ROWS);
            int nrcol = this->getNr(this->option::NR_COLS);

            std::vector<std::vector<std::string>> rows = this->_table;

            for(int irow = 0; irow < nrrow; irow++) {
                std::vector<std::string> cols = rows[irow];
                for(int icol = 0; icol < nrcol; icol++) {
                    std::string tuple = cols[icol];
                    std::cout << tuple << "\t";
                }
                std::cout << "\n";
            }
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
        }
    }

    std::string SqlWrapper::getTupleAtIndex(int row, int col, enum option what) {
        std::string tuple = "";
        if(this->_curDbCtx) {
            int nrrow, lrow, rrow, nrcol, lcol, rcol;

            nrrow = this->getNr(this->option::NR_ROWS);
            nrcol = this->getNr(this->option::NR_COLS);

            switch(what) {
                case option::FROM_ZERO:
                    lrow = 0;
                    lcol = 0;
                    rrow = nrrow - 1;
                    rcol = nrcol - 1;
                break;
                case option::NORMAL:
                    lrow = 1;
                    lcol = 1;
                    rrow = nrrow;
                    rcol = nrcol;
                break;
                default:
                    return tuple;
            }

            if((lrow <= row && row <= rrow) && (lcol <= col && col <= rcol)) {
                if(what == option::FROM_ZERO) {
                    tuple = this->_table[row][col];
                }
                if(what == option::NORMAL) {
                    tuple = this->_table[row-1][col-1];
                }
                return tuple;
            } else {
                fprintf(stderr, "[%s] Out of range (%d,%d), empty tuple\n", __FUNCTION__, row, col);
                return tuple;
            }
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
            return tuple;
        }
    }

    void SqlWrapper::getColumnNames() {
        if(this->_curDbCtx) {
            sqlite3_stmt *stmt;
            std::string query;
            int ret;

            query = "pragma table_info(demo1)";
            ret = sqlite3_prepare_v2(this->_curDbCtx, query.c_str(), query.length(), &stmt, 0);
            if(ret == SQLITE_OK) {
                this->_columns.clear();
                while(sqlite3_step(stmt) == SQLITE_ROW) {
                    char buffer[64];
                    snprintf(buffer, 64, "%s", sqlite3_column_text(stmt, 1));
                    this->_columns.push_back(buffer);
                }
            }
            sqlite3_finalize(stmt);
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
        }
    }

    std::string SqlWrapper::getColumnAt(int index) {
        if(this->_curDbCtx) { // XXX:??
            int cols = this->getNr(this->option::NR_COLS);
            if(0 <= index && index < cols) {
                return this->_columns[index];
            }
            return "";
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
            return "";
        }
    }

    void SqlWrapper::closeAllConnections() {
        const char *buffer;
        for(sqlite3 *i : this->_vectorSqlite3Ctx) {
            buffer = sqlite3_db_filename(i, NULL);
            int ret = sqlite3_close(i); // XXX:s3
            if(ret == SQLITE_OK) {
                fprintf(stderr, "[%s] Close connection with sqlite* at %p\n", buffer, i); // XXX:s3
            } else {
                this->showError();
            }
        }
    }

    void SqlWrapper::connectionInfo() {
        getDbStatus(SQLITE_DBSTATUS_SCHEMA_USED);
        getDbStatus(SQLITE_DBSTATUS_STMT_USED);
        getDbStatus(SQLITE_DBSTATUS_LOOKASIDE_HIT);
        getDbStatus(SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE);
        getDbStatus(SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL);
        getDbStatus(SQLITE_DBSTATUS_CACHE_HIT);
        getDbStatus(SQLITE_DBSTATUS_CACHE_MISS);
        getDbStatus(SQLITE_DBSTATUS_CACHE_WRITE);
        getDbStatus(SQLITE_DBSTATUS_DEFERRED_FKS);
        getDbStatus(SQLITE_DBSTATUS_CACHE_USED_SHARED);
    }

    void SqlWrapper::setContext(std::string &dbName) {
        sqlite3 *i = findContextByDatabaseName(dbName);
        if(i) {
            fprintf(stderr, "[%s] Set to new database %s\n", sqlite3_db_filename(this->_curDbCtx, NULL), dbName.c_str());
            this->_curDbCtx = i;
        } else {
            fprintf(stderr, "[%s] Database name %s not found, switch to memory\n", sqlite3_db_filename(this->_curDbCtx, NULL), dbName.c_str());
            this->_curDbCtx = this->_memDbCtx;
        }
    }

    sqlite3* SqlWrapper::getContext() {
        if(this->_curDbCtx) {
            return this->_curDbCtx;
        } else {
            fprintf(stderr, "[%s] Current db ctx NULL\n", __FUNCTION__);
            return NULL;
        }
    }

    int SqlWrapper::callbackNrRows(void *nu, int argc, char **argv, char**azColName) {
        (void)nu;
        (void)azColName;

        int ret;

        if(argc == 1) {
            std::cout<<"c1\n";
            ret = std::stoi(argv[0]); //sprintf(ret, "%d", argv[0]);
            (void)ret;
            return 0;
        } else {
            std::cout<<"c2\n";
            fprintf(stderr, "Function %s: argc != 1 (%d)\n", __FUNCTION__, argc);
            return 1;
        }
    }

    int SqlWrapper::callback1(void *nu, int argc, char **argv, char**azColName) {
        (void)nu;

        for (int i = 0; i < argc; i++) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");

        return 0;
    }

    // may be private ?

    void SqlWrapper::showError() {
        sqlite3 *db = this->_curDbCtx;
        fprintf(stderr, "[%s] Error: %s\n", sqlite3_db_filename(db, NULL), sqlite3_errmsg(db)); // XXX:s3
    }

    void SqlWrapper::getDbStatus(int flag) {
        int val = -1, hi = -1;
        int ret = sqlite3_db_status(this->_curDbCtx, flag, &val, &hi, false); // XXX:s3
        if(ret == SQLITE_OK) {
            fprintf(stderr, "[%s] flag 0x%X: val=0x%X, hi=0x%X\n", sqlite3_db_filename(this->_curDbCtx, NULL), flag, val, hi); // XXX:s3
        } else {
            this->showError();
        }
    }

    sqlite3* SqlWrapper::findContextByDatabaseName(std::string &dbName) {
        sqlite3 *i;
        if(!dbName.empty()) {
            int index, vCtxSize = this->_vectorSqlite3Ctx.size();
            for(index = 0; index < vCtxSize; index++) {
                i = this->_vectorSqlite3Ctx[index];
                std::string dbFileName(sqlite3_db_filename(i, NULL)); // XXX:s3
                if(dbFileName.empty())
                    continue;
                const char *contextDatabaseName = strrchr(dbFileName.c_str(), '/') + 1; // XXX:+1
                if(strncmp(dbName.c_str(), contextDatabaseName, dbName.length()) == 0) {
                    return i;
                }
            }
        }
        return NULL;
    }

    void SqlWrapper::insertData(std::vector<std::string> &data) {
        // XXX:count(data)==count(nrcols)
        int sizeData = data.size();
        int collen = this->getNr(this->option::NR_COLS);
        int ret;
        if(sizeData <= collen) { //XXX:magicbugdbhrhrhr:>
            sqlite3_stmt *stmt;
            int index;
            std::string query;
            std::string values;
            query = "INSERT INTO demo1 (";
            for(index = 0; index < sizeData; index++) { // XXX:iddatabase
                std::string columnName = this->getColumnAt(index+1);
                std::string value = data[index];
                query += columnName;
                values += "'" + value + "'";
                if(!(index == sizeData-1)) {
                    values += ",";
                    query += ",";
                } else {
                    query += ") ";
                }
            }
            query += "VALUES (" + values + ")";
            std::cout << __FUNCTION__ << "  " << query << "\n";
            ret = sqlite3_prepare_v2(this->_curDbCtx, query.c_str(), query.length(), &stmt, NULL);
            if(ret == SQLITE_OK) {
                ret = sqlite3_step(stmt);
                if(ret == SQLITE_DONE) {
                    fprintf(stderr, "[%s] Last inserted id = %lli\n", sqlite3_db_filename(this->_curDbCtx, NULL), sqlite3_last_insert_rowid(this->_curDbCtx)); // XXX:s3
                    sqlite3_finalize(stmt);
                }
            }
        } else {
            fprintf(stderr, "[%s] Data is empty\n", sqlite3_db_filename(this->_curDbCtx, NULL)); // XXX:s3
        }
    }
}

