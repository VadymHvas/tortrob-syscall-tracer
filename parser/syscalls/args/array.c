#include "parser/syscalls/args/array.h"
#include "parser/syscalls/args/helpers.h"
#include "core/trace.h"

static int fmt_array_el(struct parser_ctx_struct *ctx, struct el_info *el, unsigned long long value);

int fmt_array_sized_common(struct parser_ctx_struct *ctx, 
        unsigned long long addr, size_t size, struct el_info el)
{
        if (!addr)
                return fmt_null(ctx);

        if (!size)
                return fmt_string(ctx, "[]");

        char arr_buf[size * el.size];
        
        ssize_t bytes_read = read_tracee_mem(ctx->tracee, (void *)addr, arr_buf, size * el.size);
        if (bytes_read <= 0)
                return fmt_string(ctx, "<failed>");

        FMT_STRING(ctx, "[");

        int first = 1;
        for (int i = 0; i < size; i++) {
                if (!first)
                        FMT_SEPARATOR(ctx);

                char *element = arr_buf + i * el.size;
                
                fmt_array_el(ctx, &el, element);

                first = 0;
        }

        FMT_STRING(ctx, "]");

        return 0;
}

static int fmt_array_el(struct parser_ctx_struct *ctx, struct el_info *el, unsigned long long value)
{
        switch (el->type)
                {
                case EL_CSTRING:
                        FMT_CSTRING_MEM(ctx, (unsigned long long)value);
                        break;

                case EL_BYTES:
                        FMT_BYTES_MEM(ctx, (unsigned long long)value, 48);
                        break;

                case EL_INT:
                        FMT_INT(ctx, (int)value);
                        break;

                case EL_UINT:
                        FMT_INT(ctx, (unsigned int)value);
                        break;
                
                case EL_LONG:
                        FMT_LLU(ctx, (long)value);
                        break;

                case EL_ULONG:
                        FMT_LLU(ctx, (unsigned long long)value);
                        break;

                case EL_PTR:
                        FMT_ADDR(ctx, (unsigned long long)value);
                        break;

                case EL_STRUCT:
                        if (el->fmt_struct_func) {
                                FMT_STRING(ctx, "{");
                                el->fmt_struct_func(ctx, value);
                                FMT_STRING(ctx, "}");
                        } else {
                                FMT_STRING(ctx, "{struct}");
                        }
                        break;

                default:
                        FMT_HEX(ctx, *(int *)value);
                        break;
                }

        return 0;
}