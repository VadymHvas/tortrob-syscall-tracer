#pragma once

#include <time.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/fs/struct.h"

#define INIT_EL_INFO(type, size, fmt_struct_func) \
        { type, size, fmt_struct_func }

#define FMT_SIZED_ARR(ctx, addr, size, type, type_size, fmt_struct_func) \
        struct el_info el = INIT_EL_INFO(type, type_size, fmt_struct_func); \
        TRY_FMT(fmt_array_sized_common, ctx, addr, size, el)

#define FMT_SIZED_ARR_FD(ctx, addr, size) FMT_SIZED_ARR(ctx, addr, size, EL_FD, sizeof(int), NULL)

typedef enum {
        EL_INT,
        EL_UINT,
        EL_LONG,
        EL_ULONG,
        EL_PTR,
        EL_CSTRING,
        EL_BYTES,
        EL_STRUCT,
        EL_FD
} arr_el_type;

struct el_info {
        arr_el_type type;
        size_t size;

        fmt_struct_func_t fmt_struct_func;
};

int fmt_array_sized_common(struct parser_ctx_struct *ctx, 
                           unsigned long long addr, size_t size, struct el_info el);