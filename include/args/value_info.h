#pragma once

#include <sys/types.h>

#define DEFINE_VALUES_ARRAY(name) static const struct value_info name[]

#define VALUES_ARR_SIZE(values_arr) sizeof(values_arr) / sizeof(values_arr[0])

#define INIT_VALUE(value) { value, #value }

/**
 * struct value_info - structure to hold a value and its string representation.
 * 
 * @value: The actual value (e.g., an integer or a flag).
 * @name: The string representation of the value (e.g., "O_RDONLY").
 */
struct value_info {
        unsigned long long value;
        const char *name;
};

/**
 * fmt_value_generic - Generic function to format a value based on a provided array of value_info structures.
 * 
 * @param ctx: Parser context.
 * @param value: Value to format.
 * @param values: Array of value_info structures that map values to their string representations.
 * @param values_count: The number of elements in the values array.
 * 
 * Is used in syscall parsers to convert numeric values into human-readable strings based on predefined mappings.
 */
int fmt_value_generic(struct parser_ctx_struct *ctx, int value,
                      const struct value_info *values, size_t values_count);