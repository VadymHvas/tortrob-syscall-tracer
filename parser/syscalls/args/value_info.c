#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/value_info.h"

int fmt_value_generic(struct parser_ctx_struct *ctx, int value,
                const struct value_info *values, size_t values_count)
{
        if (!value) {
                FMT_INT(ctx, 0);
                return 0;
        }

        for (int i = 0; i < values_count; i++) {
                if (value == values[i].value) {
                        FMT_STRING(ctx, values[i].name);
                        return 0;
                }
        }

        FMT_HEX(ctx, value);
        return 0;
}