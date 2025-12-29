#include "include/data_types.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

int checkDataType(const Token* token) {

    assert(TYPE_MAX == 13 && "checkDataType() must handle all type values");

    switch (token->length) {

        case 2:
            if (!strcmp(token->value, "i8")) return TYPE_i8;
            if (!strcmp(token->value, "u8")) return TYPE_u8;
            break;

        case 3:

            if (token->value[0] == 'i') {
                if (!strcmp(token->value, "i16")) return TYPE_i16;
                if (!strcmp(token->value, "int")) return TYPE_i32;
                if (!strcmp(token->value, "i32")) return TYPE_i32;
                if (!strcmp(token->value, "i64")) return TYPE_i64;
            }

            else if (token->value[0] == 'u') {
                if (!strcmp(token->value, "u16")) return TYPE_u16;
                if (!strcmp(token->value, "u32")) return TYPE_u32;
                if (!strcmp(token->value, "u64")) return TYPE_u64;
            }

            else if (token->value[0] == 'f') {
                if (!strcmp(token->value, "f32")) return TYPE_FLOAT;
                if (!strcmp(token->value, "f64")) return TYPE_DOUBLE;
            }

            break;

        case 4:

            if (!isdigit(token->value[1])) {
                if (!strcmp(token->value, "char")) return TYPE_i8;
                if (!strcmp(token->value, "bool")) return TYPE_u8;
                if (!strcmp(token->value, "void")) return TYPE_VOID;
            }

            else {
                if (!strcmp(token->value, "i128")) return TYPE_i128;
                if (!strcmp(token->value, "u128")) return TYPE_u128;
            }

            break;

        case 5:
            if (!strcmp(token->value, "float")) return TYPE_FLOAT;
            break;

        case 6:
            if (!strcmp(token->value, "double")) return TYPE_DOUBLE;
            break;

        default:
            break;
    }

    return -1;
}

char* dataTypeToString(const int data_type) {

    assert(TYPE_MAX == 13 && "dataTypeToString() must handle all type values");

    switch (data_type) {
        case TYPE_i8        : return "i8";
        case TYPE_i16       : return "i16";
        case TYPE_i32       : return "i32";
        case TYPE_i64       : return "i64";
        case TYPE_i128      : return "i128";
        case TYPE_u8        : return "i8";      // LLVM ir doesn't recognize unsingned
        case TYPE_u16       : return "i16";     // LLVM ir doesn't recognize unsingned
        case TYPE_u32       : return "i32";     // LLVM ir doesn't recognize unsingned
        case TYPE_u64       : return "i64";     // LLVM ir doesn't recognize unsingned
        case TYPE_u128      : return "i128";    // LLVM ir doesn't recognize unsingned
        case TYPE_FLOAT     : return "float";
        case TYPE_DOUBLE    : return "double";
        case TYPE_VOID      : return "void";

        default:
            break;
    }

    printf("Data type not recognized\n");
    exit(ERROR_DATA_TYPE_UNRECOGNIZED);

    return "ERROR";
}