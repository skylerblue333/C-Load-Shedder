#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int exit_code(const char *command) {
    int status = system(command);
    if (status == -1 || !WIFEXITED(status)) {
        return -1;
    }
    return WEXITSTATUS(status);
}

static int expect_exit(const char *command, int expected) {
    const int actual = exit_code(command);
    if (actual != expected) {
        fprintf(stderr, "command failed expectation: %s (expected %d, got %d)\n", command, expected, actual);
        return 1;
    }
    return 0;
}

int main(void) {
    int failures = 0;
    failures += expect_exit("./app 0 >/tmp/sky-load-accept.json", 0);
    failures += expect_exit("./app 849 1000 85 >/tmp/sky-load-below.json", 0);
    failures += expect_exit("./app 850 1000 85 >/tmp/sky-load-shed.json", 3);
    failures += expect_exit("./app 1001 1000 85 >/dev/null 2>&1", 2);
    failures += expect_exit("./app nope >/dev/null 2>&1", 2);
    failures += expect_exit("./app 10 0 85 >/dev/null 2>&1", 2);
    failures += expect_exit("./app 10 1000 101 >/dev/null 2>&1", 2);
    if (failures != 0) {
        return 1;
    }
    puts("load shedder contract tests passed");
    return 0;
}
