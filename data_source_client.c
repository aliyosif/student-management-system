#include "data_source_client.h"

static const int SQL_QUERY_BUFFER_SIZE = 200;

static sqlite3 *mainDb = NULL;
static int mainVersion = 0;


/**
 * Gets the current user version of the database.
 * 
 * @param db the database to get version for
 * @return the version, or 0
 */
static int local_get_version(sqlite3 *db) {
    int rc, version = 0;
    char *sql = "PRAGMA USER_VERSION;";
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nFailed to execute query: %s", sqlite3_errmsg(db));
        return version;
    }

    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) {
        version = sqlite3_column_int(res, 0);
    }

    sqlite3_finalize(res);
    return version;
}

/**
 * Sets the current user version of the database.
 * 
 * @param db the database to set version for
 * @param version the version to be set
 */
static void local_set_version(sqlite3 *db, const int version) {
    char sql[40];
    snprintf(sql, 30, "PRAGMA USER_VERSION=%d;", version);

    int rc;
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nFailed to execute statement: %s", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(res, 1, version);
    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("\nFailed to update version number!");
    }

    sqlite3_finalize(res);
}

static int local_exec_stmt(sqlite3 *db, const char *sql) {
    int rc;
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nFailed to compile SQL statement: %s", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("\nFailed to evaluate SQL statement: %s", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    return rc;
}

static int schema_create(sqlite3 *db) {
    int rc;
    sqlite3_stmt *res;

    char sql[SQL_QUERY_BUFFER_SIZE];
    memset(sql, 0, SQL_QUERY_BUFFER_SIZE);
    
    // Create the `accounts` table
    snprintf(sql, SQL_QUERY_BUFFER_SIZE, 
        "CREATE TABLE [%s]("\
            "[%s] INTEGER PRIMARY KEY AUTOINCREMENT,"\
            "[%s] TEXT UNIQUE NOT NULL,"\
            "[%s] TEXT);",
        TABLE_ACCOUNTS_NAME, TABLE_ACCOUNTS_ID,
        TABLE_ACCOUNTS_USER, TABLE_ACCOUNTS_PASS);

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nUnable to create accounts table! %s", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("\nFailed to create accounts table!");
    }

    // Create the `students` table
    snprintf(sql, SQL_QUERY_BUFFER_SIZE,
        "CREATE TABLE [%s]("\
            "[%s] INTEGER PRIMARY KEY AUTOINCREMENT,"\
            "[%s] TEXT NOT NULL,"\
            "[%s] TEXT NOT NULL,"\
            "[%s] REAL NOT NULL DEFAULT 0.0);",
        TABLE_STUDENTS_NAME, TABLE_STUDENTS_ID, 
        TABLE_STUDENTS_FIRST, TABLE_STUDENTS_LAST,
        TABLE_STUDENTS_GPA
        );

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nUnable to create students table! %s", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("\nFailed to create students table!");
    }

    sqlite3_finalize(res);
    return SQLITE_OK;
}

sqlite3* dsc_client() {
    if (!mainDb) {
        printf("\nClient has not been initialized yet!");
    }
    return mainDb;
}

int dsc_init(const char *name, const int ver) {
    int rc;

    if (mainDb) {
        printf("\nClient already initialized!");
        return SQLITE_ERROR;
    }

    rc = sqlite3_open(name, &mainDb);
    if (rc != SQLITE_OK) {
        printf("\nCould not open database! %s", sqlite3_errmsg(mainDb));
        return rc;
    }

    mainVersion = local_get_version(mainDb);
    if (mainVersion < ver) {
        rc = schema_create(mainDb);
        if (rc == SQLITE_OK) {
            local_set_version(mainDb, ver);
            mainVersion = ver;
        }
    }

    return rc;
}

int dsc_terminate() {
    if (mainDb) {
        sqlite3_close(mainDb);
        mainDb = NULL;
        return SQLITE_OK;
    }
    return SQLITE_ERROR;
}

int dsc_version() {
    if (!mainDb) {
        printf("\nClient has not been initialized yet!");
    }
    return mainVersion;
}

int dsc_exec(const char *sql) {
    if (!mainDb) {
        return SQLITE_ERROR;
    }

    int rc;
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(mainDb, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nError compiling SQL statement: %s", sqlite3_errmsg(mainDb));
        return rc;
    }

    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("\nError executing SQL statement: %s", sqlite3_errmsg(mainDb));
    }

    sqlite3_finalize(res);
    return rc;
}

int dsc_query(const char *sql, void* (*map)(sqlite3_stmt*), void *outData) {
    if (!mainDb) { 
        return SQLITE_ERROR;
    }

    int rc;
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(mainDb, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("\nError compiling SQL query: %s", sqlite3_errmsg(mainDb));
        return rc;
    }
    
    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) {
        outData = (*map)(res);
    }

    sqlite3_finalize(res);
    return rc;
}