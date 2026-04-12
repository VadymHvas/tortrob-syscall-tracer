#include <stdlib.h>

#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/helpers.h"
#include "core/trace.h"

unsigned long long read_field(void *ptr, field_type type)
{
        switch (type) {
        case TYPE_INT:   
                return *(int *)ptr;
        case TYPE_UINT:  
                return *(unsigned int *)ptr;
        case TYPE_LONG:  
                return *(long *)ptr;
        case TYPE_ULONG: 
                return *(unsigned long *)ptr;
        case TYPE_OFF:   
                return *(long *)ptr;
        case TYPE_INO:   
                return *(unsigned long *)ptr;
        case TYPE_MODE:  
                return *(unsigned int *)ptr;
        case TYPE_DEV:   
                return *(unsigned long *)ptr;
        case TYPE_PTR:   
                return (unsigned long long)*(void **)ptr;
        }
    return 0;
}

int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, struct field_info *field)
{
        switch (field->field_repr)
        {
        case REPR_DEC:
                FMT_INT(ctx, value);
                break;
        case REPR_HEX:
                FMT_HEX(ctx, value);
                break;
        case REPR_OCT:
                FMT_OCT(ctx, value);
                break;
        case REPR_ADDR:
                FMT_ADDR(ctx, value);
                break;
        case REPR_FLAGS:
                TRY_FMT(field->field_flags_func, ctx, value);
                break;
        case REPR_FD:
                FMT_FD(ctx, value);
                break;
        case REPR_DEV:
                FMT_DEV(ctx, value);
                break;
        case REPR_MODE:
                FMT_OCT(ctx, value);
                break;
        default:
                break;
        }

        return 0;
}

int fmt_struct_common(struct parser_ctx_struct *ctx,
                fmt_struct_func_t fmt_struct_func, unsigned long long addr, size_t size)
{
        if (!addr)
                return fmt_null(ctx);
        
        void *struct_buf = malloc(size);

        if (!struct_buf)
                return fmt_string(ctx, "<failed>");

        if (read_tracee_mem(ctx->tracee, addr, struct_buf, size) <= 0)
                goto err;

        if (fmt_string(ctx, "{") ||
            fmt_struct_func(ctx, struct_buf) || 
            fmt_string(ctx, "}"))
                goto err;

        free(struct_buf);
        return 0;

err:
        free(struct_buf);
        return 1;
}

int fmt_struct_generic(struct parser_ctx_struct *ctx,
        void *st, struct field_info *fields, size_t fields_count)
{
        int first = 1;

        for (int i = 0; i < fields_count; i++) {
                if (!first)
                        FMT_SEPARATOR(ctx);

                void *field_ptr = (char *)st + fields[i].offset;

                unsigned long long value = read_field(field_ptr, fields[i].field_type);

                FMT_STRING(ctx, fields[i].name);
                FMT_STRING(ctx, "=");
                TRY_FMT(repr_field, ctx, value, &fields[i]);

                first = 0;
        }

        return 0;
}