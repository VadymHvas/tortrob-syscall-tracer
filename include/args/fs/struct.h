#pragma once

#define _GNU_SOURCE

#include <fcntl.h>

#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/statfs.h>

#include "args/helpers.h"
#include "args/struct_info.h"
#include "args/fs/value.h"

#define INIT_FIELD_INFO_STXMASK(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field_name, fmt_statx_mask)

#define INIT_FIELD_INFO_STMODE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field_name, fmt_st_mode_flags)

#define INIT_FIELD_INFO_FSTYPE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_FLAGS, struct_type, field_name, fmt_statfs_fs_type)

#define INIT_FIELD_INFO_FFLAGS(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_FLAGS, struct_type, field_name, fmt_fflags_flags)

#define INIT_FIELD_INFO_FLOCK_LTYPE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_SHORT, REPR_FLAGS, struct_type, field_name, fmt_flock_flags)

#define INIT_FIELD_INFO_FLOCK_LWHENCE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_SHORT, REPR_FLAGS, struct_type, field_name, fmt_flock_whence)

#define FMT_STAT64_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_stat64_struct, addr, sizeof(struct stat64))

#define FMT_STATX_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statx_struct, addr, sizeof(struct statx))

#define FMT_IOVEC_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_iovec_struct, addr, sizeof(struct iovec))

#define FMT_STATFS_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statfs_struct, addr, sizeof(struct statfs))

#define FMT_FLOCK_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_flock_struct, addr, sizeof(struct flock))

int fmt_stat64_struct(struct parser_ctx_struct *ctx, struct stat64 *statbuf);
int fmt_statx_struct(struct parser_ctx_struct *ctx, struct statx *statxbuf);
int fmt_iovec_struct(struct parser_ctx_struct *ctx, struct iovec *iov);
int fmt_statfs_struct(struct parser_ctx_struct *ctx, struct statfs *statfsbuf);
int fmt_timespec_struct(struct parser_ctx_struct *ctx, struct timespec *timespec);
int fmt_flock_struct(struct parser_ctx_struct *ctx, struct flock *lock);