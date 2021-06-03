#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "query.h"

int showStock() {
	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns; /* number of columns in result-set */
    SQLUSMALLINT i;
    SQLCHAR x[128];
	
	char valor[256];
	char productCode[128];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
	}
	
	printf("\nINTRODUZCA EL Product Code DESEADO > ");
	scanf("%s", productCode);
	
	sprintf(valor, "SELECT quantityinstock FROM products WHERE productcode = '%s'", productCode);
    /* simple query */
    ret = SQLExecDirect(stmt, (SQLCHAR*)valor, SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
	
    /* How many columns are there */
    ret = SQLNumResultCols(stmt, &columns);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("Error getting number of columns", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* print the name of each column */
	
    for (i = 1; i <= (SQLUSMALLINT)columns; i++) {
        SQLCHAR buf[512];
        SQLDescribeCol(stmt, i, buf, sizeof(buf), NULL, NULL, NULL, NULL, NULL);
        printf("%s\t", buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, sizeof(x), NULL);
        printf("%s\t\n", x);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int showFind() {
	SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns; /* number of columns in result-set */
    SQLUSMALLINT i;
    SQLCHAR x[512];
    SQLCHAR y[512];
	
	char valor[256];
	char productName[32];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	
	printf("\nINTRODUZCA EL Nombre del producto DESEADO > ");
	scanf("%s", productName);
	
	sprintf(valor, "SELECT productcode, productname FROM products WHERE productname LIKE '%%%s%%'", productName);
    /* simple query */
    SQLExecDirect(stmt, (SQLCHAR*)valor, SQL_NTS);
    /* How many columns are there */
    SQLNumResultCols(stmt, &columns);

    /* print the name of each column */
    for (i = 1; i <= columns; i++) {
        SQLCHAR buf[512];
        SQLDescribeCol(stmt, i, buf, sizeof(buf), NULL, NULL, NULL, NULL, NULL);
        printf("%s\t", buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, sizeof(x), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, y, sizeof(y), NULL);
        printf("%s\t%s\n", x, y);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}