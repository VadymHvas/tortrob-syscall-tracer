#pragma once

#include <time.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/array.h"
#include "parser/syscalls/args/fs/struct.h"

#define FMT_TIMESPEC_ARR(ctx, addr, size) \
        FMT_SIZED_ARR(ctx, addr, size, EL_STRUCT, sizeof(struct timespec), fmt_timespec_struct)