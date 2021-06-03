#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "query.h"

int static mainMenu();

int static valor;

void static menuProducts();
void static menuOrders();
void static menuCustomers();

int static showMenuProducts();
int static showMenuOrders();
int static showMenuCustomers();

int main () {
	int choice = 0;
	
	while (choice != 4){
		choice = mainMenu();
		switch (choice){
			case 1:
				menuProducts();
				break;
			case 2:
				menuOrders();
				break;
			case 3:
				menuCustomers();
				break;
		}
	}
		
	return 0;
}

/* Menu principal del programa. Da a elegir entre las 3 opciones o salir. */
int mainMenu() {
	int choice = 0;
	
	while ((choice < 1) || (choice > 4)) {
		printf("\t******* MAIN MENU *******\n");
		printf("Choose one of the following options:\n\n");
		printf("(1)Products\n");
		printf("(2)Orders\n");
		printf("(3)Customers\n");
		printf("(4)Exit\n\n");
		printf("Enter a number that corresponds to your choice > ");
		if(scanf("%d", &choice) == 0){
			return -1;
		}
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> ERROR: Pls choose one of the following options.\n\n");
		}
	}
	
	return choice;
}

/* Menu que se encargara de mostrar las consultas de products. */
void menuProducts() {
	int choice = 0;
	
	while (choice != 3) {
		choice = showMenuProducts();
		switch (choice){
			case 1:
				valor = showStock();
				break;
			case 2:
				valor = showFind();
				break;
		}
	}
}

/* Funcion que muestra las posibles opciones de customers. */
int showMenuProducts() {
	int choice = 0;
	
	while ((choice < 1) || (choice > 4)) {
		printf("\t********** MENU PRODUCTS **********\n\n");
		printf("(1)Stock\n");
		printf("(2)Find\n");
		printf("(3)Back\n\n");
		printf("Enter a number that corresponds to your choice > ");
		if(scanf("%d", &choice) == 0){
			return -1;
		}
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> ERROR: Pls choose one of the following options.\n\n");
		}
	}
	
	return choice;
}

/* Menu que se encargara de mostrar las consultas de orders. */
void menuOrders() {
	int choice = 0;
	
	while (choice != 4) {
		choice = showMenuOrders();
		switch (choice){
			case 1:
				valor = showOpen();
				break;
			case 2:
				valor = showRange();
				break;
			case 3:
				valor = showDetail();
				break;
		}
	}
}

/* Funcion que muestra las posibles opciones de customers. */
int showMenuOrders() {
	int choice = 0;
	
	while ((choice < 1) || (choice > 4)) {
		printf("\t********** MENU ORDERS **********\n\n");
		printf("(1)Open\n");
		printf("(2)Range\n");
		printf("(3)Detail\n");
		printf("(4)Back\n\n");
		printf("Enter a number that corresponds to your choice > ");
		if(scanf("%d", &choice) == 0){
			return -1;
		}

		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> ERROR: Pls choose one of the following options.\n\n");
		}
	}
	
	return choice;
}

/* Menu que se encargara de mostrar las consultas de customers. */
void menuCustomers() {
	int choice = 0;
	
	while (choice != 4) {
		choice = showMenuCustomers();
		switch (choice){
			case 1:
				valor = showFindCustomer();
				break;
			case 2:
				valor = showProducts();
				break;
			case 3:
				valor = showBalance();
				break;
		}
	}
}

/* Funcion que muestra las posibles opciones de customers. */
int showMenuCustomers() {
	int choice = 0;
	
	while ((choice < 1) || (choice > 4)) {
		printf("\t********** MENU CUSTOMERS **********\n\n");
		printf("(1)Find\n");
		printf("(2)List products\n");
		printf("(3)Balance\n");
		printf("(4)Back\n\n");
		printf("Enter a number that corresponds to your choice > ");
		if(scanf("%d", &choice) == 0){
			return -1;
		}
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> ERROR: Pls choose one of the following options.\n\n");
		}
	}
	
	return choice;
}
