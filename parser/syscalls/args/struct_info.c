#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/helpers.h"

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

int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, field_repr repr)
{
        switch (repr)
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