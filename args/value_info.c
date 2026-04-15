/*
 * args/value_info.c - Generic value formatting functions.
 *
 * This file provides a generic function to format integer values based on a provided 
 * mapping of values to their string representations. It is used to format syscall arguments 
 * that have specific known values, such as flags or enums.
 */

#include "args/helpers.h"
#include "args/value_info.h"

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