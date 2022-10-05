// SPDX-License-Identifier: GPL-2.0
/*
 * Seccomp filter example for x86 (32-bit and 64-bit) with BPF macros
 *
 * Copyright (c) 2012 The Chromium OS Authors <chromium-os-dev@chromium.org>
 * Author: Will Drewry <wad@chromium.org>
 *
 * The code may be used by anyone for any purpose,
 * and can serve as a starting point for developing
 * applications using prctl(PR_SET_SECCOMP, 2, ...).
 *
 * This code has been modified as a skeleton for lab3 in 6.5810 at MIT.
 */
#define __USE_GNU 1
#define _GNU_SOURCE 1

#include <linux/types.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <errno.h>

#if defined(__i386__)
#define REG_RESULT	REG_EAX
#define REG_SYSCALL	REG_EAX
#define REG_ARG0	REG_EBX
#define REG_ARG1	REG_ECX
#define REG_ARG2	REG_EDX
#define REG_ARG3	REG_ESI
#define REG_ARG4	REG_EDI
#define REG_ARG5	REG_EBP
#elif defined(__x86_64__)
#define REG_RESULT	REG_RAX
#define REG_SYSCALL	REG_RAX
#define REG_ARG0	REG_RDI
#define REG_ARG1	REG_RSI
#define REG_ARG2	REG_RDX
#define REG_ARG3	REG_R10
#define REG_ARG4	REG_R8
#define REG_ARG5	REG_R9
#endif

#ifndef PR_SET_NO_NEW_PRIVS
#define PR_SET_NO_NEW_PRIVS 38
#endif

#ifndef SYS_SECCOMP
#define SYS_SECCOMP 1
#endif

#define MAX_LOG_LEN 4096
static char buf[MAX_LOG_LEN];

// this is reentrant, allowing you to print to STDOUT for debugging safely
void safe_log(const char *fmt, ...)
{
        va_list ptr;

        va_start(ptr, fmt);
        vsnprintf(buf, MAX_LOG_LEN, fmt, ptr);
        va_end(ptr);
	write(1, buf, strlen(buf));
}

static int install_emulator(void)
{
	// your code here
	return 0;
}

static int install_filter(void)
{
	// your code here
	return 0;
}

static int do_open(const char *pathname, int flags)
{
	return syscall(__NR_open, pathname, flags);
}

static int do_close(int fd)
{
	return syscall(__NR_close, fd);
}

static ssize_t do_read(int fd, void *buf, size_t count)
{
	return syscall(__NR_read, fd, buf, count);
}

static ssize_t do_write(int fd, const void *buf, size_t count)
{
	return syscall(__NR_write, fd, buf, count);
}

static int test_func(void)
{
	int total = 0;
	int ret, fd1, fd2, fd3, count;

	fd1 = do_open("apple", O_RDONLY);
	fd2 = do_open("cider", O_RDONLY);
	fd3 = do_open("donuts", O_RDONLY);
	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
		goto fail;

	ret = do_read(fd1, &count, 4);
	if (ret != 4 || count != 5)
		goto fail;
	total += count;

	ret = do_read(fd2, &count, 4);
	if (ret != 4 || count != 5)
		goto fail;
	total += count;

	ret = do_read(fd3, &count, 4);
	if (ret != 4 || count != 6)
		goto fail;
	total += count;

	do_close(fd1);
	do_close(fd2);
	do_close(fd3);

	if (total == 16) {
		do_write(1, "SUCCESS\n", 8);
		return 0;
	}

fail:
	do_write(1, "FAIL\n", 8);
	return 1;
}

int main(int argc, char **argv)
{
	if (install_emulator())
		return 1;
	if (install_filter())
		return 1;
	return test_func();
}
