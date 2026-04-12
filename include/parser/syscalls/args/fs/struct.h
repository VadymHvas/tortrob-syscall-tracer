#pragma once

#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/statfs.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/fs/value.h"

#define INIT_FIELD_INFO_STXMASK(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field_name, fmt_statx_mask)

#define INIT_FIELD_INFO_STMODE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field_name, fmt_st_mode_flags)

#define INIT_FIELD_INFO_FSTYPE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_FLAGS, struct_type, field_name, fmt_statfs_fs_type)

#define INIT_FIELD_INFO_FFLAGS(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_FLAGS, struct_type, field_name, fmt_fflags_flags)

#define FMT_STAT64_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_stat64_struct, addr, sizeof(struct stat64))

#define FMT_STATX_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statx_struct, addr, sizeof(struct statx))

#define FMT_IOVEC_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_iovec_struct, addr, sizeof(struct iovec))

#define FMT_STATFS_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statfs_struct, addr, sizeof(struct statfs))

int fmt_stat64_struct(struct parser_ctx_struct *ctx, struct stat64 *statbuf);
int fmt_statx_struct(struct parser_ctx_struct *ctx, struct statx *statxbuf);
int fmt_iovec_struct(struct parser_ctx_struct *ctx, struct iovec *iov);
int fmt_statfs_struct(struct parser_ctx_struct *ctx, struct statfs *statfsbuf);