#include "utest.h"

#include <stddef.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

int test_misbehave() {
  const char str[] = "Hello world from a user program!\n";

  /* XXX: Currently kernel does not sigsegv offending programs, but in future it
     will, so this test will behave differently. */

  /* Successful syscall */
  assert(write(STDOUT_FILENO, str, sizeof(str) - 1) == sizeof(str) - 1);

  /* Pass bad pointer (kernel space) */
  assert(write(STDOUT_FILENO, (char *)0xDEADC0DE, 100) == -1);
  assert(errno == EFAULT);

  /* Pass bad pointer (user space) */
  assert(write(STDOUT_FILENO, (char *)0x1EE7C0DE, 100) == -1);
  assert(errno == EFAULT);

  return 0;
}

int test_exc_sigsys(void) {
  int retval = 0;
  int sysnum = 9999; /* large enough to be never implemented */
  asm volatile("move $v0, %1;"
               "syscall;"
               "sw $v0, %0"
               : "=m"(retval)
               : "r"(sysnum)
               : "memory", "v0");

  assert(retval == -78); /* XXX kernel ENOSYS */
  return 0;
}
