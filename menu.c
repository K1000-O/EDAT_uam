#include <stdio.h>
#include <stdlib.h>
//#include "odbc.h"
#include "query.h"

int mainMenu();

void menuProducts();
void menuOrders();
void menuCustomers();

int showMenuProducts();
int showMenuOrders();
int showMenuCustomers();

int main (int argc, char* argv[]) {
	int choice = 0;
	
	do {
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
	} while (choice != 4);
		
	return 0;
}

int mainMenu() {
	int choice = 0;
	
	do {
		printf("\t########### MAIN MENU ###########\n");
		printf("Por favor seleccione una de las siguientes opciones:\n\n");
		printf("(1) Products\n(2) Orders\n(3) Customers\n(4) Exit\n\n");
		printf("Introduzca un valor de la lista > ");
		scanf("%d", &choice);
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> Error en la entrada de parametros. Por favor seleccione un numero del listado\n\n");
		}
	} while ((choice < 1) || (choice > 4));
	
	return choice;
}

void menuProducts() {
	int choice = 0;
	
	do {
		choice = showMenuProducts();
		switch (choice){
			case 1:
				showStock();
				break;
			case 2:
				showFind();
				break;
		}
	} while (choice != 3);
}

int showMenuProducts() {
	int choice = 0;
	
	do {
		printf("\t********** MENU PRODUCTS **********\n\n");
		printf("(1) Stock\n(2) Find\n(3) Back\n\n");
		printf("Introduzca un valor de la lista > ");
		scanf("%d", &choice);
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> Error en la entrada de parametros. Por favor seleccione un numero del listado\n\n");
		}
	} while ((choice < 1) || (choice > 4));
	
	return choice;
}

void menuOrders() {
	int choice = 0;
	
	do {
		choice = showMenuOrders();
		switch (choice){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		}
	} while (choice != 4);
}

int showMenuOrders() {
	int choice = 0;
	
	do {
		printf("\t********** MENU ORDERS **********\n\n");
		printf("(1) Open\n(2) Range\n(3) Detail\n(4) Back\n\n");
		printf("Introduzca un valor de la lista > ");
		scanf("%d", &choice);
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> Error en la entrada de parametros. Por favor seleccione un numero del listado\n\n");
		}
	} while ((choice < 1) || (choice > 4));
	
	return choice;
}

void menuCustomers() {
	int choice = 0;
	
	do {
		choice = showMenuCustomers();
		switch (choice){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		}
	} while (choice != 4);
}

int showMenuCustomers() {
	int choice = 0;
	
	do {
		printf("\t********** MENU CUSTOMERS **********\n\n");
		printf("(1) Find\n(2) List products\n(3) Balance\n(4) Back\n\n");
		printf("Introduzca un valor de la lista > ");
		scanf("%d", &choice);
		printf("\n");
		if ((choice < 1) || (choice > 4)) {
			printf(">> Error en la entrada de parametros. Por favor seleccione un numero del listado\n\n");
		}
	} while ((choice < 1) || (choice > 4));
	
	return choice;
}