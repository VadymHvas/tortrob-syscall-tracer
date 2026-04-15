#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/statvfs.h>
#include <sys/xattr.h>
#include <sys/inotify.h>
#include <sys/mount.h>

#include "parser/syscalls/args/helpers.h"
#include "parser/syscalls/args/flag_info.h"
#include "parser/syscalls/args/fs/flags.h"
#include "parser/syscalls/args/fs/mask.h"

DEFINE_FLAGS_ARRAY(open_flags) = {
        INIT_FLAG_INFO(O_CREAT),
        INIT_FLAG_INFO(O_EXCL),
        INIT_FLAG_INFO(O_TRUNC),
        INIT_FLAG_INFO(O_APPEND), 
        INIT_FLAG_INFO(O_NONBLOCK), 
        INIT_FLAG_INFO(O_DSYNC),
        INIT_FLAG_INFO(O_SYNC), 
        INIT_FLAG_INFO(O_RSYNC), 
        INIT_FLAG_INFO(O_CLOEXEC), 
        INIT_FLAG_INFO(O_DIRECTORY), 
        INIT_FLAG_INFO(O_NOFOLLOW),
        INIT_FLAG_INFO(O_TMPFILE)
};

DEFINE_FLAGS_ARRAY(open_modes) = {
        INIT_FLAG_INFO(O_RDONLY),
        INIT_FLAG_INFO(O_WRONLY),
        INIT_FLAG_INFO(O_RDWR)
};

DEFINE_FLAGS_ARRAY(at_flags) = {
        INIT_FLAG_INFO(AT_SYMLINK_FOLLOW),
        INIT_FLAG_INFO(AT_SYMLINK_NOFOLLOW),
        INIT_FLAG_INFO(AT_REMOVEDIR),
        INIT_FLAG_INFO(AT_NO_AUTOMOUNT),
        INIT_FLAG_INFO(AT_EMPTY_PATH)
};

DEFINE_FLAGS_ARRAY(statx_sync_modes) = {
        INIT_FLAG_INFO(AT_STATX_SYNC_AS_STAT),
        INIT_FLAG_INFO(AT_STATX_FORCE_SYNC),
        INIT_FLAG_INFO(AT_STATX_DONT_SYNC)
};

DEFINE_FLAGS_ARRAY(renameat2_flags) = {
        INIT_FLAG_INFO(RENAME_NOREPLACE),
        INIT_FLAG_INFO(RENAME_EXCHANGE),
        INIT_FLAG_INFO(RENAME_WHITEOUT)
};

DEFINE_FLAGS_ARRAY(rwf_flags) = {
        INIT_FLAG_INFO(RWF_DSYNC),
        INIT_FLAG_INFO(RWF_HIPRI),
        INIT_FLAG_INFO(RWF_SYNC),
        INIT_FLAG_INFO(RWF_NOWAIT),
        INIT_FLAG_INFO(RWF_APPEND)
};

DEFINE_FLAGS_ARRAY(st_file_types) = {
        INIT_FLAG_INFO(S_IFREG),
        INIT_FLAG_INFO(S_IFDIR),
        INIT_FLAG_INFO(S_IFCHR),
        INIT_FLAG_INFO(S_IFBLK),
        INIT_FLAG_INFO(S_IFLNK),
        INIT_FLAG_INFO(S_IFSOCK)
};

DEFINE_FLAGS_ARRAY(st_file_modes) = {
        INIT_FLAG_INFO(S_IRUSR),
        INIT_FLAG_INFO(S_IWUSR),
        INIT_FLAG_INFO(S_IXUSR),

        INIT_FLAG_INFO(S_IRGRP),
        INIT_FLAG_INFO(S_IWGRP),
        INIT_FLAG_INFO(S_IXGRP),

        INIT_FLAG_INFO(S_IROTH),
        INIT_FLAG_INFO(S_IWOTH),
        INIT_FLAG_INFO(S_IXOTH)
};

DEFINE_FLAGS_ARRAY(fflags_flags) = {
        INIT_FLAG_INFO(ST_RDONLY),
        INIT_FLAG_INFO(ST_WRITE),
        INIT_FLAG_INFO(ST_NOSUID),
        INIT_FLAG_INFO(ST_NODEV),
        INIT_FLAG_INFO(ST_NOEXEC),
        INIT_FLAG_INFO(ST_SYNCHRONOUS),
        INIT_FLAG_INFO(ST_MANDLOCK),
        INIT_FLAG_INFO(ST_APPEND),
        INIT_FLAG_INFO(ST_IMMUTABLE),
        INIT_FLAG_INFO(ST_NOATIME),
        INIT_FLAG_INFO(ST_NODIRATIME),
        INIT_FLAG_INFO(ST_RELATIME),
        INIT_FLAG_INFO(ST_NOSYMFOLLOW)
};

DEFINE_FLAGS_ARRAY(xattr_flags) = {
        INIT_FLAG_INFO(XATTR_CREATE),
        INIT_FLAG_INFO(XATTR_REPLACE)
};

DEFINE_FLAGS_ARRAY(splice_flags) = {
        INIT_FLAG_INFO(SPLICE_F_MOVE),
        INIT_FLAG_INFO(SPLICE_F_NONBLOCK),
        INIT_FLAG_INFO(SPLICE_F_MORE),
        INIT_FLAG_INFO(SPLICE_F_GIFT)
};

DEFINE_FLAGS_ARRAY(inotify_flags) = {
        INIT_FLAG_INFO(IN_NONBLOCK),
        INIT_FLAG_INFO(IN_CLOEXEC)
};

DEFINE_FLAGS_ARRAY(mount_flags) = {
        INIT_FLAG_INFO(MS_DIRSYNC),
        INIT_FLAG_INFO(MS_LAZYTIME),
        INIT_FLAG_INFO(MS_MANDLOCK),
        INIT_FLAG_INFO(MS_NOATIME),
        INIT_FLAG_INFO(MS_NODEV),
        INIT_FLAG_INFO(MS_NODIRATIME),
        INIT_FLAG_INFO(MS_NOEXEC),
        INIT_FLAG_INFO(MS_NOSUID),
        INIT_FLAG_INFO(MS_RDONLY),
        INIT_FLAG_INFO(MS_REC),
        INIT_FLAG_INFO(MS_RELATIME),
        INIT_FLAG_INFO(MS_SILENT),
        INIT_FLAG_INFO(MS_STRICTATIME),
        INIT_FLAG_INFO(MS_SYNCHRONOUS),
        INIT_FLAG_INFO(MS_NOSYMFOLLOW)
};

DEFINE_FLAGS_ARRAY(umount2_flags) = {
        INIT_FLAG_INFO(MNT_FORCE),
        INIT_FLAG_INFO(MNT_DETACH),
        INIT_FLAG_INFO(MNT_EXPIRE),
        INIT_FLAG_INFO(UMOUNT_NOFOLLOW),
};

DEFINE_FLAGS_ARRAY(fsopen_flags) = {
        INIT_FLAG_INFO(FSOPEN_CLOEXEC)
};

DEFINE_FLAGS_ARRAY(fsmount_flags) = {
        INIT_FLAG_INFO(FSMOUNT_CLOEXEC)      
};

DEFINE_FLAGS_ARRAY(fsmount_attr_flags) = {
        INIT_FLAG_INFO(MOUNT_ATTR_IDMAP),
        INIT_FLAG_INFO(MOUNT_ATTR_NOATIME),
        INIT_FLAG_INFO(MOUNT_ATTR_NODEV),
        INIT_FLAG_INFO(MOUNT_ATTR_NODIRATIME),
        INIT_FLAG_INFO(MOUNT_ATTR_NOEXEC),
        INIT_FLAG_INFO(MOUNT_ATTR_NOSUID),
        INIT_FLAG_INFO(MOUNT_ATTR_NOSYMFOLLOW),
        INIT_FLAG_INFO(MOUNT_ATTR_RDONLY),
        INIT_FLAG_INFO(MOUNT_ATTR_RELATIME),
        INIT_FLAG_INFO(MOUNT_ATTR_SIZE_VER0),
        INIT_FLAG_INFO(MOUNT_ATTR_STRICTATIME)
};

DEFINE_FLAGS_ARRAY(fspick_flags) = {
        INIT_FLAG_INFO(FSPICK_CLOEXEC),
        INIT_FLAG_INFO(FSPICK_EMPTY_PATH),
        INIT_FLAG_INFO(FSPICK_NO_AUTOMOUNT),
        INIT_FLAG_INFO(FSPICK_SYMLINK_NOFOLLOW)
};

DEFINE_FLAGS_ARRAY(open_tree_flags) = {
        INIT_FLAG_INFO(OPEN_TREE_CLOEXEC),
        INIT_FLAG_INFO(OPEN_TREE_CLONE)
};

DEFINE_FLAGS_ARRAY(move_mount_flags) = {
        INIT_FLAG_INFO(MOVE_MOUNT_T_SYMLINKS),
        INIT_FLAG_INFO(MOVE_MOUNT_T_EMPTY_PATH),
        INIT_FLAG_INFO(MOVE_MOUNT_T_AUTOMOUNTS),

        INIT_FLAG_INFO(MOVE_MOUNT_SET_GROUP),
        INIT_FLAG_INFO(MOVE_MOUNT_F_SYMLINKS),
        INIT_FLAG_INFO(MOVE_MOUNT_F_EMPTY_PATH),
        INIT_FLAG_INFO(MOVE_MOUNT_F_AUTOMOUNTS),
        INIT_FLAG_INFO(MOVE_MOUNT_BENEATH)
};

DEFINE_FLAGS_ARRAY(pipe2_flags) = {
        INIT_FLAG_INFO(O_CLOEXEC),
        INIT_FLAG_INFO(O_DIRECT),
        INIT_FLAG_INFO(O_NONBLOCK),
};

int fmt_open_flags(struct parser_ctx_struct *ctx, int flags)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = 1;
        int access_mode = flags & O_ACCMODE;

        FOR_EACH_FLAGS(open_modes) {
                if (access_mode == open_modes[i].flag) {
                        FMT_STRING(ctx, open_modes[i].name);

                        break;
                }
        }

        first = 0;
        flags &= ~O_ACCMODE;

        return fmt_flags_generic(
                ctx,
                flags,
                open_flags,
                FLAGS_ARR_SIZE(open_flags),
                first
        );
}

int fmt_at_flags(struct parser_ctx_struct *ctx, int flags)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        int first = 1;

        if (flags & AT_STATX_SYNC_TYPE) {
                int sync = flags & AT_STATX_SYNC_TYPE;

                FOR_EACH_FLAGS(statx_sync_modes) {
                        if (sync == statx_sync_modes[i].flag) {
                                FMT_FLAG_SEPARATOR(ctx, first);        
                                FMT_STRING(ctx, statx_sync_modes[i].name);
                                
                                first = 0;
                                break;
                        }

                        flags &= ~AT_STATX_SYNC_TYPE;
                }
        }

        return fmt_flags_generic(
                ctx, 
                flags,
                at_flags, 
                FLAGS_ARR_SIZE(at_flags), 
                first
        );
}

int fmt_renameat2_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                renameat2_flags,
                FLAGS_ARR_SIZE(renameat2_flags),
                1
        );
}

int fmt_dup3_flags(struct parser_ctx_struct *ctx, int flags)
{
        FMT_FLAGS_ZERO_IF_NONE(ctx, flags);

        if (flags == O_CLOEXEC)
                FMT_STRING(ctx, "O_CLOEXEC");
        else
                FMT_INT(ctx, flags);

        return 0;
}

int fmt_rwf_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                rwf_flags,
                FLAGS_ARR_SIZE(rwf_flags),
                1
        );
}

int fmt_st_mode_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        mode_t mode = 0;

        FOR_EACH_FLAGS(st_file_modes) {
                if (flags & st_file_modes[i].flag)
                        mode |= st_file_modes[i].flag;
        }

        flags &= S_IFMT;

        TRY_FMT(fmt_flags_generic, 
                ctx, flags, st_file_types, FLAGS_ARR_SIZE(st_file_types), 1);

        FMT_STRING(ctx, "|");
        
        FMT_OCT(ctx, mode);

        return 0;
}

int fmt_fflags_flags(struct parser_ctx_struct *ctx, long flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                fflags_flags,
                FLAGS_ARR_SIZE(fflags_flags),
                1
        );
}

int fmt_xattr_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                xattr_flags,
                FLAGS_ARR_SIZE(xattr_flags),
                1
        );
}

int fmt_splice_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                splice_flags,
                FLAGS_ARR_SIZE(splice_flags),
                1
        );
}

int fmt_inotify_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                inotify_flags,
                FLAGS_ARR_SIZE(inotify_flags),
                1
        );
}

int fmt_mount_flags(struct parser_ctx_struct *ctx, unsigned long flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                mount_flags,
                FLAGS_ARR_SIZE(mount_flags),
                1
        );
}

int fmt_umount2_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                umount2_flags,
                FLAGS_ARR_SIZE(umount2_flags),
                1
        );
}

int fmt_fsopen_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                fsopen_flags,
                FLAGS_ARR_SIZE(fsopen_flags),
                1
        );
}

int fmt_fsmount_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                fsmount_flags,
                FLAGS_ARR_SIZE(fsmount_flags),
                1
        );
}

int fmt_fsmount_attr_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                fsmount_attr_flags,
                FLAGS_ARR_SIZE(fsmount_attr_flags),
                1
        );
}

int fmt_fspick_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                fspick_flags,
                FLAGS_ARR_SIZE(fspick_flags),
                1
        );
}

int fmt_open_tree_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                open_tree_flags,
                FLAGS_ARR_SIZE(open_tree_flags),
                1
        );
}

int fmt_move_mount_flags(struct parser_ctx_struct *ctx, unsigned int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                move_mount_flags,
                FLAGS_ARR_SIZE(move_mount_flags),
                1
        );
}

int fmt_pipe2_flags(struct parser_ctx_struct *ctx, int flags)
{
        return fmt_flags_generic(
                ctx,
                flags,
                pipe2_flags,
                FLAGS_ARR_SIZE(pipe2_flags),
                1
        );
}