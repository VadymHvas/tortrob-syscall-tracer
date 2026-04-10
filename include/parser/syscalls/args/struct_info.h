#pragma once

#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>

#include "parser/syscalls/args/helpers.h"

#define DEFINE_STRUCT_FMT(struct_name, struct_type) DEFINE_FMT(struct_name##_struct, struct_type)
#define DEFINE_FIELDS_ARRAY(name) static const struct field_info name[]

/* This macro takes an array as an argument, not a pointer. */
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

/**
 * enum field_type describes semantic interpretation of a struct field,
 * including both its underlying type and formatting behavior.
 * 
 * TYPE_INT   - signed integer (REPR_DEC)
 * TYPE_UINT  - unsigned integer (REPR_DEC)
 * TYPE_LONG  - signed long integer (REPR_DEC)
 * TYPE_ULONG - unsigned long integer (REPR_DEC)
 * TYPE_PTR   - pointer value (REPR_ADDR)
 * TYPE_INO   - inode number (ino_t, REPR_DEC)
 * TYPE_OFF   - offset value (off_t, REPR_DEC)
 * TYPE_DEV   - device ID (dev_t, REPR_DEV "major:minor")
 * TYPE_MODE  - file mode (mode_t, REPR_OCT)
 * 
 * Used in:
 * - field_info initialization
 * - read_field() for extracting values
 */
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

/**
 * enum field_repr defines output representation of a field in the formatting engine.
 * 
 * This controls formatting stage after value extraction (see field_type).
 * 
 * REPR_DEC   - Decimal (FMT_INT)
 * REPR_HEX   - Hexadecimal (FMT_HEX)
 * REPR_OCT   - Octal (FMT_OCT)
 * REPR_ADDR  - Address (FMT_ADDR)
 * REPR_FLAGS - Bitmask flags (formatted using custom flag formatter)
 * REPR_FD    - File descriptor (FMT_FD)
 * REPR_DEV   - Device ID (FMT_DEV)
 * REPR_MODE  - File mode (printed as octal, FMT_OCT)
 * 
 * Used in:
 * - field_info initialization.
 * - repr_field() for formatting output.
 */
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

/**
 * fmt_struct_func_t - Function pointer type for struct formatters.
 *
 * @ctx: Parser context.
 * @st:  Pointer to struct being formatted (generic, cast internally).
 *
 * Return: 0 on success, non-zero on error.
 * Used in: fmt_struct_common() to dispatch specific struct formatters.
 */
typedef int (*fmt_struct_func_t)(struct parser_ctx_struct *ctx, void *st);

/**
 * field_flags_func_t - Function pointer type for bitmask flags formatters.
 * 
 * @ctx: Parser context.
 * @flags: Bitmask flags.
 * 
 * Return: 0 on success, non-zero on error.
 * Used in: repr_field() to format bitmask flags.
 */
typedef int (*field_flags_func_t) (struct parser_ctx_struct *ctx, unsigned long long flags);

/**
 * struct field_info - Describes a struct field for the formatting engine.
 *
 * This structure provides metadata required to:
 * - locate the field in memory (offset)
 * - interpret its value (field_type)
 * - format its output (field_repr)
 *
 * @field_type: Semantic type of the field (see enum field_type),
 *              used by read_field() to correctly extract the value.
 *
 * @field_repr: Output representation format (see enum field_repr),
 *              used by repr_field() to format the value.
 *
 * @offset:     Byte offset of the field within the target struct.
 *              Used to compute field address from base pointer.
 *
 * @name:       Field name used in formatted output.
 *
 * @field_flags_func:
 *              Optional callback for formatting flag/mask values.
 *              Used when field_repr == REPR_FLAGS.
 *              If NULL, default formatting is applied.
 */
struct field_info {
        field_type field_type;
        field_repr field_repr;
        size_t offset;
        const char *name;
        field_flags_func_t field_flags_func;
};

/**
 * read_field - Extracts a field value from memory according to its semantic type.
 *
 * @field: Pointer to field memory (within traced struct).
 * @type:  Field type descriptor (see enum field_type).
 *
 * Reads raw field data from memory and converts it into a normalized
 * unsigned long long representation for further formatting.
 *
 * Note:
 * - Signed values are cast to unsigned long long (no sign preservation in return type).
 * - Pointer values are returned as integer addresses.
 * - The caller is responsible for interpreting the value via field_repr.
 *
 * Return: Field value converted to unsigned long long.
 */
unsigned long long read_field(void *field, field_type type);

/**
 * repr_field - Format a field value according to its metadata.
 *
 * @ctx:   Parser context.
 * @value: Field value normalized by read_field().
 * @field: Field metadata descriptor (see struct field_info).
 *
 * Applies output formatting based on field->field_repr and emits the result
 * using the corresponding FMT_* helpers.
 *
 * Special handling:
 * - REPR_FLAGS uses field->field_flags_func if provided.
 * - Other representations are handled via built-in formatters.
 *
 * This function is part of the formatting stage and assumes that the value
 * has already been correctly extracted and normalized.
 *
 * Return: 0 on success, non-zero on error.
 */
int repr_field(struct parser_ctx_struct *ctx, unsigned long long value, struct field_info *field);

/**
 * fmt_struct_common - Generic formatter for structures from tracee memory.
 *
 * @ctx:               Parser context.
 * @fmt_struct_func:   Callback used to format struct fields.
 * @addr:              Address of struct in tracee memory.
 * @size:              Size of struct to read.
 *
 * Reads a struct from the tracee address space into a local buffer and
 * formats it using the provided callback.
 *
 * The @fmt_struct_func is responsible for formatting individual fields
 * using the already copied struct buffer.
 *
 * Return:
 * 0 on success, non-zero on error (allocation, read failure, or formatting error).
 */
int fmt_struct_common(struct parser_ctx_struct *ctx,
                      fmt_struct_func_t fmt_struct_func, unsigned long long addr, size_t size);

/**
 * fmt_struct_generic - Generic formatter for struct fields using metadata table.
 *
 * @ctx:           Parser context.
 * @st:            Pointer to struct instance (already copied to local memory).
 * @fields:        Array of field descriptors (see struct field_info).
 * @fields_count:  Number of elements in @fields array.
 *
 * Iterates over provided field descriptors and formats each field
 * using the following pipeline:
 *   field_ptr = st + offset
 *   value     = read_field(field_ptr, field_type)
 *   repr_field(ctx, value, field)
 *
 * Output format:
 *   name=value pairs separated by FMT_SEPARATOR(ctx)
 *
 * Return:
 * 0 on success, non-zero on formatting error.
 */

int fmt_struct_generic(struct parser_ctx_struct *ctx,
                       void *st, struct field_info *fields, size_t fields_count);