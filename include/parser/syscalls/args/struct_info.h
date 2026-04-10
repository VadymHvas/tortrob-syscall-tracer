#pragma once

#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"

#define DEFINE_STRUCT_FMT(struct_name, struct_type) DEFINE_FMT(struct_name##_struct, struct_type)
#define DEFINE_FIELDS_ARRAY(name) static const struct field_info name[]

#define FIELDS_ARR_SIZE(fields_arr) sizeof(fields_arr) / sizeof(fields_arr[0])

#define INIT_FIELD_INFO(type, repr, struct_type, field_name, flags_func) \
        { \
                .field_type       = type, \
                .field_repr       = repr, \
                .offset           = offsetof(struct_type, field_name), \
                .name             = #field_name, \
                .field_flags_func = flags_func \
        }

#define INIT_FIELD_INFO_INT(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_UINT(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_UINT, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_LONG(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_ULONG(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_ULONG, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_PTR(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_PTR, REPR_ADDR, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_INO(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INO, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_OFF(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_OFF, REPR_DEC, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_DEV(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_DEV, REPR_DEV, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_MODE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_MODE, REPR_MODE, struct_type, field_name, NULL)

#define INIT_FIELD_INFO_MASK(struct_type, field_name, flags_func) \
        INIT_FIELD_INFO(TYPE_INT, REPR_FLAGS, struct_type, field_name, flags_func)

typedef enum {
        TYPE_INT,
        TYPE_UINT,
        TYPE_LONG,
        TYPE_ULONG,
        TYPE_PTR,
        TYPE_INO,       // ino_t
        TYPE_OFF,       // off_t
        TYPE_DEV,       // dev_t
        TYPE_MODE       // mode_t
} field_type;

typedef enum {
        REPR_DEC,
        REPR_HEX,
        REPR_OCT,
        REPR_ADDR,
        REPR_FLAGS,
        REPR_FD,
        REPR_DEV,
        REPR_MODE
} field_repr;

typedef (*field_flags_func_t) (struct parser_ctx_struct *ctx, int flags);

/**
 */
struct field_info {
        field_type field_type;
        field_repr field_repr;
        size_t offset;
        const char *name;
        field_flags_func_t field_flags_func;
};

int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, struct field_info *field);
unsigned long long read_field(void *field, field_type type);
DEFINE_FMT(struct_generic, void *st, struct field_info *fields, size_t fields_count);