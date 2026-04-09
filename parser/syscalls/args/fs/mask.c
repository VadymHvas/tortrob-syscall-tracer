#define _GNU_SOURCE

#include <unistd.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/flags.h"
#include "parser/syscalls/args/fs/mask.h"

DEFINE_FLAGS_ARRAY(statx_masks) = {
        INIT_FLAG_INFO(STATX_TYPE),
        INIT_FLAG_INFO(STATX_MODE),
        INIT_FLAG_INFO(STATX_NLINK),
        INIT_FLAG_INFO(STATX_UID),
        INIT_FLAG_INFO(STATX_GID),
        INIT_FLAG_INFO(STATX_ATIME),
        INIT_FLAG_INFO(STATX_MTIME),
        INIT_FLAG_INFO(STATX_CTIME),
        INIT_FLAG_INFO(STATX_INO),
        INIT_FLAG_INFO(STATX_SIZE),
        INIT_FLAG_INFO(STATX_BLOCKS),
        INIT_FLAG_INFO(STATX_BTIME),
        INIT_FLAG_INFO(STATX_ALL)
};

DEFINE_FLAGS_ARRAY(access_masks) = {
        INIT_FLAG_INFO(R_OK),
        INIT_FLAG_INFO(W_OK),
        INIT_FLAG_INFO(X_OK)
};

/*
 * TODO: add unrecognized bits formatting
 */

DEFINE_MASK_FMT(statx)
{
        FMT_MASK_ZERO_IF_NONE(ctx, mask);

        int first = 1;

        if ((mask & STATX_BASIC_STATS) == STATX_BASIC_STATS) {
                FMT_STRING(ctx, "STATX_BASIC_STATS");

                mask &= ~STATX_BASIC_STATS;
                first = 0;
        }

        FOR_EACH_MASKS(statx_masks) {
                if (mask & statx_masks[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first);
                        FMT_STRING(ctx, statx_masks[i].name);

                        mask &= ~statx_masks[i].flag;
                        first = 0;
                }
        }

        return 0;
}

DEFINE_MASK_FMT(access)
{
        if (!mask) {
                FMT_STRING(ctx, "F_OK");
                return 0;
        }

        int first = 1;

        FOR_EACH_MASKS(access_masks) {
                if (mask & access_masks[i].flag) {
                        FMT_FLAG_SEPARATOR(ctx, first);
                        FMT_STRING(ctx, access_masks[i].name);

                        mask &= ~statx_masks[i].flag;
                        first = 0;
                }
        }

        return 0;
}