#pragma once

#include <time.h>

#include "args/helpers.h"
#include "args/fs/struct.h"

#define INIT_EL_INFO(type, size, fmt_struct_func) \
        { type, size, fmt_struct_func }

#define FMT_SIZED_ARR(ctx, addr, size, type, type_size, fmt_struct_func) \
        struct el_info el = INIT_EL_INFO(type, type_size, fmt_struct_func); \
        TRY_FMT(fmt_array_sized_common, ctx, addr, size, el)

#define FMT_STRING_ARR(ctx, addr) \
        TRY_FMT(fmt_string_array, ctx, addr)

#define FMT_SIZED_ARR_FD(ctx, addr, size) FMT_SIZED_ARR(ctx, addr, size, EL_FD, sizeof(int), NULL)

/**
 * arr_el_type - Enum to represent different types of elements in an array.
 * 
 * EL_INT: Represents an integer element.
 * EL_UINT: Represents an unsigned integer element.
 * EL_LONG: Represents a long integer element.
 * EL_ULONG: Represents an unsigned long integer element.
 * EL_PTR: Represents a pointer element.
 * EL_CSTRING: Represents a C-string element.
 * EL_BYTES: Represents a byte array element.
 * EL_STRUCT: Represents a structured element that requires a custom formatting function.
 * EL_FD: Represents a file descriptor element.
 */
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

/**
 * struct el_info - Structure to hold information about an array element type.
 * 
 * type: Type of the array element (from arr_el_type).
 * size: Size of the array element in bytes.
 * fmt_struct_func: Pointer to a function used to format the element if it 
 *      is of type EL_STRUCT. This function should take a parser context and a pointer to the element data, and
 */
struct el_info {
        arr_el_type type;
        size_t size;

        fmt_struct_func_t fmt_struct_func;
};

/**
 * fmt_array_sized_common - Common function to format arrays of various types and sizes.
 * 
 * @param ctx: Parser context.
 * @param addr: Address of the array in the tracee's memory.
 * @param size: Number of elements in the array.
 * @param el: Structure containing information about the type and size of the array elements, as
 *      well as a pointer to a formatting function if the elements are of type EL_STRUCT.
 */
int fmt_array_sized_common(struct parser_ctx_struct *ctx, 
                           unsigned long long addr, size_t size, struct el_info el);

int fmt_string_array(struct parser_ctx_struct *ctx, unsigned long long addr);