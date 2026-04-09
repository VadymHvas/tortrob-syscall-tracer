#define _GNU_SOURCE

#include <sys/stat.h>

#include "parser/syscalls/args/struct_info.h"
#include "parser/syscalls/args/fs/struct.h"

DEFINE_FIELDS_ARRAY(stat64_fields) = {
        INIT_FIELD_INFO(NUM, struct stat64, st_ino),
        INIT_FIELD_INFO(NUM, struct stat64, st_nlink),
        INIT_FIELD_INFO(MODE, struct stat64, st_mode),
        INIT_FIELD_INFO(NUM, struct stat64, st_uid),
        INIT_FIELD_INFO(NUM, struct stat64, st_gid),
        INIT_FIELD_INFO(NUM, struct stat64, st_rdev),
        INIT_FIELD_INFO(NUM, struct stat64, st_size),
        INIT_FIELD_INFO(NUM, struct stat64, st_blksize),
        INIT_FIELD_INFO(NUM, struct stat64, st_blocks),
};

DEFINE_STRUCT_FMT(stat64, struct stat64 *statbuf)
{
        int first = 1;

        for (int i = 0; i < sizeof(stat64_fields) / sizeof(stat64_fields[0]); i++) {
                if (!first)
                        FMT_SEPARATOR(ctx);
                
                FMT_STRING(ctx, stat64_fields[i].name);
                FMT_STRING(ctx, "=");
                FMT_INT(ctx, (int)((char *)statbuf + stat64_fields[i].offset));

                first = 0;
        }

        return 0;
}