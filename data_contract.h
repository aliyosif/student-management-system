/**
 * Copyright (c) Tyler R. Suehr 2019. All rights reserved
 * 
 * Purpose:
 * This files contains the definitions of the systems data models.
 * 
 * Expectations:
 * It is expected that the system in which this program runs represents
 * the school in which the students attend. Therefore, aggregating multiple
 * schools is pointless... it can be simple stored in a file. It is therefore
 * assumed that the school stored in a file, not the database, is the school
 * in which all students in the database are attending.
 */

#define MAX_CRED_LEN 45
#define MAX_NAME_LEN 65

// Definitions for `accounts` table
#define TABLE_ACCOUNT_ID "acc_id"
#define TABLE_ACCOUNT_USER "acc_user"
#define TABLE_ACCOUNT_PASS "acc_pass"

// Definitions for `students` table
#define TABLE_STUDENTS_ID "st_id"
#define TABLE_STUDENTS_FIRST "st_first"
#define TABLE_STUDENTS_LAST "st_last"
#define TABLE_STUDENTS_GPA "st_gpa"

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