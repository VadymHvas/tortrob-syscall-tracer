#pragma once

#define FOR_EACH_FLAGS(flags_arr) for (int i = 0; i < sizeof(flags_arr) / sizeof(flags_arr[0]); i++)
#define FMT_FLAGS_ZERO_IF_NONE(ctx, flags) \
        if (!(flags)) { \
                FMT_INT((ctx), 0); \
                return 0; \
        }

/* 
 * Masks and special cases use the same iteration pattern and zero check as flags,
 * macro is alias for readability.
 */
#define FOR_EACH_MASKS(masks_arr)  FOR_EACH_FLAGS(masks_arr)
#define FOR_EACH_SPECIAL(spec_arr) FOR_EACH_FLAGS(spec_arr)
#define FMT_SPECIAL_ZERO_IF_NONE(ctx, spec) FMT_FLAGS_ZERO_IF_NONE(ctx, spec)
#define FMT_MASK_ZERO_IF_NONE(ctx, mask)    FMT_FLAGS_ZERO_IF_NONE(ctx, mask)

/*
 * FMT_FLAG_SEPARATOR() - This is a universal macro, it is separated
 * from the context of use by masks or flags, 
 * so the additional macro FMT_MASK_SEPARATOR() is not needed.
 */
#define FMT_FLAG_SEPARATOR(ctx, first) \
        if (!(first)) \
                FMT_STRING(ctx, "|")

#define DEFINE_FLAGS_ARRAY(name) static const struct flag_info name[]
#define INIT_FLAG_INFO(flag) { flag, #flag }

/**
 * struct flag_info - Flag presentation.
 * 
 * @flag: Flag value.
 * @name: Flag name.
 * 
 * This structure stores all the necessary
 * information for correct flags, masks, etc formatting.
 */
struct flag_info {
        unsigned long long flag;
        const char *name;
};