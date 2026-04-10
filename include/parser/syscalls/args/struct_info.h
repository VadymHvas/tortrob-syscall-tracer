#pragma once

#include <stddef.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"

#define DEFINE_STRUCT_FMT(struct_name, struct_type) DEFINE_FMT(struct_name##_struct, struct_type)
#define DEFINE_FIELDS_ARRAY(name) static const struct field_info name[]

#define FOR_EACH_FIELDS(fields_arr) for (int i = 0; i < sizeof(fields_arr) / sizeof(fields_arr[0]); i++)

#define GET_FIELD_PTR_BY_OFFSET(st, offset) (char *)st + offset

#define INIT_FIELD_INFO(type, repr, struct_type, field_name) \
        { \
                .field_type  = type, \
                .field_repr  = repr, \
                .offset      = offsetof(struct_type, field_name), \
                .name        = #field_name \
        }

#define INIT_FIELD_INFO_INT(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INT, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_UINT(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_UINT, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_LONG(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_LONG, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_ULONG(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_ULONG, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_PTR(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_PTR, REPR_ADDR, struct_type, field_name)

#define INIT_FIELD_INFO_INO(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_INO, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_OFF(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_OFF, REPR_DEC, struct_type, field_name)

#define INIT_FIELD_INFO_DEV(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_DEV, REPR_DEV, struct_type, field_name)

#define INIT_FIELD_INFO_MODE(struct_type, field_name) \
        INIT_FIELD_INFO(TYPE_MODE, REPR_MODE, struct_type, field_name)

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

struct field_info {
        field_type field_type;
        field_repr field_repr;
        size_t offset;
        const char *name;
};

int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, field_repr repr);
unsigned long long read_field(void *field, field_type type);