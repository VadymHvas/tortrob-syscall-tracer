#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/helpers.h"

/*
 * DEFINE_SYSCALL_PARSER(name)
 *
 * Helper macro to define a syscall argument parser function with
 * a consistent signature.
 *
 * Each parser is responsible for formatting syscall arguments into
 * the provided buffer and updating the offset.
 */
#define DEFINE_SYSCALL_PARSER(name)\
        int syscall_##name##_parser(struct parser_ctx_struct *ctx, raw_reg args[])

#define SYSCALL_PARSER_NAME(name) syscall_##name##_parser

/**
 * parse_func_t - Callback type for syscall arguments formatting.
 * 
 * @ctx:  Parser context.
 * @regs: Array of arguments in raw format (unsigned long long).
 */
typedef (*parse_func_t) (struct parser_ctx_struct *ctx, raw_reg regs[]);

/**
 * struct parser_struct - Represents single syscall parsers entry.
 * 
 * @nr:         The native syscall number.
 * @parse_func: Function that formats syscall arguments.
 */
struct parser_struct {
        long nr;
        parse_func_t parse_func;
};

/**
 * struct parser_group_struct - Groups multiple syscall parsers 
 *                              into a logical category (e.g., filesystem, process, network).
 * 
 * @entry: Pointer to an array of parser_struct.
 * @len:   Number of elements in the array.
 */
struct parser_group_struct {
        const struct parser_struct *entry;
        size_t len;
};

/**
 * syscall_parse - Dispatches a syscall to its corresponding argument parser.
 * 
 * This function performs a lookup based on syscall number and invokes
 * the appropriate parser. If no specific parser is found, a default
 * fallback parser is used.
 * 
 * @ctx: Parser context.
 * @syscall: Syscall metadata (nr and arguments count).
 * @args: Syscall arguments in raw format (unsigned long long).
 * 
 * @returns:
 *      - 0 on success
 *      - non-zero on failure
 */
int syscall_parse(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, raw_reg args[]);