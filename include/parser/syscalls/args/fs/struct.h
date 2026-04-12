#pragma once

#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/statfs.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/struct_info.h"

#define FMT_STAT64_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_stat64_struct, addr, sizeof(struct stat64))

#define FMT_STATX_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statx_struct, addr, sizeof(struct statx))

#define FMT_IOVEC_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_iovec_struct, addr, sizeof(struct iovec))

#define FMT_STATFS_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statfs_struct, addr, sizeof(struct statfs))

DEFINE_STRUCT_FMT(stat64, struct stat64 *statbuf);
DEFINE_STRUCT_FMT(statx, struct statx *statxbuf);
DEFINE_STRUCT_FMT(iovec, struct iovec *iov);
DEFINE_STRUCT_FMT(statfs, struct statfs *statfsbuf);