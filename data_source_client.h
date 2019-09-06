/**
 * Copyright (c) Tyler R. Suehr 2019. All rights reserved
 * 
 * Purpose:
 * (n/a)
 * 
 * Expectations:
 * It is expected that the system in which this program runs represents
 * the school in which the students attend. Therefore, aggregating multiple
 * schools is pointless... it can be simple stored in a file. It is therefore
 * assumed that the school stored in a file, not the database, is the school
 * in which all students in the database are attending.
 */

#ifndef _DATA_SOURCE_CLIENT_H_
#define _DATA_SOURCE_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

#define MAX_CRED_LEN 45
#define MAX_NAME_LEN 65

// Definitions for `accounts` table
#define TABLE_ACCOUNTS_NAME "accounts"
#define TABLE_ACCOUNTS_ID "acc_id"
#define TABLE_ACCOUNTS_USER "acc_user"
#define TABLE_ACCOUNTS_PASS "acc_pass"

// Definitions for `students` table
#define TABLE_STUDENTS_NAME "students"
#define TABLE_STUDENTS_ID "st_id"
#define TABLE_STUDENTS_FIRST "st_first"
#define TABLE_STUDENTS_LAST "st_last"
#define TABLE_STUDENTS_GPA "st_gpa"

#define CLIENT_OK  0 // Denotes a successful response code
#define CLIENT_ERR 1 // Denotes an error response code
typedef unsigned char client_rc; // Defines a client response code type

/**
 * Defines the data model for an employee user account.
 */
typedef struct account {
    int id;                      // Account unique identifier
    char username[MAX_CRED_LEN]; // Username credential of account
    char password[MAX_CRED_LEN]; // Password credential of account
} account_t;

/**
 * Defines data model for a school facility.
 */
typedef struct school {
    int id;                  // School unique identifier
    char name[MAX_NAME_LEN]; // Name of the school
} school_t;

/**
 * Defines data model for a student attending a school.
 */
typedef struct student {
    int id;                       // Student unique identifier
    char firstName[MAX_NAME_LEN]; // First name of the student
    char lastName[MAX_NAME_LEN];  // Last name of the student
    float gpa;                    // Student's overall GPA
} student_t;

sqlite3* dsc_client();
int dsc_init(const char *name, const int ver);
int dsc_terminate();
int dsc_version();
int dsc_exec(const char *sql);
int dsc_query(const char *sql, void* (*map)(sqlite3_stmt*), void *outData);

#endif