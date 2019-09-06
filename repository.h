/**
 * Copyright (c) Tyler R. Suehr 2019. All rights reserved
 * 
 * Purpose:
 * This file defines a simple reusable CRUD repository.
 */

#ifndef _REPOSITORY_H_
#define _REPOSITORY_H_

// Defines a repository response code
#define REPOSITORY_OK 0
#define REPOSITORY_ERR 1
typedef unsigned char repo_rc;

/**
 * Defines a repository object.
 */
typedef struct repository {
    /**
     * Performs a query for an aggregate of results.
     * 
     * @param argc the number of results returned, or NULL
     * @param sql the SQL query to execute
     * @return the results, or NULL
     */
    void* (*queryAll)(int *argc, const char *sql);

    /**
     * Performs a query for a single result.
     * 
     * @param sql the SQL query to execute
     * @return the result, or NULL
     */
    void* (*query)(const char *sql);

    /**
     * Finds a single item using a unique identifier.
     * 
     * @param1 the unique identifier
     * @return the result, or NULL
     */
    void* (*findById)(void*);

    /**
     * Saves an item in the repository.
     * 
     * @param1 the item to be saved
     * @return the operation result code
     */
    repo_rc (*save)(void*);

    /**
     * Deletes an item in the repository.
     * 
     * @param1 the item to be removed
     * @return the operation result code
     */
    repo_rc (*remove)(void*);

    /**
     * Deletes an item in the repository using its unique identifier.
     * 
     * @param1 the item's unique identifer
     * @return the operation result code
     */
    repo_rc (*removeById)(void*);
} repository_t;

#endif