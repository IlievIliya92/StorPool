# Reverse Engineering

```{contents}
:depth: 3
```

## Task description

At [binaries.tgz](https://quiz.storpool.com/binaries.tgz) there’s an archive with
two executable binary files that crash in different ways. Can you explain why and
what are they trying to do

## Binary ./a

The binary is statically compiled with clang for x86-64 and with striped symbols.

```console
$ a: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, for GNU/Linux 3.2.0, BuildID[sha1]=f78fa493787cd1a399b33b8c872299feabab709c, stripped
```

The execution of `./a` is terminated by SIGSEGV signal. Using [strace](https://man7.org/linux/man-pages/man1/strace.1.html) to track the sys calls it is visible that the program tries to open a file
named './pesho' and read a signle block of data with length of 1024 bytes.

*stace output:*

```
execve("./a", ["./a"], 0x7ffd692321d0 /* 68 vars */) = 0
brk(NULL)                               = 0xcdd000
brk(0xcde1c0)                           = 0xcde1c0
arch_prctl(ARCH_SET_FS, 0xcdd880)       = 0
uname({sysname="Linux", nodename="iliya-ws", ...}) = 0
readlink("/proc/self/exe", "/home/iliya/Work/StorPool/StorPo"..., 4096) = 80
brk(0xcff1c0)                           = 0xcff1c0
brk(0xd00000)                           = 0xd00000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "./pesho", O_RDONLY)   = -1 ENOENT (No such file or directory)
read(-1, 0x7ffd96d7e770, 1024)          = -1 EBADF (Bad file descriptor)
close(-1)                               = -1 EBADF (Bad file descriptor)
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=NULL} ---
+++ killed by SIGSEGV (core dumped) +++
```

Possible C code corresponding to 'a':

```C
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    const char *filename = "./pesho";
    char buffer[1024];

    int fd = open(filename, O_RDONLY);
    /**
     * Missing check for a valid fd
     * The './pesho' file is missing on the file system or the relative path is
     * incorrect.
     */

    ssize_t n = read(fd, buffer, sizeof buffer);
    if (n == -1) {
        return -1;
    }
    close(fd);

    return 0;
}
```

## Binary ./b

The binary is statically compiled with clang for x86-64 and with striped symbols.

```console
$ b: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, for GNU/Linux 2.6.32, BuildID[sha1]=cffb08c07ba5e0fcd6c0860760955dab0b8c1707, stripped
```

The execution of `./b` is terminated by SIGSEGV signal. Using [strace](https://man7.org/linux/man-pages/man1/strace.1.html) to track the sys calls we get the following output.

*stace output:*

```
execve("./b", ["./b"], 0x7ffe21120bc0 /* 68 vars */) = 0
uname({sysname="Linux", nodename="iliya-ws", ...}) = 0
brk(NULL)                               = 0x249d000
brk(0x249e1c0)                          = 0x249e1c0
arch_prctl(ARCH_SET_FS, 0x249d880)      = 0
readlink("/proc/self/exe", "/home/iliya/Work/StorPool/StorPo"..., 4096) = 80
brk(0x24bf1c0)                          = 0x24bf1c0
brk(0x24c0000)                          = 0x24c0000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
--- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=NULL} ---
```

There seems to be an issue with the heap and the dynamic memory allocator.
The program doesn't reach the application code since there are no further sys calls
made after:

```
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
```

Statically linked against incompatible C library
