/*
 * args/flag_info.c - Generic flag formatter for syscall arguments.
 *
 * Converts bitmask flags into human-readable names using a provided
 * flag table, and prints remaining unknown bits as hex.
 *
 * Used for decoding common Linux syscall flag arguments (fs, fcntl,
 * mount-related flags, etc.).
 */

#include <sys/types.h>

#include "args/helpers.h"
#include "args/flag_info.h"

int fmt_flags_generic(struct parser_ctx_struct *ctx, 
        int flags, const struct flag_info *flags_arr, size_t flags_arr_count, int is_first)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = is_first;

        for (int i = 0; i < flags_arr_count; i++) {
                if (flags & flags_arr[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first);
                        FMT_STRING(ctx, flags_arr[i].name);

                        first = 0;
                        flags &= ~flags_arr[i].flag;
                }
        }

        if (flags) {
                FMT_FLAG_SEPARATOR(ctx, first);
                FMT_HEX(ctx, flags);
        }

        return 0;
}