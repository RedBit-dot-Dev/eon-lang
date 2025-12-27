#ifndef EON_SYMBOL_TABLE_H
#define EON_SYMBOL_TABLE_H
#include "data_types.h"
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
    char* literal_value;
    int line_usage;
    int line_definition;
    enum _symbol_kinds kind;
    enum _data_type data_type;
    struct _symbol_node* next;
} Symbol_Node;

typedef struct _symbol_hash_node {
    char* key; // name of symbol node
    Symbol_Node* symbol_node;
    struct _symbol_hash_node* next;
} Symbol_Hash_Node;

typedef struct _symbol_hash_table {
    int used;
    int capacity;
    Symbol_Hash_Node** array;
} Symbol_Hash_Table;

typedef struct _symbol_table {
    Vector* child_scopes;
    Symbol_Node* node_list;
    Symbol_Hash_Table* hash_table;
    struct _symbol_table* parent;
} Symbol_Table;

int hash(Symbol_Hash_Table* h_table, char* key);

void rehash(Symbol_Hash_Table* h_table);

void insertInSymbolHashTable(Symbol_Hash_Table* h_table, Symbol_Node* s_node);

bool insertInSymbolTable(Symbol_Table* s_table, char* name, int data_type, int kind, int line_definition, int line_usage, char* literal_value);

Symbol_Node* searchSymbolNode(Symbol_Table* s_table, char* name);

Symbol_Hash_Node* createSymbolHashNode(char* key, Symbol_Node* symbol_node);

Symbol_Hash_Table* createSymbolHashTable();

Symbol_Node* createSymbolNode(char* name, int data_type, int kind, int line_definition, int line_usage, char* literal_value);

Symbol_Table* createSymbolTable(Symbol_Table* parent);

void appendChildScope(Symbol_Table* parent, Symbol_Table* child);

void symbolHashTableFree(Symbol_Hash_Table* h_table);

void symbolTableFree(Symbol_Table* s_table);

#endif