#ifndef EON_SYMBOL_TABLE_H
#define EON_SYMBOL_TABLE_H
#include "types.h"
#include "vector.h"
#include <stdbool.h>

enum _symbol_kinds {
    SYMBOL_VAR,
    SYMBOL_CONST,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER,
};

typedef struct _symbol_node {
    char* name;
    enum _data_types data_type;
    enum _symbol_kinds kind;
    int line_declaration;
    int line_usage;
    char* literal_value;
    struct _symbol_node* next;
} Symbol_Node;

typedef struct _symbol_hash_node {
    struct _symbol_hash_node* next;
    Symbol_Node* symbol_node;
    char* key; // name of symbol node
} Symbol_Hash_Node;

typedef struct _symbol_hash_table {
    Symbol_Hash_Node** array;
    int capacity;
    int used;
} Symbol_Hash_Table;

typedef struct _symbol_table {
    struct _symbol_table* parent;
    Symbol_Node* node_list;
    Symbol_Hash_Table* hash_table;
    Vector* child_scopes;
} Symbol_Table;

int hash(Symbol_Hash_Table* h_table, char* key);

void rehash(Symbol_Hash_Table* h_table);

void insertInSymbolHashTable(Symbol_Hash_Table* h_table, Symbol_Node* s_node);

bool insertInSymbolTable(Symbol_Table* s_table, char* name, int data_type, int kind, int line_declaration, int line_usage, char* literal_value);

Symbol_Node* searchSymbolNode(Symbol_Table* s_table, char* name);

Symbol_Hash_Node* createSymbolHashNode(char* key, Symbol_Node* symbol_node);

Symbol_Hash_Table* createSymbolHashTable();

Symbol_Node* createSymbolNode(char* name, int data_type, int kind, int line_declaration, int line_usage, char* literal_value);

Symbol_Table* createSymbolTable(Symbol_Table* parent);

void appendChildScope(Symbol_Table* parent);

void appendChildScope(Symbol_Table* parent);

void symbolHashTableFree(Symbol_Hash_Table* h_table);

void symbolTableFree(Symbol_Table* s_table);


#endif