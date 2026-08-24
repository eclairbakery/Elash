#pragma once

#include <elash/mir/type.h>

/// BSType - Backend Specific Type
/// (That's why it's an opaque pointer)
typedef void ElBSType;

/// BTIQ - Backend Specific Query
typedef struct ElBSQuery ElBSQuery;

typedef usize ElBSQuerySize(const ElBSQuery* self, const ElBSType* type);
typedef usize ElBSQueryAlign(const ElBSQuery* self, const ElBSType* type);
// TODO: getting offsets of tuple fields

typedef ElBSType* ElBSQueryMap(const ElBSQuery* self, const ElMirType* mtype);

typedef struct ElBSQuery {
    ElBSQueryMap* map;

    ElBSQuerySize*  get_size;
    ElBSQueryAlign* get_align;

    void* ctx;
} ElBSQuery;
