#include "db.h"
#include "panik.h"

int pk_db_open(const char* path, sqlite3* db) {
    if (sqlite3_open(path, &db) != SQLITE_OK) {
        return error("Sqlite open error");
    }
    return 0;
}

void pk_db_close(sqlite3* db) {
    sqlite3_close(db);
}

char* pk_db_latest(char* pkg, struct config config, sqlite3* db) {
    sqlite3_stmt* stmt;
    const char* sql =
        "SELECT file FROM packages WHERE name = ? AND arch = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        error("Statement prepare failure");
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, pkg, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, config.arch, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* ver = (const char*)sqlite3_column_text(stmt, 0);
    }
}
