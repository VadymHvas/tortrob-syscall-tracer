#pragma once

#include <sys/types.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/ABI/abi.h"
#include "parser/syscalls/args/helpers.h"

#define SYSCALL_ENTER_PARSER_NAME(name) syscall_enter_##name##_parser
#define SYSCALL_EXIT_PARSER_NAME(name) syscall_exit_##name##_parser

/*
 * DEFINE_SYSCALL_*_PARSER(name)
 *
 * Helper macro to define a syscall entry/exit argument parser function with
 * a consistent signature.
 *
 * Each parser is responsible for formatting syscall arguments into
 * the provided buffer and updating the offset.
 */
#define DEFINE_SYSCALL_ENTER_PARSER(name)\
        int SYSCALL_ENTER_PARSER_NAME(name)(struct parser_ctx_struct *ctx, reg_t args[])

#define DEFINE_SYSCALL_EXIT_PARSER(name)\
        int SYSCALL_EXIT_PARSER_NAME(name)(struct parser_ctx_struct *ctx, reg_t args[])

/* 
 * These helpers map X-macro boolean flags (0/1) into
 * either a syscall parser function pointer or NULL.
 *
 * This allows conditional registration of entry/exit
 * parsers directly from the X-macro syscall table
 * without introducing runtime branching logic.
 */
#define ENTER_IF_1(name) SYSCALL_ENTER_PARSER_NAME(name)
#define ENTER_IF_0(name) NULL

#define EXIT_IF_1(name)  SYSCALL_EXIT_PARSER_NAME(name)
#define EXIT_IF_0(name)  NULL

#define DEFINE_PARSER(nr, enter_fn, exit_fn) \
        { \
                nr, \
                .enter_parse = enter_fn, \
                .exit_parse  = exit_fn, \
                .needs_enter = (enter_fn != NULL), \
                .needs_exit  = (exit_fn != NULL) \
        }

/**
 * parse_func_t - Callback type for syscall arguments formatting.
 * 
 * @ctx:  Parser context.
 * @args: Array of arguments in raw format (uint64_t).
 * 
 * Return: 0 on success, non-zero on error.
 */
typedef int (*parse_func_t) (struct parser_ctx_struct *ctx, reg_t args[]);

/**
 * struct parser_struct - Represents single syscall parser entry.
 * 
 * @nr:          The native syscall number.
 * @enter_parse: Function that formats syscall arguments in syscall enter.
 * @exit_parse:  Function that formats syscall arguments in syscall exit.
 * 
 * @needs_enter: non-zero if enter parser should be used.
 * @needs_exit:  non-zero if exit parser should be used.
 */
struct parser_struct {
        long nr;
        
        parse_func_t enter_parse;
        parse_func_t exit_parse;
        
        int needs_enter;
        int needs_exit;
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
int syscall_parse(struct parser_ctx_struct *ctx, const struct syscall_entry *syscall, reg_t args[]);