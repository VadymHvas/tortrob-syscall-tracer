#pragma once

#include <time.h>

#include "args/helpers.h"
#include "args/array.h"
#include "args/fs/struct.h"

#define FMT_TIMESPEC_ARR(ctx, addr, size) \
        FMT_SIZED_ARR(ctx, addr, size, EL_STRUCT, sizeof(struct timespec), fmt_timespec_struct)