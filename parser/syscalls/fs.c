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

const struct parser_struct fs_syscalls[] = {
#define X(name, nr) { nr, SYSCALL_PARSER_NAME(name) },
FS_SYSCALL_LIST
#undef X
};

const char *STDFD_NAMES[] = {
        [0] = "stdin",
        [1] = "stdout",
        [2] = "stderr",
};

/* read(int fd, void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(read)
{
        if (!ctx->delayed) {
                FMT_FD(ctx, args[0]);
                FMT_SEPARATOR(ctx);

                ctx->delayed = DELAY;
        } else {
                FMT_STRING_MEM(ctx, args[1], args[2]);
                FMT_SEPARATOR(ctx);
                FMT_INT(ctx, args[2]);

                ctx->delayed = DELAY_NONE;
        }

        return 0;
};

/* write(int fd, const void *buf, size_t count) */
DEFINE_SYSCALL_PARSER(write)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
};

/* open(const char *path, int flags, mode_t mode) */
DEFINE_SYSCALL_PARSER(open)
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
DEFINE_SYSCALL_PARSER(openat)
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
DEFINE_SYSCALL_PARSER(close)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* rename(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(rename)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath) */
DEFINE_SYSCALL_PARSER(renameat)
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
DEFINE_SYSCALL_PARSER(renameat2)
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
DEFINE_SYSCALL_PARSER(unlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* unlinkat(int dirfd, const char *pathname, int flags) */
DEFINE_SYSCALL_PARSER(unlinkat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_AT_FLAGS(ctx, args[2]);

        return 0;
}

/* link(const char *oldpath, const char *newpath) */
DEFINE_SYSCALL_PARSER(link)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags) */
DEFINE_SYSCALL_PARSER(linkat)
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
DEFINE_SYSCALL_PARSER(symlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);

        return 0;
}

/* symlinkat(const char *target, int newdirfd, const char *linkpath) */
DEFINE_SYSCALL_PARSER(symlinkat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[2], NAME_MAX);

        return 0;
}

/* lseek(int fd, off_t offset, int whence) */
DEFINE_SYSCALL_PARSER(lseek)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* pread64(int fd, void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pread64)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* pwrite64(int fd, const void *buf, size_t count, off_t offset) */
DEFINE_SYSCALL_PARSER(pwrite64)
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
DEFINE_SYSCALL_PARSER(readlink)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz) */
DEFINE_SYSCALL_PARSER(readlinkat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[3]);

        return 0;
}

/* getcwd(char *buf, size_t size) */
DEFINE_SYSCALL_PARSER(getcwd)
{
        FMT_ADDR(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);

        return 0;
}

/* chdir(const char *path) */
DEFINE_SYSCALL_PARSER(chdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* mkdir(const char *pathname, mode_t mode) */
DEFINE_SYSCALL_PARSER(mkdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* mkdirat(int dirfd, const char *pathname, mode_t mode) */
DEFINE_SYSCALL_PARSER(mkdirat)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_STRING_MEM(ctx, args[1], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[2]);

        return 0;
}

/* rmdir(const char *pathname) */
DEFINE_SYSCALL_PARSER(rmdir)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);

        return 0;
}

/* creat(const char *pathname, mode_t mode) */
DEFINE_SYSCALL_PARSER(creat)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* truncate(const char *path, off_t length */
DEFINE_SYSCALL_PARSER(truncate)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);

        return 0;
}

/* ftruncate(int fd, off_t length) */
DEFINE_SYSCALL_PARSER(ftruncate)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[1]);

        return 0;
}

/* stat(const char *pathname, struct stat *statbuf) */
DEFINE_SYSCALL_PARSER(stat)
{
        if (!ctx->delayed) {
                FMT_STRING_MEM(ctx, args[0], NAME_MAX);
                FMT_SEPARATOR(ctx);

                ctx->delayed = DELAY;
        } else if (ctx->delayed) {
                FMT_STAT64_STRUCT(ctx, args[1]);

                ctx->delayed = DELAY_NONE;
        }

        return 0;
}

/* fstat(int fd, struct stat *statbuf) */
DEFINE_SYSCALL_PARSER(fstat)
{
        if (!ctx->delayed) {
                FMT_FD(ctx, args[0]);
                FMT_SEPARATOR(ctx);

                ctx->delayed = DELAY;
        } else if (ctx->delayed) {
                FMT_STAT64_STRUCT(ctx, args[1]);

                ctx->delayed = DELAY_NONE;
        }

        return 0;
}

/* lstat(const char *pathname, struct stat *statbuf) */
DEFINE_SYSCALL_PARSER(lstat)
{
        if (!ctx->delayed) {
                FMT_STRING_MEM(ctx, args[0], NAME_MAX);
                FMT_SEPARATOR(ctx);

                ctx->delayed = DELAY;
        } else if (ctx->delayed) {
                FMT_STAT64_STRUCT(ctx, args[1]);

                ctx->delayed = DELAY_NONE;
        }

        return 0;
}

/* access(const char *pathname, int mode) */
DEFINE_SYSCALL_PARSER(access)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ACCESS_MASK(ctx, args[1]);

        return 0;
}

/* chmod(const char *path, mode_t mode) */
DEFINE_SYSCALL_PARSER(chmod)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* fchmod(int fd, mode_t mode) */
DEFINE_SYSCALL_PARSER(fchmod)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);

        return 0;
}

/* chown(const char *path, uid_t owner, gid_t group) */
DEFINE_SYSCALL_PARSER(chown)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag) */
DEFINE_SYSCALL_PARSER(fchownat)
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
DEFINE_SYSCALL_PARSER(statx)
{
        if (!ctx->delayed) {
                FMT_FD(ctx, args[0]);
                FMT_SEPARATOR(ctx);
                FMT_STRING_MEM(ctx, args[1], NAME_MAX);
                FMT_SEPARATOR(ctx);
                FMT_AT_FLAGS(ctx, args[2]);
                FMT_SEPARATOR(ctx);
                FMT_STATX_MASK(ctx, args[3]);
                FMT_SEPARATOR(ctx);

                ctx->delayed = DELAY;
        } else if (ctx->delayed) {
                FMT_STATX_STRUCT(ctx, args[4]);

                ctx->delayed = DELAY_NONE;
        }

        return 0;
}

/* statfs(const char *path, struct statfs *buf) */
DEFINE_SYSCALL_PARSER(statfs)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);

        return 0;
}

/* fstatfs(int fd, struct statfs *buf) */
DEFINE_SYSCALL_PARSER(fstatfs)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);

        return 0;
}

/* mknod(const char *path, mode_t mode, dev_t dev) */
DEFINE_SYSCALL_PARSER(mknod)
{
        FMT_STRING_MEM(ctx, args[0], NAME_MAX);
        FMT_SEPARATOR(ctx);
        FMT_OCT(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
} 

/* mknodat(int dirfd, const char *path, mode_t mode, dev_t dev) */
DEFINE_SYSCALL_PARSER(mknodat)
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
DEFINE_SYSCALL_PARSER(fsync)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* fdatasync(int fd) */
DEFINE_SYSCALL_PARSER(fdatasync)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* dup(int oldfd) */
DEFINE_SYSCALL_PARSER(dup)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* dup2(int oldfd, int newfd) */
DEFINE_SYSCALL_PARSER(dup2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);

        return 0;
}

/* dup3(int oldfd, int newfd, int flags) */
DEFINE_SYSCALL_PARSER(dup3)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_FD(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_DUP3_FLAGS(ctx, args[2]);

        return 0;
}

/* fcntl(int fd, int op, ...) */
DEFINE_SYSCALL_PARSER(fcntl)
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
DEFINE_SYSCALL_PARSER(readv)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* writev(int fd, const struct iovec *iov, int iovcnt) */
DEFINE_SYSCALL_PARSER(writev)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);

        return 0;
}

/* preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset) */
DEFINE_SYSCALL_PARSER(preadv)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);

        return 0;
}

/* pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset) */
DEFINE_SYSCALL_PARSER(pwritev)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
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
DEFINE_SYSCALL_PARSER(preadv2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_RWF_FLAGS(ctx, args[5]);

        return 0;
}

/* pwrite2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags) */
DEFINE_SYSCALL_PARSER(pwritev2)
{
        FMT_FD(ctx, args[0]);
        FMT_SEPARATOR(ctx);
        FMT_ADDR(ctx, args[1]);
        FMT_SEPARATOR(ctx);
        FMT_INT(ctx, args[2]);
        FMT_SEPARATOR(ctx);
        FMT_OFF(ctx, args[3]);
        FMT_SEPARATOR(ctx);
        FMT_RWF_FLAGS(ctx, args[5]);

        return 0;
}

/* umask(mode_t mode) */
DEFINE_SYSCALL_PARSER(umask)
{
        FMT_OCT(ctx, args[0]);

        return 0;
}

/* syncfs(int fd) */
DEFINE_SYSCALL_PARSER(syncfs)
{
        FMT_FD(ctx, args[0]);

        return 0;
}

/* fallocate(int fd, int mode, off_t offset, off_t size) */
DEFINE_SYSCALL_PARSER(fallocate)
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