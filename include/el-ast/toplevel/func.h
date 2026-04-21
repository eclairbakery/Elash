#include <el-ast/common/ident.h>
#include <el-ast/stmt/block.h>

#include <el-defs/inttypes.h>

typedef struct ElAstFuncParam ElAstFuncParam;
struct ElAstFuncParam {
    ElAstIdentNode* type;
    ElAstIdentNode* name;

    ElAstFuncParam* next;
    ElAstFuncParam* prev;
};

typedef struct ElAstFuncParamList {
    ElAstFuncParam* head;
    ElAstFuncParam* tail;
    usize count;
} ElAstFuncParamList;

typedef struct ElAstFuncDefinition {
    ElAstIdentNode* ret_type;
    ElAstIdentNode* name;
    ElAstFuncParamList params;
    ElAstBlockStmtNode* block;
} ElAstFuncDefinition;
