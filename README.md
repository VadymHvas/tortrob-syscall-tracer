# Tortrob Linux x86_64 syscall tracer
Simple syscall tracer based on ptrace.

## Features
- Creating and attaching at existing process.
- 100+ Linux syscalls parsers.
- Structs, arrays, strings, flags, masks, enums parsers.

### Parsing

#### Syscalls
90% coverage of fs related syscalls parsers, such as:
- file operations (open/at, close, write, read, pwrite64, pread64, writev, pwritev, readv, preadv, pwritev2, preadv2, fcntl)
- dir operations (mkdir/at, rmdir, chdir, getdents64)
- stat (stat, statx, fstat, lstat, newfstatat, statfs, fstatfs)
- fs operations (mount, umount2, fsopen, fsconfig, fsmount, fspick, move_mount, chroot, pivot_root)
- extended attributes (lsetxattr, fsetxattr, getxattr, lgetxattr, fgetxattr, listxattr, llistxattr, flistxattr, removexattr, lremovexattr, fremovexattr)
- inotify operations (inotify_init1, inotify_add_watch, inotify_rm_watch)

and others.


#### Syscall Arguments
Parsing of many structures, flags, masks, enums ​​used by these calls.
- structs (stat64, statfs, statx, timespec, flock)
- flags/masks (open flags/modes, _AT, xattr, fsopen, pipe2, inotify, mount, statx, access and others...)
- enums (fcntl, fadvise64, flock)

### Example
```
statx(AT_FDCWD, "test.txt", 0, STATX_BASIC_STATS|STATX_BTIME, {stx_mask=STATX_BASIC_STATS|STATX_BTIME|0x1000, stx_blksize=4096, stx_nlink=1, stx_uid=1000, stx_gid=1000, stx_mode=S_IFREG|644, stx_ino=7215728, stx_size=20}) = 0
fstat(stdout, {st_ino=3, st_nlink=1, st_mode=S_IFCHR|620, st_uid=1000, st_gid=5, st_rdev=136:0, st_size=0, st_blksize=1024, st_blocks=0}) = 0
write(stdout, "size: 20\n", 9size: 20
) = 9
openat(AT_FDCWD, "test.txt", O_RDWR|O_CREAT|O_CLOEXEC, 644) = 3
flock(3, LOCK_EX|LOCK_NB) = 0
write(3, "locked write\n", 13) = 13
fcntl(3, F_SETLK, {l_type=0, l_whence=SEEK_SET, l_start=0, l_len=10}) = 0
pipe2([4, 5], O_CLOEXEC|O_NONBLOCK) = 0
write(5, "pipe data\n\x00", 11) = 11
read(4, "pipe data\n\x00", 64) = 11
inotify_init1(IN_NONBLOCK|IN_CLOEXEC) = 6
inotify_add_watch(6, "test.txt", IN_CLOSE_WRITE|IN_MODIFY) = 1
write(3, "change\n", 7) = 7
read(6, "\x01\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 1024) = 16
statfs(".", {f_type=EXT2/EXT3/EXT4, f_bsize=4096, f_blocks=60984407, f_bfree=36724185, f_bavail=33608141, f_files=15564800, f_ffree=14972736, f_flags=ST_RELATIME|0x20}) = 0
write(stdout, "fs type: ef53\n", 14fs type: ef53
) = 14
...
```

## Build & Run
Clone the repository
```bash
git clone https://github.com/VadymHvas/tortrob-syscall-tracer.git

cd tortrob-syscall-tracer
```

compile it
```bash
make
```

run
```bash
./ttrace <prog>
```

or attach to process
```bash
./ttrace -p <PID>
```

clean
```bash
make clean
```

---
Happy hacking🐂!