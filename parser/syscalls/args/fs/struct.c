#define _GNU_SOURCE

#include <sys/stat.h>

#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/fs/struct.h"

DEFINE_FIELDS_ARRAY(stat64_fields) = {
        INIT_FIELD_INFO_INO(struct stat64, st_ino),
        INIT_FIELD_INFO_ULONG(struct stat64, st_nlink),
        INIT_FIELD_INFO_MODE(struct stat64, st_mode),
        INIT_FIELD_INFO_UINT(struct stat64, st_uid),
        INIT_FIELD_INFO_UINT(struct stat64, st_gid),
        INIT_FIELD_INFO_DEV(struct stat64, st_rdev),
        INIT_FIELD_INFO_OFF(struct stat64, st_size),
        INIT_FIELD_INFO_LONG(struct stat64, st_blksize),
        INIT_FIELD_INFO_LONG(struct stat64, st_blocks),
};

DEFINE_STRUCT_FMT(stat64, struct stat64 *statbuf)
{
        int first = 1;

        FOR_EACH_FIELDS(stat64_fields) {
                if (!first)
                        FMT_SEPARATOR(ctx);

                void *field = (char *)statbuf + stat64_fields[i].offset;

                unsigned long long value = read_field(field, stat64_fields[i].field_type);

                FMT_STRING(ctx, stat64_fields[i].name);
                FMT_STRING(ctx, "=");
                TRY_FMT(repr_field, ctx, value, stat64_fields[i].field_repr);

                first = 0;
        }

        return 0;
}