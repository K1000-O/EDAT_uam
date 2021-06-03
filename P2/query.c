#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "query.h"

/**
*	Método showStock. Devolveremos el número de unidades de un producto según el identificador que introduzca el usuario.
*
*/
int showStock() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    char x[128] = "%";
    char buf[512] = "\0";

    char valor[256] = "\0";
    char productCode[128]= "\0";

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

	/* Pedimos el product code que queremos consultar. */
	printf("\nIntroduce de productCode you want to search for > ");
	if(scanf("%s", productCode) == EOF) return -1;
	
	/* Introducimos en la variable valor la consulta con el pproduct code anteriormente introducido. */
	if(snprintf(valor,sizeof(valor), "SELECT quantityinstock FROM PRODUCTS WHERE productcode = '%s'", productCode) == 0) return -1;
	
	/* Pasamos la variable valor como consulta y obtenemos sus datos. */
    /* simple query */
    ret = SQLExecDirect(stmt, (SQLCHAR*)valor, SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
	
	/* Obtenemos el numero de columnas de la consulta. */
    ret = SQLNumResultCols(stmt, &columns);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("Error getting number of columns", stmt, SQL_HANDLE_STMT);
        return ret;
    }

	/* Imprimimos el nombre de cada columna ya puesto en la consulta. */
    for (i = 1; i <= (SQLUSMALLINT)columns; i++) {
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Realizamos un bucle por cada dato de la consulta e imprimos los datos. */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, (SQLLEN)sizeof(x), NULL);
        printf("%s\t", x);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
*	Metodo que devuelve un listado de productos que contengan una cadena pasada por teclado.
*
*/
int showFind() {
	SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    char x[512]= "\0";
    char y[512]= "\0";
    char buf[512] = "\0";
	
	char valor[256]= "\0";
	char productName[32]= "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    if(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt) == SQL_ERROR) return -1;
	
	printf("\nIntroduce de productCode you want to search for > ");
	if(scanf("%s", productName)==EOF)return -1;
	
	if(snprintf(valor,sizeof(valor), "SELECT productcode, productname FROM products WHERE productname LIKE '%%%s%%'", productName)==0) return -1;
    /* simple query */
    if(SQLExecDirect(stmt, (SQLCHAR*)valor, SQL_NTS)== SQL_ERROR) return -1;
    /* How many columns are there */
    if(SQLNumResultCols(stmt, &columns) == SQL_ERROR) return -1;

    /* print the name of each column */
    for (i = 1; (SQLSMALLINT)i <= (SQLSMALLINT)columns; i++) {
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, (SQLLEN)sizeof(x), NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, y, (SQLLEN)sizeof(y), NULL);
        printf("%s\t%s\n", (char*)x,(char*) y);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
*	Funcion que devuelve el listado de pedidos que aun no se han enviado.
*
*/
int showOpen() {
	SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    SQLINTEGER x = 0;
    char buf[512] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    if(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt)==SQL_ERROR) return -1;
	
    /* simple query */
    if(SQLExecDirect(stmt, (SQLCHAR*)"SELECT ordernumber FROM orders WHERE shippeddate IS NULL ORDER BY ordernumber", SQL_NTS)== SQL_ERROR) return -1;
    /* How many columns are there */
    if(SQLNumResultCols(stmt, &columns) == SQL_ERROR) return -1;

    /* print the name of each column */
    for (i = 1; (SQLSMALLINT)i <= (SQLSMALLINT)columns; i++) {
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_INTEGER, &x, x, NULL);
        printf("%d\n", x);
    }	

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
*	Funcion range que devuelve un listado de pedidos cuya fecha de pedido vaya entre dos dates.
*
*/
int showRange() {
	SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    SQLINTEGER x = 0;
    char y[256] = "\0";
    char z[256] = "\0";
    char buf[512] = "\0";

	char valor[256] = "\0";
	char fecha1[256]= "\0";
	char fecha2[256]= "\0";


    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    if(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt) == SQL_ERROR) return -1;

	printf("Enter two dates with '-' between them > \n");

	if(scanf("%s - %s", fecha1, fecha2)==EOF) return -1;

	if(snprintf(valor,sizeof(valor), "select ordernumber, orderdate, shippeddate from orders where (orderdate <= '%s' and orderdate >= '%s') or (orderdate >= '%s' and orderdate <= '%s') order by ordernumber", fecha1, fecha2, fecha1, fecha2)==0)return -1;

    /* simple query */
    if(SQLExecDirect(stmt, (SQLCHAR*)valor, SQL_NTS)== SQL_ERROR) return -1;
    /* How many columns are there */
    if(SQLNumResultCols(stmt, &columns) == SQL_ERROR) return -1;

    /* print the name of each column */
    for (i = 1; (SQLSMALLINT)i <= (SQLSMALLINT)columns; i++) {
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_INTEGER, &x, x, NULL);
	ret = SQLGetData(stmt, 2, SQL_C_CHAR, &y, (SQLLEN)sizeof(y), NULL);
	ret = SQLGetData(stmt, 3, SQL_C_CHAR, &z, (SQLLEN)sizeof(z), NULL);
        printf("%d\t%s\t%s\n", x, y, z);
    }	

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
*	Funcion que mostrara con detalle la informacion de un producto.
*
*/
int showDetail() {
	SQLHENV env = NULL;
    SQLHDBC dbc= NULL;
    SQLHSTMT stmt= NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns =0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    char x[256]= "\0";
    char y[256]= "\0";
    SQLINTEGER z = 0;
    char u[256]= "\0";
    SQLINTEGER v = 0;
    SQLINTEGER w = 0;
    char buf1[512] = "\0";
    char buf2[512] = "\0";
    char buf3[512] = "\0";

	char valor[256]= "\0";
	int ordernumber = 0;

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

	
	printf("\nEnter the ordernumber you want to look for >");
	if(scanf("%d", &ordernumber)==EOF)return -1;
	
	if(snprintf(valor, sizeof(valor), "select orderdate, status from orders where ordernumber = '%d'", ordernumber)==0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf1, (SQLSMALLINT)sizeof(buf1), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf1);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, (SQLLEN)sizeof(x), NULL);
	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 1");
       	 return EXIT_FAILURE;
    	}
		ret = SQLGetData(stmt, 2, SQL_C_CHAR, y, (SQLLEN)sizeof(y), NULL);
    	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 2");
        	return EXIT_FAILURE;
    	}
        printf("%s\t%s\t\n", x, y);
    }
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

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

	if(snprintf(valor,sizeof(valor), "select SUM(quantityordered*priceeach) from orderdetails where ordernumber = '%d'", ordernumber)==0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf2, (SQLSMALLINT)sizeof(buf2), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf2);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_INTEGER, &z, z, NULL);
	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 3");
       	 return EXIT_FAILURE;
    	}
        printf("%d\t\n", z);
    }
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

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

	if(snprintf(valor,sizeof(valor), "select productcode, quantityordered, priceeach from orderdetails where ordernumber = '%d' order by orderlinenumber ", ordernumber)==0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf3, (SQLSMALLINT)sizeof(buf3), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf3);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, &u, (SQLLEN)sizeof(u), NULL);
	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 4");
       	 return EXIT_FAILURE;
    	}
	ret = SQLGetData(stmt, 2, SQL_INTEGER, &v, v, NULL);
    	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 5");
        	return EXIT_FAILURE;
    	}
	ret = SQLGetData(stmt, 3, SQL_INTEGER, &w, w, NULL);
    	if (!SQL_SUCCEEDED(ret)) {
		printf("Error en 5");
        	return EXIT_FAILURE;
    	}
        printf("%s\t%d\t\t%d\t\n", (char*)u, v, w);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int showFindCustomer() {
	SQLHENV env =  NULL;
    SQLHDBC dbc=  NULL;
    SQLHSTMT stmt=  NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    SQLINTEGER w = 0;
    char x[128]= "\0";
    char y[128]= "\0";
    char z[128]= "\0";
    char buf[512] = "\0";

	
	char valor[256]= "\0";
	char cadena[128]= "\0";

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

	
	printf("\nEnter the string you of the contactfirstname > ");
	if(scanf("%s", cadena)==EOF) return -1;
	
	if(snprintf(valor,sizeof(valor), "select customernumber,customername, contactfirstname ,contactlastname from customers where contactfirstname like '%%%s%%' or contactlastname like '%%%s%%'", cadena, cadena) == 0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_INTEGER, &w, w, NULL);
        ret = SQLGetData(stmt, 2, SQL_C_CHAR, x, (SQLLEN)sizeof(x), NULL);
	ret = SQLGetData(stmt, 3, SQL_C_CHAR, y, (SQLLEN)sizeof(y), NULL);
	ret = SQLGetData(stmt, 4, SQL_C_CHAR, z, (SQLLEN)sizeof(z), NULL);
        printf("%d %s %s %s\n",w,(char*)x,(char*)y,(char*)z);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



int showProducts() {
    SQLHENV env =  NULL;
    SQLHDBC dbc =  NULL;
    SQLHSTMT stmt =  NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i = 0;
    char x[128]= "\0";
    SQLINTEGER y = 0;
    char buf[512] = "\0";

	
	char valor[512]= "\0";
	int customernumber = 0;

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

	
	printf("\nEnter the customernumber > ");
	if(scanf("%d", &customernumber) == EOF) return -1;
	
	if(snprintf(valor,sizeof(valor), "select p.productname, sum(o2.quantityordered) as suma from orders as o1, orderdetails as o2, products as p where o1.ordernumber = o2.ordernumber and o2.productcode = p.productcode and o1.customernumber = '%d' group by p.productname, p.productcode order by p.productcode", customernumber) == 0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_C_CHAR, x, (SQLLEN)sizeof(x), NULL);
	ret = SQLGetData(stmt, 2, SQL_INTEGER, &y, y, NULL);
        printf("%s %d\n", (char*)x,y);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int showBalance() {
	SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT i =0;
    SQLINTEGER x=0;
    char buf[512] = "\0";

	
	char valor[512] = "%";
	int customernumber = 0;

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

	
	printf("\nEnter the id of customernumber > ");
	if (scanf("%d", &customernumber)==EOF) return -1;
	
	if(snprintf(valor,sizeof(valor), "select ((sum(p.amount))-(sum(o2.priceeach*o2.quantityordered))) as BALANCE from payments as p, orders as o1, orderdetails as o2 where o1.ordernumber = o2.ordernumber and p.customernumber = o1.customernumber and o1.customernumber = '%d'", customernumber)==0) return -1;
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
        if(SQLDescribeCol(stmt, i, (SQLCHAR*)buf, (SQLSMALLINT)sizeof(buf), NULL, NULL, NULL, NULL, NULL)==SQL_ERROR) 			return-1;
        printf("%s\t", (char*)buf);
    }
    printf("\n");

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        ret = SQLGetData(stmt, 1, SQL_INTEGER, &x, x, NULL);
        printf("%d\t\n", x);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

