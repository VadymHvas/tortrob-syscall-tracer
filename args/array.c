/*
 * args/array.c - Generic array formatter for syscall arguments.
 *
 * Reads an array from tracee memory and formats each element
 * according to provided type metadata (el_info).
 *
 * Supports primitive types, pointers, file descriptors,
 * strings, raw bytes, and user-defined structs.
 */

#include <stdlib.h>

#include "args/array.h"
#include "args/helpers.h"
#include "core/trace.h"

static int fmt_array_el(struct parser_ctx_struct *ctx, struct el_info *el, void *element);

int fmt_array_sized_common(struct parser_ctx_struct *ctx, 
        unsigned long long addr, size_t size, struct el_info el)
{
        if (!addr)
                return fmt_null(ctx);

        if (!size)
                return fmt_string(ctx, "[]");

        if (!el.size)
                return fmt_string(ctx, "<invalid el>");

        size_t total;
        if (__builtin_mul_overflow(size, el.size, &total))
                return fmt_string(ctx, "<overflow>");

        char *arr_buf = malloc(total);
        if (!arr_buf)
                return fmt_string(ctx, "<oom>");

        ssize_t bytes_read = read_tracee_mem(ctx->tracee, (void *)addr, arr_buf, total);
        if (bytes_read <= 0) {
                free(arr_buf);
                return fmt_string(ctx, "<failed>");
        }

        size_t elements = bytes_read / el.size;

        FMT_STRING(ctx, "[");

        for (size_t i = 0; i < elements; i++) {
                if (i)
                        FMT_SEPARATOR(ctx);

                void *element = arr_buf + i * el.size;

                fmt_array_el(ctx, &el, element);
        }

        FMT_STRING(ctx, "]");

        free(arr_buf);
        return 0;
}

static int fmt_array_el(struct parser_ctx_struct *ctx, struct el_info *el, void *element)
{
        switch (el->type)
        {
        case EL_CSTRING:
                FMT_CSTRING_MEM(ctx, *(unsigned long long *)element);
                break;

        case EL_BYTES:
                FMT_BYTES_MEM(ctx, (unsigned long long)element, el->size);
                break;

        case EL_INT:
                FMT_INT(ctx, *(int *)element);
                break;

        case EL_UINT:
                FMT_UINT(ctx, *(unsigned int *)element);
                break;

        case EL_LONG:
                FMT_LONG(ctx, *(long *)element);
                break;

        case EL_ULONG:
                FMT_ULONG(ctx, *(unsigned long long *)element);
                break;

        case EL_PTR:
                FMT_ADDR(ctx, *(unsigned long long *)element);
                break;

        case EL_FD:
                FMT_FD(ctx, *(int *)element);
                break;

        case EL_STRUCT:
                if (el->fmt_struct_func) {
                        FMT_STRING(ctx, "{");
                        el->fmt_struct_func(ctx, element);
                        FMT_STRING(ctx, "}");
                } else {
                        FMT_STRING(ctx, "{struct}");
                }
                break;

        default:
                FMT_HEX(ctx, *(unsigned char *)element);
                break;
        }

        return 0;
}