#ifndef EON_TYPES_H
#define EON_TYPES_H
#include "token.h"

enum _data_types {
    TYPE_i8, // defined char
    TYPE_i16,
    TYPE_i32, //defined int
    TYPE_i64,
    TYPE_i128,
    TYPE_u8, // defined bool and byte
    TYPE_u16,
    TYPE_u32,
    TYPE_u64,
    TYPE_u128,
    TYPE_f32, // defined float
    TYPE_f64,
    TYPE_VOID,
    TYPE_MAX,
};

int tokenToDataType(const Token* token);

char* dataTypeToString(const int data_type);

#endif