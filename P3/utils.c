#include "utils.h"

#define tamNode 1000

/**
 * @brief Metodo que crea la tabla de datos con la que trabajaremos mas tarde.
 * 
 * @param tableName nombre del fichero en la que se guardan las tablas.
 * @return true 
 * @return false 
 */
bool createTable(const char* tableName) {
	FILE* fichero = NULL;
	int ini = -1;
	char nameAux[100] = "\0";
	
	if (!tableName) return false;
	else strcpy(nameAux, tableName);
	
	if (!(fichero = fopen(tableName, "r"))) {
		if (!(fichero = fopen(nameAux, "w"))) return false;
		
		fwrite (&ini, DATA_HEADER_SIZE, 1, fichero);
	}
	
	fclose(fichero);
	
	createIndex(tableName);
	return true;
}

/**
 * @brief Funcion que crea el fichero index. Cambia la extension a .idx
 * 
 * @param indexName nombre del fichero al que cambiarle la extensión.
 * @return true 
 * @return false 
 */
bool createIndex (const char* indexName) {
	FILE* fichero = NULL;
	int ini = -1;
	char nameAux[100] = "\0";
	
	if (!indexName) return false;
	else replaceExtensionByIdx(indexName, nameAux);
	
	if (!(fichero = fopen(nameAux, "r"))) {
		if (!(fichero = fopen(nameAux, "w"))) return false;
		
		fwrite(&ini, sizeof(int), 1, fichero);
		fwrite(&ini, sizeof(int), 1, fichero);
	}
	
	fclose(fichero);
	return true;
}

/**
 * @brief Funcion utilizada para que realizamos una búsqueda del key en cuestión.
 * 
 * @param book_id id de la key a buscar.
 * @param indexName nombre del fichero idx.
 * @param nodeIDOrDataOffset dato para devolver el offset.
 * @return true 
 * @return false 
 */
bool findKey (const char* book_id, const char* indexName, int* nodeIDOrDataOffset) {
	int padre = 0, state = 0;
	Node* node = NULL;
	FILE* file = NULL;
	if (!book_id) {
		fprintf(stderr, "No se ha introducido un id a buscar.");
		return false;
	} else if (!indexName || !nodeIDOrDataOffset) {
		fprintf(stderr, "No hay fichero indice.");
		return false;
	} else {
		file = fopen(indexName, "r");
	}
	
	/* Inicializamos la matriz de nodos. */
	node = node_ini();
	if (!node) return false;
	
	/* Leemos el primer dato del fichero que corresponde al padre. */
	fread(&padre, sizeof(int), 1, file);
	
	/* Usaremos un algoritmo: Cada nodo ocupa 20 espacios en el fichero. Sabiendo eso, podemos concluir
	que el offset del nodo es 20 * id + 8. Leemos los datos del padre. */
	fseek(file, 20*padre+8, SEEK_SET);
	
	/* Guardamos los datos del padre. */
	fread(&node->book_id, sizeof(char), PK_SIZE, file);
	fread(&node->left, sizeof(int), 1, file);
	fread(&node->right, sizeof(int), 1, file);
	fread(&node->parent, sizeof(int), 1, file);
	fread(&node->offset, sizeof(int), 1, file);
	
	while(state != -1) {
		if (!(strncmp(node->book_id, book_id, PK_SIZE))) {
			/* Se ha encontrado. */
			*nodeIDOrDataOffset = node->offset;
			
			/* Liberamos todo. */
			free(node);
			fclose(file);
			return true;
		} else if (strcmp(node->book_id, book_id) > 0) {
			if (node->left == -1) {
				state = -1;
			} else {
				/* Si no se encuentra y existe uno menor, se modifica el nodeIDOrDataOffset y
				situamos el puntero del fichero en el offset correspondiente. */
				*nodeIDOrDataOffset = node->left;
				fseek(file, 20*node->left+8, SEEK_SET);
			}
		} else if  (strcmp(node->book_id, book_id) < 0){
			if (node->right == -1) {
				state = -1;
			} else {
				/* Si no se encuentra y existe uno mayor, se modifica el nodeIDOrDataOffset y
				situamos el puntero del fichero en el offset correspondiente. */
				*nodeIDOrDataOffset = node->right;
				fseek(file, 20*node->right+8, SEEK_SET);
			}
		}
		
		/* Como no se ha encontrado, modificamos los datos del nodo siguiente. */
		fread(&node->book_id, sizeof(char), PK_SIZE, file);
		fread(&node->left, sizeof(int), 1, file);
		fread(&node->right, sizeof(int), 1, file);
		fread(&node->parent, sizeof(int), 1, file);
		fread(&node->offset, sizeof(int), 1, file);
	}
	
	/* Liberamos todo. */
	free(node);
	fclose(file);
		
	return false;
}

/**
 * @brief Funcion que anyadira una entrada a la tabla.
 * 
 * @param book libro que hay que anyadir.
 * @param tableName nombre de la tabla.
 * @param indexName nombre del indice a anyadir.
 * @return true 
 * @return false 
 */
bool addTableEntry(Book *book, const char *tableName, const char *indexName) {
	FILE *fichero = NULL;
    int nodeIDOrDataOffset;
    int pos=-1;

	/* Tratamiento de errores. */
	if (!book || !tableName || !indexName) {
		fprintf(stderr, "Error \"addTableEntry()\" 1: los parametros de la funcion no existen.\n");
		return -1;
	}

    if (findKey(book->book_id, indexName, &nodeIDOrDataOffset)) {
        fprintf(stderr, "Error \"addTableEntry()\" 2: la clave introducida ya está en la base de datos\n");
        return false;
    }

    if (!(fichero = fopen(tableName, "rb"))) {
		fprintf(stderr, "Error \"addTableEntry()\" 3: error en la apertura de fichero.");
        return false;
    }
	
    /*Comprobamos si hay ficheros borrados. */
    fread(&pos, sizeof(int), 1, fichero);

	/* Si no hay nodos borrados */
    if (pos == -1) { 
        fclose(fichero);

        if (!(fichero = fopen(tableName, "ab"))) {
			fprintf(stderr, "Error \"addTableEntry()\" 4: error en la apertura de fichero.");
            return false;
        }

        fwrite(book->book_id, sizeof(char), 4, fichero);
        fwrite(&book->title_len, sizeof(int), 1, fichero);
        fwrite(book->title, sizeof(char), book->title_len, fichero);

        fclose(fichero);
    }

    /*offset: es el nodeIdOrDataOffset*/
    /*actualizar index*/
	return addIndexEntry(book->book_id, nodeIDOrDataOffset, indexName);
}

/**
 * @brief Funcion que introduce una entrada en el .idx, si la entrada existe, no la introduce y devuelve false.
 * 
 * @param book_id id a introducir en el idx.
 * @param bookOffset offset del libro.
 * @param indexName nombre del fichero.
 * @return true 
 * @return false 
 */
bool addIndexEntry (char* book_id, int bookOffset, const char* indexName) {
	FILE *fichero = NULL;
    int nodeIDOrDataOffset = 0, num_nodes = 0, nodo_inicial = 0, borrado, a = -1, registrosBorrados = 0;
    char padre[PK_SIZE] = "/0";
	char fileAux[32] = "/0";
	
	if (!indexName) {
		return -1;
	} else {
		/* Remplazamos la extension por si se ha abierto el .dat */
		replaceExtensionByIdx(indexName, fileAux);
	}
	
    if (!(fichero=fopen(fileAux,"r+"))) {
        return false;
    }
	
	/* Buscamos a ver si el elemento ya existe. */
    if (findKey(book_id, fileAux, &nodeIDOrDataOffset)) {
        fclose(fichero);
        return false;
    }

	/* Leemos los primeros datos del fichero para empezar a trabajar. */
    fread(&nodo_inicial, sizeof(int), 1, fichero);
    fread(&borrado, sizeof(int), 1, fichero);

	/* Si existe un nodo borrado, lo buscamos e introducimos el nuevo en ese hueco. Sino, escribimos en el final del fichero. */
    if (borrado != -1) {
        fseek(fichero, nodeIDOrDataOffset*20+8, SEEK_SET);
        fread(padre, sizeof(padre), 1, fichero);
       
        if (strncmp(book_id,padre,PK_SIZE) < 0) {
            fwrite(&borrado, sizeof(int), 1, fichero);
        } else {
            fseek(fichero,ftell(fichero)+4,SEEK_SET);
            fwrite(&borrado, sizeof(int), 1, fichero);
        }

        fseek(fichero, borrado*20+12, SEEK_SET);
        fread(&registrosBorrados, sizeof(int), 1, fichero);
		
        fseek(fichero, 4, SEEK_SET);
		fwrite(&registrosBorrados, sizeof(int), 1, fichero);
		
        fseek(fichero, borrado*20+8, SEEK_SET);
        fwrite(book_id, PK_SIZE, 1, fichero);
		
		fwrite(&a, sizeof(int), 1, fichero);
		fwrite(&a, sizeof(int), 1, fichero);
		fwrite(&nodeIDOrDataOffset, sizeof(int), 1, fichero);
        fwrite(&bookOffset, sizeof(int), 1, fichero);
    } else {
        fseek(fichero, 0, SEEK_END);
        num_nodes = (ftell(fichero)-8)/20;

        if (nodo_inicial == -1) {
            fseek(fichero, 0, SEEK_SET);
            fwrite(&num_nodes, sizeof(int), 1, fichero);
		} else {
            fseek(fichero, nodeIDOrDataOffset*20+8, SEEK_SET);
            fread(padre, sizeof(padre), 1, fichero);
			
            if (strncmp(book_id, padre, PK_SIZE) < 0) {
                fwrite(&num_nodes, sizeof(int), 1, fichero);
            } else {
                fseek(fichero,ftell(fichero)+4,SEEK_SET);
                fwrite(&num_nodes, sizeof(int), 1, fichero);
            }
        }
		
		/* Vamos al final del fichero e introducimos el nuevo nodo. */
		fseek(fichero, 0, SEEK_END);
		fwrite(book_id, PK_SIZE, 1, fichero);
		fwrite(&a, sizeof(int), 1, fichero);
		fwrite(&a, sizeof(int), 1, fichero);
		fwrite(&nodeIDOrDataOffset, sizeof(int), 1, fichero);
		fwrite(&bookOffset, sizeof(int), 1, fichero);   
    }

    fclose(fichero);
    return true;
}

/**
 * @brief Funcion que se encargara de imprimir un arbol de los datos guardados en indexName.
 * 
 * @param level profundidad de la impresion del arbol.
 * @param indexName nombre del fichero a imprimir.
 */
void printTree (size_t level, const char* indexName) {
	int registrosBorrados = 0, padre = 0, totalTam = 0, i = 0;
	char fileAux[32] = "\0";
	int nodoActual = 0;
	int nodoIzq = -1;
	int nodoDer = -1;	
	Node** node = NULL;
	
	/* Inicializamos la tabla de nodos. */
	node = malloc (tamNode * sizeof(Node*));
	if (!node) return;
	
	if (!indexName) {		
		free(node);
		return;
	} else {
		replaceExtensionByIdx(indexName, fileAux);
	}
	
	index_read(node, fileAux, &totalTam, &padre, &registrosBorrados);
	
	/* Imprimimos el nodo padre y actualizamos los datos de los nodos. */
	nodoActual = padre;
	node_print(stdout, node, padre, -1);
	nodoIzq = node[nodoActual]->left;
	nodoDer = node[nodoActual]->right;
	
	if (level > 0) {
		if (nodoIzq != -1) {
			node_rec(node, nodoIzq, 1, level, IZQ);
		}
		
		if (nodoDer != -1) {
			node_rec(node, nodoDer, 1, level, DER);
		}
	}
	
	/* Liberamos todo. */
	for (i = 0; i < totalTam; i++) {
		node_destroy(node[i]);
	}
	
	free(node);
	
	return;
}

/**
 * @brief Reserva de memoria del nodo.
 * 
 * @return Node* 
 */
Node* node_ini() {	
	Node* node = (Node*) malloc (sizeof(Node));
	if (!node) return NULL;	
	
	strcpy(node->book_id, "\0");
	node->left = -1;
	node->right = -1;
	node->parent = -1;
	node->offset = -1;
		
	return node;
}

/**
 * @brief Funcion para destruir la memoria alocada.
 * 
 * @param node nodo a destruir.
 */
void node_destroy(Node* node) {
	if (node != NULL) {
		free(node);
		node = NULL;
	}
	
	return;
}

/* 
*  Funcion que imprime los datos del nodo.
*  Sintaxis: nombre (posicion); offset.
*/
void node_print(FILE* fp, Node** node, int pos, int rama) {
	char cadena[PK_SIZE+1] = "\0";
	if (!fp || !node) return;
	
	/* Copio solo los 4 primeros caracteres del book_id del nodo ya que tiene un numero aleatorio 
	y hace que se haga mal... */
	strncpy(cadena, node[pos]->book_id, PK_SIZE);
	
	if (rama == IZQ) {
		fprintf(stdout, " l ");
	} else if (rama == DER) {
		fprintf(stdout, " r ");
	}
	
	fprintf(fp, "%s (%d); %d\n", cadena, pos, node[pos]->offset);
	return;
}

/* Funcion que imprime de forma recursiva los datos del arbol. */
void node_rec(Node** node, int pos, int leaf, const int level, int rama) {
	int i = 0, nextLeaf = 0;
	
	/* Su la hoja maxima es igual a la variable leaf, no seguimos imprimiendo. */
	if (!node || leaf == level+1) return;
	else if (pos == -1) return;
	else {			
		/* Imprimimos los datos. */
		/* Realizamos un tabulado correspondiente con el numero de hoja que sea. */
		for(i = 0; i < leaf; i++) {
			fprintf(stdout, "\t");
		}
		node_print(stdout, node, pos, rama);
		
		nextLeaf = leaf + 1;
		
		/* Entramos de forma recursiva al resto de casos. */
		if (node[pos]->left != -1) {			
			node_rec(node, node[pos]->left, nextLeaf, level, IZQ);
		}
		
		if (node[pos]->right != -1) {			
			node_rec(node, node[pos]->right, nextLeaf, level, DER);
		}
	}
	
	return;
}

/* Funcion que realiza el cambio de extension. Suponemos que la extension del fichero es de 3 caracteres. */
void replaceExtensionByIdx(const char *fileName, char * indexName) {
	if (!fileName || !indexName) return;
	
	/* Copiamos solo el nombre del fichero sin la extension. */
	strncpy(indexName, fileName, strlen(fileName)-4);
	
	/* Cambiamos la extension a .idx. */
	strcat(indexName, ".idx");
    return;
}

/* 
*  Funcion auxiliar que lee los datos de un ficero index. 
*  Para recorrer de forma correcta el fichero, se ha de reservar memoria para un doble puntero de nodos
*  e inicializar las variables totalTam, padre y registosBorrados.
*/
void index_read(Node** node, const char* fileAux, int* totalTam, int* padre, int* registrosBorrados) {
	int i = 0;
	FILE* fichero = NULL;
	char nombreFich[32] = "\0";
	
	replaceExtensionByIdx(fileAux, nombreFich);
	if (!(fichero = fopen(nombreFich, "r"))) return;
	else if ((*totalTam + *padre + *registrosBorrados) != 0) {
		*totalTam = 0;
		*padre = 0;
		*registrosBorrados = 0;
	}
	
	/* Se leen los dos primeros datos de la cabecera. */
	fread(padre, sizeof(int), 1, fichero);
	fread(registrosBorrados, sizeof(int), 1, fichero);
	
	/* Nos disponemos a leer cada dato de los nodos. */
	
	for (i = 0; !feof(fichero); i++) {
		node[i] = node_ini();
		
		/* Leemos el nombre del nodo. */
		fread(node[i]->book_id, sizeof(char), sizeof(node[i]->book_id), fichero);
		if (strcmp(node[i]->book_id, "\0")) {
			/* Leemos el hijo izquierdo. */
			fread(&node[i]->left, sizeof(int), 1, fichero);
			
			/* Leemos el hijo derecho. */
			fread(&node[i]->right, sizeof(int), 1, fichero);
			
			/* Leemos el padre. */
			fread(&node[i]->parent, sizeof(int), 1, fichero);
			
			/* Leemos el offset. */
			fread(&node[i]->offset, sizeof(int), 1, fichero);

			/* Aumentamos en 1 el tamaño del array de nodos que hay. */
			*totalTam += 1; 
		} else {
			node_destroy(node[i]);
			i--;
		}
		
	}
	
	fclose(fichero);
	
	return;
}