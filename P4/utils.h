#ifndef EDAT_H
#define EDAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#define NO_DELETED_REGISTERS −1
#define INDEX_HEADER_SIZE 8
#define DATA_HEADER_SIZE 4
/* primary key size */
#define PK_SIZE 4
#define LANG_SIZE 2

#define IZQ 0
#define DER 1
/* Our table is going to contain astring (title) and
an alphanumeric primary key (book id)
*/
typedef struct book {
	char book_id[PK_SIZE]; /* primary key */
	size_t title_len; /* title length */
	char* title ; /* string to be saved in the database */
	char lang[LANG_SIZE]; /* Añadido para meter la lengua "clave secundaria". */
} Book;

/* Note that in general a struct cannot be used to
	handle tables in data bases since the table structure
	is unknown at compilation time .
*/
typedef struct node {
	char book_id[PK_SIZE]; /* ?? + LANG_SIZE */
	int left, right, parent, offset;
} Node;

/* Function prototypes.
	see function descriptions in the following sections
	
	All function return true if success or false if failed
	except findKey which return true if register found
	and false otherwise.
*/
bool createTable(const char* tableName);
bool createIndex (const char* indexName);
bool findKey (const char* book_id, const char* indexName, int* nodeIDOrDataOffset);
bool addTableEntry (Book* book, const char* tableName, const char* indexName);
bool addIndexEntry (char* book_id, int bookOffset, const char* indexName);
void printTree (size_t level, const char* indexName);
void replaceExtensionByIdx(const char *fileName, char * indexName);

/******************************************************/
/* 			  Metodos de control del nodo             */
/******************************************************/
Node* node_ini();
void node_destroy(Node* node);
void node_print(FILE* fp, Node** node, int pos, int rama);
void node_rec(Node** node, int pos, int leaf, const int level, int rama);
/*******************************************************/

void index_read(Node** node, const char* fileAux, int* totalTam, int* padre, int* registrosBorrados);

#endif