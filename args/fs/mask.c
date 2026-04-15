/*
 * args/fs/mask.c - Formatting functions for filesystem-related masks.
 */

#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/inotify.h>

#include "args/helpers.h"
#include "args/flag_info.h"
#include "args/fs/flags.h"
#include "args/fs/mask.h"

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

DEFINE_FLAGS_ARRAY(fallocate_masks) = {
        INIT_FLAG_INFO(FALLOC_FL_KEEP_SIZE),
        INIT_FLAG_INFO(FALLOC_FL_UNSHARE_RANGE),
        INIT_FLAG_INFO(FALLOC_FL_PUNCH_HOLE),
        INIT_FLAG_INFO(FALLOC_FL_COLLAPSE_RANGE),
        INIT_FLAG_INFO(FALLOC_FL_ZERO_RANGE),
        INIT_FLAG_INFO(FALLOC_FL_INSERT_RANGE),
};

DEFINE_FLAGS_ARRAY(inotify_masks) = {
        INIT_FLAG_INFO(IN_ACCESS),
        INIT_FLAG_INFO(IN_ATTRIB),
        INIT_FLAG_INFO(IN_CLOSE_WRITE),
        INIT_FLAG_INFO(IN_CLOSE_NOWRITE),
        INIT_FLAG_INFO(IN_CREATE),
        INIT_FLAG_INFO(IN_DELETE),
        INIT_FLAG_INFO(IN_DELETE_SELF),
        INIT_FLAG_INFO(IN_MODIFY),
        INIT_FLAG_INFO(IN_MOVE_SELF),
        INIT_FLAG_INFO(IN_MOVED_FROM),
        INIT_FLAG_INFO(IN_MOVED_TO),
        INIT_FLAG_INFO(IN_OPEN),
        INIT_FLAG_INFO(IN_DONT_FOLLOW),
        INIT_FLAG_INFO(IN_EXCL_UNLINK),
        INIT_FLAG_INFO(IN_MASK_ADD),
        INIT_FLAG_INFO(IN_ONESHOT),
        INIT_FLAG_INFO(IN_ONLYDIR),
};

int fmt_statx_mask(struct parser_ctx_struct *ctx, unsigned int mask)
{
        FMT_MASK_ZERO_IF_NONE(ctx, mask);

        int first = 1;

        if ((mask & STATX_BASIC_STATS) == STATX_BASIC_STATS) {
                FMT_STRING(ctx, "STATX_BASIC_STATS");

                mask &= ~STATX_BASIC_STATS;
                first = 0;
        }

        return fmt_flags_generic(
                ctx,
                mask,
                statx_masks,
                FLAGS_ARR_SIZE(statx_masks),
                first
        );
}

int fmt_access_mask(struct parser_ctx_struct *ctx, int mask)
{
        if (!mask) {
                FMT_STRING(ctx, "F_OK");
                return 0;
        }

        return fmt_flags_generic(
                ctx,
                mask,
                access_masks,
                FLAGS_ARR_SIZE(access_masks), 
                1
        );
}

int fmt_fallocate_mask(struct parser_ctx_struct *ctx, int mask)
{
        return fmt_flags_generic(
                ctx,
                mask,
                fallocate_masks,
                FLAGS_ARR_SIZE(fallocate_masks), 
                1
        );
}

int fmt_inotify_mask(struct parser_ctx_struct *ctx, unsigned int mask)
{
        return fmt_flags_generic(
                ctx,
                mask,
                inotify_masks,
                FLAGS_ARR_SIZE(inotify_masks),
                1
        );
}