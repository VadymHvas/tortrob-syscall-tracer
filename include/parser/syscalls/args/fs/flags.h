#pragma once

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"

#define FMT_OPEN_FLAGS(ctx, flags)         TRY_FMT(fmt_open_flags, ctx, flags)
#define FMT_AT_FLAGS(ctx, flags)           TRY_FMT(fmt_at_flags, ctx, flags)
#define FMT_RENAMEAT2_FLAGS(ctx, flags)    TRY_FMT(fmt_renameat2_flags, ctx, flags)
#define FMT_DUP3_FLAGS(ctx, flags)         TRY_FMT(fmt_dup3_flags, ctx, flags)
#define FMT_RWF_FLAGS(ctx, flags)          TRY_FMT(fmt_rwf_flags, ctx, flags)
#define FMT_XATTR_FLAGS(ctx, flags)        TRY_FMT(fmt_xattr_flags, ctx, flags)
#define FMT_SPLICE_FLAGS(ctx, flags)       TRY_FMT(fmt_splice_flags, ctx, flags)
#define FMT_INOTIFY_FLAGS(ctx, flags)      TRY_FMT(fmt_inotify_flags, ctx, flags)
#define FMT_MOUNT_FLAGS(ctx, flags)        TRY_FMT(fmt_mount_flags, ctx, flags)
#define FMT_UMOUNT2_FLAGS(ctx, flags)      TRY_FMT(fmt_umount2_flags, ctx, flags)
#define FMT_FSOPEN_FLAGS(ctx, flags)       TRY_FMT(fmt_fsopen_flags, ctx, flags)
#define FMT_FSMOUNT_FLAGS(ctx, flags)      TRY_FMT(fmt_fsmount_flags, ctx, flags)
#define FMT_FSMOUNT_ATTR_FLAGS(ctx, flags) TRY_FMT(fmt_fsmount_attr_flags, ctx, flags)
#define FMT_FSPICK_FLAGS(ctx, flags)       TRY_FMT(fmt_fspick_flags, ctx, flags)
#define FMT_OPEN_TREE_FLAGS(ctx, flags)    TRY_FMT(fmt_open_tree_flags, ctx, flags)
#define FMT_MOVE_MOUNT_FLAGS(ctx, flags)   TRY_FMT(fmt_move_mount_flags, ctx, flags)
#define FMT_PIPE2_FLAGS(ctx, flags)        TRY_FMT(fmt_pipe2_flags, ctx, flags)

struct parser_ctx_struct;

int fmt_open_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_at_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_renameat2_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_dup3_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_rwf_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_st_mode_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_fflags_flags(struct parser_ctx_struct *ctx, long flags);
int fmt_xattr_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_splice_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_inotify_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_mount_flags(struct parser_ctx_struct *ctx, unsigned long flags);
int fmt_umount2_flags(struct parser_ctx_struct *ctx, int flags);
int fmt_fsopen_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_fsmount_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_fsmount_attr_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_fspick_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_open_tree_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_move_mount_flags(struct parser_ctx_struct *ctx, unsigned int flags);
int fmt_pipe2_flags(struct parser_ctx_struct *ctx, int flags);