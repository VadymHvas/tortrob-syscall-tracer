#pragma once

#define FOR_EACH_FLAGS(flags_arr) for (int i = 0; i < sizeof(flags_arr) / sizeof(flags_arr[0]); i++)

// Masks use the same iteration pattern as flags, macro is alias for readability
#define FOR_EACH_MASKS(masks_arr) FOR_EACH_FLAGS(masks_arr)

struct flag_info {
        int flag;
        const char *name;
};