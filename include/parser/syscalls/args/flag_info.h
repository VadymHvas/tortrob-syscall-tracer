#pragma once

#define FOR_EACH_FLAGS(flags_arr) for (int i = 0; i < sizeof(flags_arr) / sizeof(flags_arr[0]); i++)
#define FMT_FLAGS_ZERO_IF_NONE(ctx, flags) \
        if (!(flags)) { \
                FMT_INT((ctx), 0); \
                return 0; \
        }

/* Masks use the same iteration pattern and zero check as flags,
 * macro is alias for readability.
 */
#define FOR_EACH_MASKS(masks_arr) FOR_EACH_FLAGS(masks_arr)
#define FMT_MASK_ZERO_IF_NONE(ctx, mask) FMT_FLAGS_ZERO_IF_NONE(ctx, mask)

#define FMT_FLAG_SEPARATOR(ctx, first) \
        if (!(first)) \
                FMT_STRING(ctx, "|")

#define INIT_FLAG_INFO(flag) { flag, #flag }

struct flag_info {
        int flag;
        const char *name;
};