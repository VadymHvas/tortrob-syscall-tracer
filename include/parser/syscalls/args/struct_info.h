#pragma once

#include <stddef.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"

#define DEFINE_STRUCT_FMT(name, st) DEFINE_FMT(name##_struct, st)

#define DEFINE_FIELDS_ARRAY(name) static const struct field_info name[]
#define INIT_FIELD_INFO(type, struct, name) { type, offsetof(struct, name), #name}

enum field_type {
        NUM,
        FD,
        FLAGS,
        ADDR,
        MODE
};

struct field_info {
        int field_type;
        size_t offset;
        const char *name;
};