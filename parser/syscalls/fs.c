/**
 * parser/syscalls/fs.c - Implementation of filesystem syscall parsers.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>

#include "parser/syscalls/syscall-table.h"
#include "parser/syscalls/parser.h"
#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/fs.h"

#include "parser/syscalls/args/fs/flags.h"
#include "parser/syscalls/args/fs/mask.h"
#include "parser/syscalls/args/fs/special.h"
#include "parser/syscalls/args/fs/struct.h"
#include "parser/syscalls/args/fs/value.h"

const struct parser_struct fs_syscalls[] = {
#define X(name, nr, has_enter, has_exit) \
        DEFINE_PARSER( \
                nr, \
                ENTER_IF_##has_enter(name), \
                EXIT_IF_##has_exit(name) \
        ), \

FS_SYSCALL_LIST
#undef X
};

const char *STDFD_NAMES[] = {
        [0] = "stdin",
        [1] = "stdout",
        [2] = "stderr",
};

/* read(int fd, void *buf, size_t count) */
DEFINE_SYSCALL_ENTER_PARSER(read)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        return 0;
};

DEFINE_SYSCALL_EXIT_PARSER(read)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* write(int fd, const void *buf, size_t count) */
DEFINE_SYSCALL_ENTER_PARSER(write)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
};

/* open(const char *path, int flags, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(open)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OPEN_FLAGS(ctx, args[1]);

        if (args[1] & O_CREAT) {
                FMT_SEPARATOR(ctx);
                FMT_OCT(ctx, args[2]);
        }

        return 0;
}

/* openat(int dirfd, const char *pathname, int flags, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(openat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OPEN_FLAGS(ctx, args[2]);

        if (args[2] & O_CREAT) {
                FMT_SEPARATOR(ctx);
                FMT_OCT(ctx, args[3]);
        }

        return 0;
}

/* close(int fd) */
DEFINE_SYSCALL_ENTER_PARSER(close)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* rename(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_ENTER_PARSER(rename)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath) */
DEFINE_SYSCALL_ENTER_PARSER(renameat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[3], NAME_MAX);

        return 0;
}

/* renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags) */
DEFINE_SYSCALL_ENTER_PARSER(renameat2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[3], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_RENAMEAT2_FLAGS(ctx, args[4]);

        return 0;
}

/* unlink(const char *path) */
DEFINE_SYSCALL_ENTER_PARSER(unlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* unlinkat(int dirfd, const char *pathname, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(unlinkat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[2]);

        return 0;
}

/* link(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_ENTER_PARSER(link)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(linkat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[3], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[4]);

        return 0;
}

/* symlink(const char *target, const char *linkpath) */
DEFINE_SYSCALL_ENTER_PARSER(symlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* symlinkat(const char *target, int newdirfd, const char *linkpath) */
DEFINE_SYSCALL_ENTER_PARSER(symlinkat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[2], NAME_MAX);

        return 0;
}

/* lseek(int fd, off_t offset, int whence) */
DEFINE_SYSCALL_ENTER_PARSER(lseek)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* pread64(int fd, void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_ENTER_PARSER(pread64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(pread64)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* pwrite64(int fd, const void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_ENTER_PARSER(pwrite64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* readlink(const char *path, char *buf, size_t count) */
DEFINE_SYSCALL_ENTER_PARSER(readlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(readlink)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz) */
DEFINE_SYSCALL_ENTER_PARSER(readlinkat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(readlinkat)
{
        FMT_STRING_MEM_IF_OK(ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);

        return 0;
}

/* getcwd(char *buf, size_t size) */
DEFINE_SYSCALL_EXIT_PARSER(getcwd)
{
        FMT_STRING_MEM_IF_OK(ctx, args[0], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);

        return 0;
}

/* chdir(const char *path) */
DEFINE_SYSCALL_ENTER_PARSER(chdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* mkdir(const char *pathname, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(mkdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* mkdirat(int dirfd, const char *pathname, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(mkdirat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[2]);

        return 0;
}

/* rmdir(const char *pathname) */
DEFINE_SYSCALL_ENTER_PARSER(rmdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* creat(const char *pathname, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(creat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* truncate(const char *path, off_t length */
DEFINE_SYSCALL_ENTER_PARSER(truncate)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);

        return 0;
}

/* ftruncate(int fd, off_t length) */
DEFINE_SYSCALL_ENTER_PARSER(ftruncate)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);

        return 0;
}

/* stat(const char *pathname, struct stat *statbuf) */
DEFINE_SYSCALL_ENTER_PARSER(stat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(stat)
{
        FMT_STRUCT_IF_OK(FMT_STAT64_STRUCT, ctx, args[1], ctx->retval);

        return 0;
}

/* fstat(int fd, struct stat *statbuf) */
DEFINE_SYSCALL_ENTER_PARSER(fstat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(fstat)
{
        FMT_STRUCT_IF_OK(FMT_STAT64_STRUCT, ctx, args[1], ctx->retval);

        return 0;
}

/* lstat(const char *pathname, struct stat *statbuf) */
DEFINE_SYSCALL_ENTER_PARSER(lstat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(lstat)
{
        FMT_STRUCT_IF_OK(FMT_STAT64_STRUCT, ctx, args[1], ctx->retval);

        return 0;
}

/* access(const char *pathname, int mode) */
DEFINE_SYSCALL_ENTER_PARSER(access)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ACCESS_MASK(ctx, args[1]);

        return 0;
}

/* chmod(const char *path, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(chmod)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* fchmod(int fd, mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(fchmod)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* chown(const char *path, uid_t owner, gid_t group) */
DEFINE_SYSCALL_ENTER_PARSER(chown)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag) */
DEFINE_SYSCALL_ENTER_PARSER(fchownat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[4]);

        return 0;
}

/* statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf) */
DEFINE_SYSCALL_ENTER_PARSER(statx)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_STATX_MASK(ctx, args[3]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(statx)
{
        FMT_STRUCT_IF_OK(FMT_STATX_STRUCT, ctx, args[4], ctx->retval);

        return 0;
}

/* statfs(const char *path, struct statfs *buf) */
DEFINE_SYSCALL_ENTER_PARSER(statfs)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(statfs)
{
        FMT_STRUCT_IF_OK(FMT_STATFS_STRUCT, ctx, args[1], ctx->retval);

        return 0;
}

/* fstatfs(int fd, struct statfs *buf) */
DEFINE_SYSCALL_ENTER_PARSER(fstatfs)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(fstatfs)
{
        FMT_STRUCT_IF_OK(FMT_STATFS_STRUCT, ctx, args[1], ctx->retval);

        return 0;
}

/* mknod(const char *path, mode_t mode, dev_t dev) */
DEFINE_SYSCALL_ENTER_PARSER(mknod)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
} 

/* mknodat(int dirfd, const char *path, mode_t mode, dev_t dev) */
DEFINE_SYSCALL_ENTER_PARSER(mknodat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);

        return 0;
}

/* fsync(int fd) */
DEFINE_SYSCALL_ENTER_PARSER(fsync)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* fdatasync(int fd) */
DEFINE_SYSCALL_ENTER_PARSER(fdatasync)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* dup(int oldfd) */
DEFINE_SYSCALL_ENTER_PARSER(dup)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* dup2(int oldfd, int newfd) */
DEFINE_SYSCALL_ENTER_PARSER(dup2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);

        return 0;
}

/* dup3(int oldfd, int newfd, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(dup3)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_DUP3_FLAGS(ctx, args[2]);

        return 0;
}

/* fcntl(int fd, int op, ...) */
DEFINE_SYSCALL_ENTER_PARSER(fcntl)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FCNTL_CMD(ctx, args[1]);

        if (ctx->extra != FCNTL_IGNORE_ARG3) {
                FMT_SEPARATOR(ctx);
                FMT_FCNTL_ARG3(ctx, args[2]);
        }

        return 0;
}

/* readv(int fd, const struct iovec *iov, int iovcnt) */
DEFINE_SYSCALL_ENTER_PARSER(readv)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(readv)
{
        FMT_STRUCT_IF_OK(FMT_IOVEC_STRUCT, ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* writev(int fd, const struct iovec *iov, int iovcnt) */
DEFINE_SYSCALL_ENTER_PARSER(writev)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_IOVEC_STRUCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset) */
DEFINE_SYSCALL_ENTER_PARSER(preadv)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(preadv)
{
        FMT_STRUCT_IF_OK(FMT_IOVEC_STRUCT, ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset) */
DEFINE_SYSCALL_ENTER_PARSER(pwritev)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_IOVEC_STRUCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* 
 * preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags)
 * 
 * NOTE: preadv2() and pwritev2() signatures in the Linux kernel and in glibc
 *       differ in one parameter pos_h which is passed in the r8 (args[4),
 *       but the parser of these calls ignores it
 */
DEFINE_SYSCALL_ENTER_PARSER(preadv2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(preadv2)
{
        FMT_STRUCT_IF_OK(FMT_IOVEC_STRUCT, ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_RWF_FLAGS(ctx, args[5]);

        return 0;
}

/* pwrite2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(pwritev2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_IOVEC_STRUCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_RWF_FLAGS(ctx, args[5]);

        return 0;
}

/* umask(mode_t mode) */
DEFINE_SYSCALL_ENTER_PARSER(umask)
{
        FMT_OCT(ctx, args[0]);

        return 0;
}

/* syncfs(int fd) */
DEFINE_SYSCALL_ENTER_PARSER(syncfs)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* fallocate(int fd, int mode, off_t offset, off_t size) */
DEFINE_SYSCALL_ENTER_PARSER(fallocate)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FALLOCATE_MASK(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* flock(int fd, int op) */
DEFINE_SYSCALL_ENTER_PARSER(flock)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FLOCK_OP(ctx, args[1]);

        return 0;
}

/* fadvise64(int fd, off_t offset, off_t len, int advice) */
DEFINE_SYSCALL_ENTER_PARSER(fadvise64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_FADVISE64_ADVICE(ctx, args[3]);

        return 0;
}

/* newfstatat(int dirfd, const char *pathname, struct stat *statbuf, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(newfstatat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(newfstatat)
{
        FMT_STRUCT_IF_OK(FMT_STAT64_STRUCT, ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[3]);

        return 0;
}

/* int setxattr(const char *path,
                const char *name, const void value[size], size_t size, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(setxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[2], args[3]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_XATTR_FLAGS(ctx, args[4]);

        return 0;
}

/* int lsetxattr(const char *path,
                 const char *name, const void value[size], size_t size, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(lsetxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[2], args[3]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_XATTR_FLAGS(ctx, args[4]);

        return 0;
}

/* int fsetxattr(int fd, const char *name, const void value[size], size_t size, int flags) */
DEFINE_SYSCALL_ENTER_PARSER(fsetxattr)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[2], args[3]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_XATTR_FLAGS(ctx, args[4]);

        return 0;
}

/* getxattr(const char *path, const char *name, void value[size], size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(getxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(getxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* lgetxattr(const char *path, const char *name, void value[size], size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(lgetxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(lgetxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* fgetxattr(int fd, const char *name, void value[size], size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(fgetxattr)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(fgetxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[2], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* listxattr(const char *path, char *list, size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(listxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(listxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[2]);

        return 0;
}

/* llistxattr(const char *path, char *list, size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(llistxattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(llistxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[2]);
        
        return 0;
}

/* flistxattr(int fd, char *list, size_t size) */
DEFINE_SYSCALL_ENTER_PARSER(flistxattr)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);

        return 0;
}

DEFINE_SYSCALL_EXIT_PARSER(flistxattr)
{
        FMT_STRING_MEM_IF_OK(ctx, args[1], ctx->retval);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[2]);

        return 0;
}

/* removexattr(const char *path, const char *name) */
DEFINE_SYSCALL_ENTER_PARSER(removexattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* lremovexattr(const char *path, const char *name) */
DEFINE_SYSCALL_ENTER_PARSER(lremovexattr)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* fremovexattr(int fd, const char *name) */
DEFINE_SYSCALL_ENTER_PARSER(fremovexattr)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* syscall(SYS_pivot_root, const char *new_root, const char *put_old) */
DEFINE_SYSCALL_ENTER_PARSER(pivot_root)
{
        FMT_STRING_MEM(ctx, args[0], PATH_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], PATH_MAX);

        return 0;
}

/* chroot(const char *path) */
DEFINE_SYSCALL_ENTER_PARSER(chroot)
{
        FMT_STRING_MEM(ctx, args[0], PATH_MAX);

        return 0;
}

/* sendfile(int out_fd, int in_fd, off_t *offset, size_t count) */
DEFINE_SYSCALL_ENTER_PARSER(sendfile)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[3]);

        return 0;
}

/* splice(int fd_in, off_t *off_in,
          int fd_out, off_t *off_out, size_t size, unsigned int flags) */
DEFINE_SYSCALL_ENTER_PARSER(splice)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[4]);
        FMT_SEPARATOR(ctx);
        FMT_SPLICE_FLAGS(ctx, args[5]);

        return 0;
}

/* vmsplice(int fd, const struct iovec *iov,
            size_t nr_segs, unsigned int flags) */
DEFINE_SYSCALL_ENTER_PARSER(vmsplice)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_IOVEC_STRUCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_LLU(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_SPLICE_FLAGS(ctx, args[3]);

        return 0;
}