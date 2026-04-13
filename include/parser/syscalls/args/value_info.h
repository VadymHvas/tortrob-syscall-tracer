#pragma once

#define DEFINE_VALUES_ARRAY(name) static const struct value_info name[]

#define VALUES_ARR_SIZE(values_arr) sizeof(values_arr) / sizeof(values_arr[0])

#define INIT_VALUE(value) { value, #value }

struct value_info {
        unsigned long long value;
        const char *name;
};

int fmt_value_generic(struct parser_ctx_struct *ctx, int value,
                      const struct value_info *values, size_t values_count);