#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/uio.h>

#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/fs/struct.h"
#include "parser/syscalls/args/fs/mask.h"

DEFINE_FIELDS_ARRAY(stat64_fields) = {
        INIT_FIELD_INFO_INO(struct stat64, st_ino),
        INIT_FIELD_INFO_ULONG(struct stat64, st_nlink),
        INIT_FIELD_INFO_STMODE(struct stat64, st_mode),
        INIT_FIELD_INFO_UINT(struct stat64, st_uid),
        INIT_FIELD_INFO_UINT(struct stat64, st_gid),
        INIT_FIELD_INFO_DEV(struct stat64, st_rdev),
        INIT_FIELD_INFO_ULONG(struct stat64, st_size),
        INIT_FIELD_INFO_LONG(struct stat64, st_blksize),
        INIT_FIELD_INFO_LONG(struct stat64, st_blocks),
};

DEFINE_FIELDS_ARRAY(statx_fields) = {
        INIT_FIELD_INFO_MASK(struct statx, stx_mask, fmt_statx_mask),
        INIT_FIELD_INFO_INT(struct statx, stx_blksize),

        INIT_FIELD_INFO_INT(struct statx, stx_nlink),
        INIT_FIELD_INFO_UINT(struct statx, stx_uid),
        INIT_FIELD_INFO_UINT(struct statx, stx_gid),
        INIT_FIELD_INFO_STMODE(struct statx, stx_mode),

        INIT_FIELD_INFO_INO(struct statx, stx_ino),
        INIT_FIELD_INFO_ULONG(struct statx, stx_size)
};

DEFINE_FIELDS_ARRAY(iovec_fields) = {
        INIT_FIELD_INFO_PTR(struct iovec, iov_base),
        INIT_FIELD_INFO_ULONG(struct iovec, iov_len) 
};

DEFINE_STRUCT_FMT(stat64, struct stat64 *statbuf)
{
        return fmt_struct_generic(ctx, statbuf, stat64_fields, FIELDS_ARR_SIZE(stat64_fields));
}

DEFINE_STRUCT_FMT(statx, struct statx *statxbuf)
{
        return fmt_struct_generic(ctx, statxbuf, statx_fields, FIELDS_ARR_SIZE(statx_fields));
}

DEFINE_STRUCT_FMT(iovec, struct iovec *iov)
{
        return fmt_struct_generic(ctx, iov, iovec_fields, FIELDS_ARR_SIZE(iovec_fields));
}