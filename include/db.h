#pragma once
#include <sqlite3.h>
#include "panik.h"

int pk_db_open(const char* path, sqlite3* db);
void pk_db_close(sqlite3* db);

char* pk_db_latest(char* pkg, struct config config, sqlite3* db);
