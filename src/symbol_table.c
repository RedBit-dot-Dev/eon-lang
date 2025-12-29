#include "include/symbol_table.h"
#include "include/arena.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Arena* arena_main;

char* symbolKindToString(int symbol_kind) {

    switch (symbol_kind) {
        case SYMBOL_CONST: return "SYMBOL_CONST";
        case SYMBOL_FUNCTION: return "SYMBOL_FUNCTION";
        case SYMBOL_PARAMETER: return "SYMBOL_PARAMETER";
        case SYMBOL_VAR: return "SYMBOL_VAR";
    }

    return NULL;
}

void printSymbolTable(Symbol_Table* s_table, int tab_count) {

    if (s_table == NULL) return;
    

    Symbol_Node* index = s_table->node_list;

    while (index != NULL) {

        for (int i = 0; i < tab_count; i++) {
            printf("\t");
        }

        printf("NAME: %s, KIND: %s, DATA_TYPE: %s, LINE_DEFINITION: %d, LINE_USAGE: %d, LITERAL_VALUE: %s\n",
            index->name,
            symbolKindToString(index->kind),
            dataTypeToString(index->data_type),
            index->line_definition,
            index->line_usage,
            index->literal_value
        );

        index = index->next;

    }

    if (s_table->child_scopes != NULL) {

        Symbol_Table** children = (Symbol_Table**) s_table->child_scopes->items;

        for (int i = 0; i < s_table->child_scopes->used; i++) {
            printSymbolTable(children[i], tab_count+1);
        }

    }

    return;
}

int hash(Symbol_Hash_Table* h_table, char* key) {

    int key_v = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        key_v *= 2;
        key_v += key[i];
    }

    // h(k) = m * ( (k*A) mod 1 )

    double A = 0.6180339887;

    int temp = (int) key_v * A;

    int hashed = (int) (h_table->capacity * ((key_v * A) - temp));

    return hashed;
}

void rehash(Symbol_Hash_Table* h_table) {

    int arr_size = h_table->used;

    Symbol_Node* arr_nodes[arr_size];

    for (int i = 0, j = 0; i < h_table->capacity; i++) {

        if (h_table->array[i] != NULL) {
            
            Symbol_Hash_Node* index = h_table->array[i];
    
            while (index->next != NULL) {
    
                arr_nodes[j++] = index->symbol_node;
    
                Symbol_Hash_Node* to_free = index;
                index = index->next;
                free(to_free);
            }

            arr_nodes[j++] = index->symbol_node;

            free(index);

            h_table->array[i] = NULL;

        }

    }

    h_table->capacity *= 2;
    h_table->used = 0;

    h_table->array = realloc(h_table->array, sizeof(Symbol_Hash_Node*) * h_table->capacity);

    for (int i = h_table->capacity/2; i < h_table->capacity; i++) {
        h_table->array[i] = NULL;
    }
    

    for (int i = 0; i < arr_size; i++) {
        insertInSymbolHashTable(h_table, arr_nodes[i]);
    }

    return;
}

void insertInSymbolHashTable(Symbol_Hash_Table* h_table, Symbol_Node* s_node) {

    if (h_table->array == NULL) {
        h_table->array = (Symbol_Hash_Node**) malloc(sizeof(Symbol_Hash_Node*));
        if (!h_table->array) {
            printf("Insufficient memory to allocate h_table array\n");
            exit(ERROR_INSUFFICIENT_MEMORY_RAM);
        }
        h_table->array[0] = NULL;
        h_table->capacity = 1;
    }

    int hashed = hash(h_table, s_node->name);

    Symbol_Hash_Node* hn = createSymbolHashNode(s_node->name, s_node);

    hn->next = h_table->array[hashed];
    h_table->array[hashed] = hn;

    h_table->used++;

    if (h_table->used > (h_table->capacity * 0.8)) {
        rehash(h_table);
    }

    return;
}

bool insertInSymbolTable(Symbol_Table* s_table, char* name, int data_type, int kind, int line_definition, int line_usage, char* literal_value) {

    if (searchSymbolNode(s_table, name) != NULL) {
        return false;
    }

    Symbol_Node* sn = createSymbolNode(name, data_type, kind, line_definition, line_usage, literal_value);
    sn->next = s_table->node_list;
    s_table->node_list = sn;

    insertInSymbolHashTable(s_table->hash_table, sn);

    return true;
}

Symbol_Node* searchScope(Symbol_Table* s_table, char* name) {
    
    if (s_table->hash_table == NULL || s_table->hash_table->array == NULL) return NULL;
    
    int hashed = hash(s_table->hash_table, name);
    
    Symbol_Hash_Node* index = s_table->hash_table->array[hashed];
    
    while (index != NULL) {
        if (strcmp(index->key, name) == 0) {
            return index->symbol_node;
        }
        index = index->next;
    }
    
    return NULL;
}

Symbol_Node* searchSymbolNode(Symbol_Table* s_table, char* name) {

    Symbol_Table* scope_to_search = s_table;
    Symbol_Node* result = NULL;

    while (scope_to_search != NULL) {

        result = searchScope(scope_to_search, name);

        if (result != NULL) break;

        scope_to_search = scope_to_search->parent;

    }

    return result;
}

// Assumes the variable exists in the provided scope or higher
bool isVarGlobal(Symbol_Table* s_table, char* name) {

    Symbol_Table* scope_to_search = s_table;
    Symbol_Node* result = NULL;

    while (scope_to_search != NULL) {

        result = searchScope(scope_to_search, name);

        if (result != NULL) break;

        scope_to_search = scope_to_search->parent;

    }

    if (scope_to_search->parent == NULL) {
        return true;
    }

    return false;
}

Symbol_Hash_Node* createSymbolHashNode(char* key, Symbol_Node* symbol_node) {

    Symbol_Hash_Node* hn = (Symbol_Hash_Node*) malloc(sizeof(Symbol_Hash_Node));
    if (!hn) {
        printf("Insufficient memory to allocate symbol hash node\n");
        exit(ERROR_INSUFFICIENT_MEMORY_RAM);
    }

    hn->key = key;
    hn->symbol_node = symbol_node;
    hn->next = NULL;

    return hn;
}

Symbol_Hash_Table* createSymbolHashTable() {

    Symbol_Hash_Table* ht = (Symbol_Hash_Table*) malloc(sizeof(Symbol_Hash_Table));
    if (!ht) {
        printf("Insufficient memory to allocate symbol hash table\n");
        exit(ERROR_INSUFFICIENT_MEMORY_RAM);
    }

    ht->array = NULL;
    ht->capacity = 0;
    ht->used = 0;

    return ht;
}

Symbol_Node* createSymbolNode(char* name, int data_type, int kind, int line_definition, int line_usage, char* literal_value) {

    Symbol_Node* sn = (Symbol_Node*) arenaPush(arena_main, sizeof(Symbol_Node), false);

    sn->name = name;
    sn->data_type = data_type;
    sn->kind = kind;
    sn->line_definition = line_definition;
    sn->line_usage = line_usage;
    sn->literal_value = literal_value;
    sn->next = NULL;

    return sn;
}

Symbol_Table* createSymbolTable(Symbol_Table* parent) {

    Symbol_Table* st = (Symbol_Table*) malloc(sizeof(Symbol_Table));
    if (!st) {
        printf("Insufficient memory to allocate symbol table\n");
        exit(ERROR_INSUFFICIENT_MEMORY_RAM);
    }

    st->parent = parent;
    st->node_list = NULL;
    st->hash_table = createSymbolHashTable();
    st->child_scopes = NULL;

    return st;
}

void appendChildScope(Symbol_Table* parent, Symbol_Table* child) {

    if (parent->child_scopes == NULL) {
        parent->child_scopes = createVector(sizeof(Symbol_Table*));
    }

    vectorPush(parent->child_scopes, child);

    return;
}

//! FIX THIS
void symbolHashTableFree(Symbol_Hash_Table* h_table) {

    if (h_table == NULL) return;

    if (h_table->array != NULL) {

        for (int i = 0; i < h_table->capacity; i++) {
    
            Symbol_Hash_Node* prior = h_table->array[i];
    
            if (prior == NULL) {
                continue;
            }
    
            Symbol_Hash_Node* index = prior->next;
    
            while (index != NULL) {
                free(prior->symbol_node);
                free(prior);
                prior = index;
                index = index->next;
            }
            free(prior->symbol_node);
            free(prior);
    
        }

    }

    free(h_table);

    return;
}

void symbolTableFree(Symbol_Table* s_table) {

    if (s_table->child_scopes != NULL) {
        Symbol_Table** vec = (Symbol_Table**) s_table->child_scopes->items;
        for (int i = 0; i < s_table->child_scopes->used; i++) {
            symbolTableFree(vec[i]);
        }
    }

    symbolHashTableFree(s_table->hash_table);

    free(s_table);

    return;
}