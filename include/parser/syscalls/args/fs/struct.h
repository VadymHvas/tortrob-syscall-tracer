#pragma once

#define _GNU_SOURCE

#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/struct_info.h"

#define FMT_STAT64_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_stat64_struct, addr, sizeof(struct stat64))

#define FMT_STATX_STRUCT(ctx, addr) \
        TRY_FMT(fmt_struct_common, ctx, fmt_statx_struct, addr, sizeof(struct statx))

DEFINE_STRUCT_FMT(stat64, struct stat64 *statbuf);
DEFINE_STRUCT_FMT(statx, struct statx *statxbuf);