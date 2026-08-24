#include "lowerer-internals.h"

#include <elash/util/assert.h>
#include <elash/util/todo.h>
#include <elash/mir/type.h>

static ElMirType* make_slice_type(ElDynArena* arena, ElMirType* usize_type, const ElHirSliceType* slice) {
    ElMirType** items = EL_DYNARENA_NEW_ARR(arena, ElMirType*, 2);
    ElMirType*  base   = el_lowerer_map_type_raw(arena, usize_type, slice->base);
    items[EL_MIR_SLICE_FIELD_DATA] = el_mir_new_ptr_type(arena, base);
    items[EL_MIR_SLICE_FIELD_LEN]  = usize_type;
    return el_mir_new_tuple_type(arena, items, 2);
}

ElMirType* el_lowerer_map_type_raw(ElDynArena* arena, ElMirType* usize_type, const ElHirType* type) {
    if (type == NULL) return NULL;
    switch (type->kind) {
    case EL_HIR_TYPE_PRIM:
        switch (type->as.prim.kind) {
        case EL_PRIMTYPE_VOID:
            return el_mir_new_void_type(arena);
        case EL_PRIMTYPE_BOOL:
            return el_mir_new_int_type(arena, 1, false);
        case EL_PRIMTYPE_INT: {
            uint32_t width = 0;
            // TODO: actually map native and efficient ints to correct width
            // NOLINTBEGIN(readability-magic-numbers)
            switch (type->as.prim.as.integral.width) {
            case EL_HIR_IWIDTH_NATIVE:    width = 64;  break;
            case EL_HIR_IWIDTH_EFFICIENT: width = 32;  break;
            case EL_HIR_IWIDTH_8:         width = 8;   break;
            case EL_HIR_IWIDTH_16:        width = 16;  break;
            case EL_HIR_IWIDTH_32:        width = 32;  break;
            case EL_HIR_IWIDTH_64:        width = 64;  break;
            case EL_HIR_IWIDTH_128:       width = 128; break;
            }
            // NOLINTEND(readability-magic-numbers)
            return el_mir_new_int_type(arena, width, type->as.prim.as.integral.is_signed);
        }
        case EL_PRIMTYPE_FLOAT: {
            uint32_t width = 0;
            // TODO: actually map efficient floats to correct width
            // NOLINTBEGIN(readability-magic-numbers)
            switch (type->as.prim.as.fp.width) {
            case EL_HIR_FPWIDTH_EFFICIENT: width = 32; break;
            case EL_HIR_FPWIDTH_16:        width = 16; break;
            case EL_HIR_FPWIDTH_32:        width = 32; break;
            case EL_HIR_FPWIDTH_64:        width = 64; break;
            case EL_HIR_FPWIDTH_128:       width = 128; break;
            }
            // NOLINTEND(readability-magic-numbers)
            return el_mir_new_float_type(arena, width);
        }
        }
        EL_UNREACHABLE("unknown prim type kind");
    case EL_HIR_TYPE_FUNC: {
        ElMirType* ret_type = el_lowerer_map_type_raw(arena, usize_type, type->as.func.ret_type);
        ElMirType** params = EL_DYNARENA_NEW_ARR(arena, ElMirType*, type->as.func.param_count);
        for (usize i = 0; i < type->as.func.param_count; ++i) {
            params[i] = el_lowerer_map_type_raw(arena, usize_type, type->as.func.params[i]);
        }
        return el_mir_new_func_type(arena, ret_type, params, type->as.func.param_count);
    }
    case EL_HIR_TYPE_ARRAY: {
        ElMirType* base = el_lowerer_map_type_raw(arena, usize_type, type->as.array.base);
        return el_mir_new_array_type(arena, base, type->as.array.size);
    }
    case EL_HIR_TYPE_STRUCT: {
        ElMirType** elements = EL_DYNARENA_NEW_ARR(arena, ElMirType*, type->as.struct_.count);
        for (usize i = 0; i < type->as.struct_.count; ++i) {
            elements[i] = el_lowerer_map_type_raw(arena, usize_type, type->as.struct_.fields[i].type);
        }
        return el_mir_new_tuple_type(arena, elements, type->as.struct_.count);
    }
    case EL_HIR_TYPE_TUPLE: {
        ElMirType** elements = EL_DYNARENA_NEW_ARR(arena, ElMirType*, type->as.tuple.count);
        for (usize i = 0; i < type->as.tuple.count; ++i) {
            elements[i] = el_lowerer_map_type_raw(arena, usize_type, type->as.tuple.elements[i]);
        }
        return el_mir_new_tuple_type(arena, elements, type->as.tuple.count);
    }
    case EL_HIR_TYPE_REF:
        return el_mir_new_ptr_type(arena, el_lowerer_map_type_raw(arena, usize_type, type->as.ref.base));
    case EL_HIR_TYPE_RWSLICE:
        return el_mir_new_ptr_type(arena, el_lowerer_map_type_raw(arena, usize_type, type->as.rwslice.base));
    case EL_HIR_TYPE_SLICE:
        return make_slice_type(arena, usize_type, &type->as.slice);
    case EL_HIR_TYPE_DISTINCT:
        return el_lowerer_map_type_raw(arena, usize_type, type->as.distinct.orig);
    }
    EL_UNREACHABLE("unknown hir type kind");
}

ElMirType* el_lowerer_map_type(ElLowerer* lw, const ElHirType* type) {
    return el_lowerer_map_type_raw(lw->arena, lw->builtins->type_usize, type);
}
