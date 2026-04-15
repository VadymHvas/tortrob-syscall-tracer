#include <stdlib.h>

#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/helpers.h"
#include "core/trace.h"

static unsigned long long read_field(void *ptr, field_type type);
static int field_repr_dec(struct parser_ctx_struct *ctx, unsigned long long num, field_type num_type);
static int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, struct field_info *field);

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

static unsigned long long read_field(void *ptr, field_type type)
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
        case TYPE_SHORT:
                return *(short *)ptr;
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

static int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, struct field_info *field)
{
        switch (field->field_repr)
        {
        case REPR_DEC:
                field_repr_dec(ctx, value, field->field_type);
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
                FMT_STRING(ctx, "<unknown type>");
                break;
        }

        return 0;
}

static int field_repr_dec(struct parser_ctx_struct *ctx, unsigned long long num, field_type num_type)
{
        switch (num_type)
        {
        case TYPE_INT:
        case TYPE_SHORT:
                FMT_INT(ctx, num);
                break;

        case TYPE_UINT:
                FMT_UINT(ctx, num);
                break;
        
        case TYPE_LONG:
                FMT_LONG(ctx, num);
                break;

        case TYPE_ULONG:
                FMT_ULONG(ctx, num);
                break;

        case TYPE_INO:
                FMT_ULONG(ctx, num);
                break;

        case TYPE_OFF:
                FMT_OFF(ctx, num);
                break;

        default:
                FMT_LLU(ctx, num);
                break;
        }

        return 0;
}