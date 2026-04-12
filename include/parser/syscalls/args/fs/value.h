#pragma once

#include "parser/syscalls/args/helpers.h"

#define DEFINE_VALUES_ARRAY(name) static const struct value_info name[]

#define FOR_EACH_VALUES(values_arr) for (int i = 0; i < sizeof(values_arr) / sizeof(values_arr[0]); i++)

#define FMT_FCNTL_CMD(ctx, cmd)           TRY_FMT(fmt_fcntl_cmd, ctx, cmd)
#define FMT_FLOCK_OP(ctx, op)             TRY_FMT(fmt_flock_op, ctx, op)
#define FMT_FADVISE64_ADVICE(ctx, advice) TRY_FMT(fmt_fadvise64_advice, ctx, advice)

#define INIT_VALUE(value) { value, #value }

struct value_info {
        unsigned long long value;
        const char *name;
};

DEFINE_FMT(fcntl_cmd, int cmd);
DEFINE_FMT(flock_op, int op);
DEFINE_FMT(fadvise64_advice, int advice);
DEFINE_FMT(statfs_fs_type, int fs_type);