#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

typedef struct ElAstNode ElAstNode;

typedef enum ElAstLiteralType {
    EL_AST_LIT_INT,
    EL_AST_LIT_FLOAT,
    EL_AST_LIT_CHAR,
    EL_AST_LIT_STRING,
    EL_AST_LIT_BOOL,
    EL_AST_LIT_NULL,
} ElAstLiteralType;

typedef struct ElAstIntLiteralNode {
    uint64_t value;
} ElAstIntLiteralNode;

typedef struct ElAstFloatLiteralNode {
    long double value;
} ElAstFloatLiteralNode;

typedef struct ElAstCharLiteralNode {
    char value;
} ElAstCharLiteralNode;

typedef struct ElAstStringLiteralNode {
    ElStringView value;
} ElAstStringLiteralNode;

typedef struct ElAstBoolLiteralNode {
    bool value;
} ElAstBoolLiteralNode;

typedef struct ElAstLiteralNode {
    ElAstLiteralType type;
    union {
        ElAstIntLiteralNode int_;
        ElAstFloatLiteralNode float_;
        ElAstCharLiteralNode char_;
        ElAstStringLiteralNode str_;
        ElAstBoolLiteralNode bool_;
    } of;
} ElAstLiteralNode;

ElAstNode* el_ast_new_int_literal(ElDynArena* arena, uint64_t value);
ElAstNode* el_ast_new_float_literal(ElDynArena* arena, long double value);
ElAstNode* el_ast_new_char_literal(ElDynArena* arena, char value);
ElAstNode* el_ast_new_string_literal(ElDynArena* arena, ElStringView value);
ElAstNode* el_ast_new_bool_literal(ElDynArena* arena, bool value);
ElAstNode* el_ast_new_null_literal(ElDynArena* arena);
