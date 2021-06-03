#include "utils.h"

void imprimirIndiceMenu();

int main (void) {
	char caso[7] = "\0";
	char fichero[32] = "\0";
	int use = 0, offset = 0;
	long leaf = 0;
	char book_id[PK_SIZE] = "/0";
	bool status = false;
	
	do {
		imprimirIndiceMenu();
		
		fprintf(stdout, ">>> "); 
		scanf("%s", caso);
		
		if (!strcmp(caso, "use") || !strcmp(caso, "u")) {
			use = 1;
			fprintf(stdout, "Introduzca el nombre del fichero.\n");
			fprintf(stdout, ">>> "); 
			scanf ("%s", fichero);
			
			status = createTable(fichero);
			
			if (status == false) {
				fprintf(stderr, "Error al crear la tabla.\n\n");
			}
		} else if (!strcmp(caso, "insert") || !strcmp(caso, "i")) {
			if (!use) {
				fprintf(stdout, "Tiene que acceder primero al comando 'use'.\n");
			} else {
				fprintf(stdout, "Introduzca el nombre del nodo a insertar (TAMANIO == 4).\n");
				fprintf(stdout, ">>> "); 
				scanf("%s", book_id);
				
				fprintf(stdout, "Introduzca el offset del producto.\n");
				fprintf(stdout, ">>> "); 
				scanf("%d", &offset);
				
				status = addIndexEntry(book_id, offset, fichero);
				
				if (status == false) {
					fprintf(stderr, "Error en la introducción del fichero.\n\n");
				}
			}
		} else if (!strcmp(caso, "print") || !strcmp(caso, "p")) {
			if (!use) {
				fprintf(stdout, "Tiene que acceder primero al comando 'use'.\n");
			} else {
				fprintf(stdout, "Introduzca el numero de hojas a imprimir.\n");
				fprintf(stdout, ">>> "); 
				scanf("%ld", &leaf);
				
				printTree(leaf, fichero);
			}
		} 
		
		status = false;
	} while (strcmp(caso, "exit") != 0 && strcmp(caso, "e") != 0);
	
	return 0;
}

/**
 * @brief Metodo que imprime el indice del menu de la practica 3.
 * 
 */
void imprimirIndiceMenu() {
	fprintf(stdout, "\nEscriba uno de los siguientes comandos.\n\n");
	fprintf(stdout, "****** MENU P3 ******\n");
	fprintf(stdout, "\t1. use\n");
	fprintf(stdout, "\t2. insert\n");
	fprintf(stdout, "\t3. print\n");
	fprintf(stdout, "\t4. exit\n");
}