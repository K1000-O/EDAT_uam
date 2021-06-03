#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

int main() {
    const char *tableName = "myDataBase.dat";
    const char *indexName = "myDataBase.idx";
    
    Book * b = malloc (sizeof(Book));
    strcpy(b->book_id, "BAR2");
    b->title_len = PK_SIZE;
    b->title = (char*) malloc (sizeof(char) * 10);
    strcpy(b->title, "Hola k");
    
    printf("-----Original tree-------\n");
    printTree(4, indexName);
    addTableEntry(b, tableName, indexName);

    printf("-----after adding BAR2-------\n");
    printTree(4, indexName);
    return 0;
}