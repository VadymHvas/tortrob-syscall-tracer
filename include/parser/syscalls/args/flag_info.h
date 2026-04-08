#pragma once

#define FOR_EACH_FLAGS(flags_arr) for (int i = 0; i < sizeof(flags_arr) / sizeof(flags_arr[0]); i++)
#define FMT_FLAGS_ZERO_IF_NONE(ctx, flags) \
        if (!(flags)) { \
                FMT_INT((ctx), 0); \
                return 0; \
        }

#define DEFINE_FLAGS_FMT(flags_name) DEFINE_FMT(flags_name##_flags, int flags)

/* 
 * Masks and special cases use the same iteration pattern and zero check as flags,
 * macro is alias for readability.
 */
#define FOR_EACH_MASKS(masks_arr)  FOR_EACH_FLAGS(masks_arr)
#define FOR_EACH_SPECIAL(spec_arr) FOR_EACH_FLAGS(spec_arr)
#define FMT_SPECIAL_ZERO_IF_NONE(ctx, spec) FMT_FLAGS_ZERO_IF_NONE(ctx, spec)
#define FMT_MASK_ZERO_IF_NONE(ctx, mask)    FMT_FLAGS_ZERO_IF_NONE(ctx, mask)

#define FMT_FLAG_SEPARATOR(ctx, first) \
        if (!(first)) \
                FMT_STRING(ctx, "|")

#define DEFINE_MASK_FMT(mask_name)         DEFINE_FMT(mask_name##_mask, unsigned int mask)
#define DEFINE_SPECIAL_FMT(spec_name, ...) DEFINE_FMT(spec_name, __VA_ARGS__)

#define INIT_FLAG_INFO(flag) { flag, #flag }

struct flag_info {
        int flag;
        const char *name;
};